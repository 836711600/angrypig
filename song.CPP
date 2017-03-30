#include<stdio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include <process.h>
#include <bios.h>
#include <alloc.h>
#include <string.h>
#include <math.h>
#include<io.h>
#include<mem.h>
#include<fcntl.h>
#include <stdarg.h>
#include <time.h>
/* wav文件结构 */
typedef struct WaveData
{
    unsigned long sample_lenth;
    unsigned short rate;
    unsigned short channels;
    unsigned char   time_constant;
    char            bit_res;
    char        *name_wav;
    char            *sample;
} WAV;
/* wav头信息结构 */
typedef struct HeaderType
{
    long            riff;        /*RIFF类资源文件头部*/
    unsigned long   file_len;    /*文件长度*/
    char            wave[4];     /*"WAVE"标志*/
    char            fmt [4];     /*"fmt"标志*/
    char            NI1 [4];     /*过渡字节*/
    unsigned short format_type;/*格式类别(10H为PCM形式的声音数据)*/
    unsigned short Channels;    /*Channels 1 = 单声道; 2 = 立体声*/
    long            frequency;   /*采样频率*/
    long            trans_speed;/*音频数据传送速率*/
    char            NI2 [2];     /*过渡字节*/
    short           sample_bits;/*样本的数据位数(8/16)*/
    char            data[4];     /*数据标记符"data"*/
    unsigned long   wav_len;     /*语音数据的长度*/
    char            NI3 [4];     /*过渡字节*/
} HEAD_WAV;
int G_base;              /* 记录DSP芯片的基址 */
int G_port;              /* 端口的基址 */        
int install_DSP();           /* 初始化声卡 */
int RestDSP(int Test);           /* 复位DSP芯片 */
int WriteDSP(int value);         /* 向DSP芯片写数据 */
WAV *Load_wav(char *name_wav);       /* 加载WAV相关信息 */
void Play_wav(WAV *Wav_file);        /* 播放加载好的 WAV 文件 */
void Destroy_wav(WAV *Wav_file);     /* 释放声音文件 */

int install_DSP()
{
     int i, g_address[6] = {0x210, 0x220, 0x230, 0x240, 0x250, 0x260};
     for (i = 0; i < 6; i++)
     {
         if (RestDSP(g_address[i]))    
         {
             G_port = g_address[i];     /* 记录端口基地址 */
             return 1;
         }
     } 
     return 0;
}
/****************************************************************************
检查一个声卡基址是否存在，如果存在则将声卡复位                              *
****************************************************************************/
int RestDSP(int Test)
{
     /* 重置DSP */
     outportb(Test + 0x6, 1);
     delay(5);   /* 延时是必须的 */
     outportb(Test + 0x6, 0);
     //delay(5);

     /* 如果重置成功则检查 */
     if ((inportb(Test + 0xE) & 0x80 == 0x80) 
         && (inportb(Test + 0xA) == 0xAA))    
     {
         G_base = Test;
         return 1;
     }   
     else
     {
         return 0;
     }
}
WAV *Load_wav(char *name_wav)
{
     FILE     *fp;
     HEAD_WAV     Wav_file_head;
     WAV      *Wav_file;  
     /* 打开声音文件 */
     if ((fp = fopen(name_wav, "rb")) == NULL)
     {
         printf("can't open wav file!");
         return NULL;
     }
     /* 开辟空间 */
     if ((Wav_file = (WAV *)malloc(sizeof(WAV))) == NULL)
     {
         printf("have't mem!");
         fclose(fp);
         return NULL;
     } 
     /* 读取文件头信息 */
     fread(&Wav_file_head, sizeof(HEAD_WAV), 1, fp);
/* 检查RIFF头 0x46464952 其实就是字符串 "RIFF" 
        这样好处理一些，毕竟比较数字比比较字符串要省事的多 */
     if (Wav_file_head.riff != 0x46464952)
     {
         printf("isn't wav file!");
         fclose(fp);
         return NULL;
     }
     /* 获取文件名 */
     Wav_file->name_wav = name_wav;
     Wav_file->rate = Wav_file_head.frequency;        /* 采样频率 */
     Wav_file->channels = Wav_file_head.Channels;     /* 声道 */
     /* 计算真实采样率 */
     Wav_file->time_constant = 256 - (1000000L / (Wav_file->rate * Wav_file->channels));
     /* 获取声音数据长度 */
     Wav_file->sample_lenth = Wav_file_head.file_len - 50;
     /* 分配内存，以存放声音数据 */
     if ((Wav_file->sample = (char *)malloc(Wav_file->sample_lenth)) == NULL)
     {
         printf("have't mem!");
         fclose(fp);
         return NULL;
     }
   /* 若文件过长 */
     if (Wav_file_head.file_len - 50 > 0xC7FF)
     {
     /* 读取声音数据 */
         fread(Wav_file->sample, 0xC7FF, 1, fp);
     }
     else
     {
     /* 读取声音数据 */
         fread(Wav_file->sample, Wav_file->sample_lenth, 1, fp);
     }
    fclose(fp);
      return Wav_file;
}
void Play_wav(WAV *Wav_file)
{
     FILE     *fp;
     long          LinearAddress;
     unsigned short page, offset;
     int      i;
     unsigned int     count;     /* 记录共分为几块播放 */
     unsigned int     romd;     /* 记录不满一块的数据段长度，以便最后依次播放 */
   /* 获取块数，和余数 */
     count = (int)(Wav_file->sample_lenth / 51199);
     romd   = (int)(Wav_file->sample_lenth % 51199); 
     if (romd != 0)
     {
         count++;
     }
     /* 将音频流指针转换成线性地址 */
     LinearAddress = FP_SEG(Wav_file->sample);
     LinearAddress = (LinearAddress << 4) + FP_OFF(Wav_file->sample);

     page    = LinearAddress >> 16;       /*计算页*/
     offset = LinearAddress & 0xFFFF;    /*计算页偏移*/


     /*注意 ：这个操作只能工作于DMA的第一通道*/
     outportb (0x0A, 5);                 /*Mask 锁DMA通道一*/
     outportb (0x0C, 0);                 /*清除DMA内部翻转标志*/
     outportb (0x0B, 0x49);              /*设置成回（播）放模式*/
     /*
     模式由下面几项组成:
     0x49 = 二进制 01 00 10 01
                   |   |   |   |
                   |   |   |   +- DMA通道 01
                   |   |   +---- 读操作 (从内存到DSP)
                   |   +------- 单一周期方式
                   +---------- 块方式
     */
    
     outportb ( 0x83, page);              /*将页面写入DMA控制器*/
     outportb ( 0x03, 0xC7FF & 0x100);
     outportb ( 0x03, 0xC7FF >> 8);
     /* 开启声卡 */
     WriteDSP(0xD1);    
   if (count == 1)              /* 文件长度 <= 51199bit */
     {
         WriteDSP(0x40);                      /* DSP第40h号命令 ：设置采样频率 */
         WriteDSP(Wav_file->time_constant ); /* Write time constant */
         outportb ( 0x02, offset & 0x100);   /*将偏移量写入DMA控制器*/
         outportb ( 0x02, offset >> 8);
         outportb ( 0x0A, 1 );                /*激活DMA通道一*/
         WriteDSP( 0x14 );                   /*DSP第14h号命令 ：单一周期回放*/
         WriteDSP( Wav_file->sample_lenth & 0xFF );
         WriteDSP( Wav_file->sample_lenth >> 8);  
     }
     else             /* 文件长度 > 51199bit */
     {
         fp = fopen(Wav_file->name_wav, "rb");
         fseek(fp, (long)(sizeof(HEAD_WAV) + 0xC7FF), SEEK_SET); 
       for (i = 0; i < count; i++)
         {
             WriteDSP(0x40);                      /* DSP第40h号命令 ：设置采样频率 */
             WriteDSP(Wav_file->time_constant ); /* Write time constant */
             outportb ( 0x02, offset & 0x100);   /*将偏移量写入DMA控制器*/
             outportb ( 0x02, offset >> 8);
   
             outportb ( 0x0A, 1 );                /*激活DMA通道一*/
             WriteDSP( 0x14 );                   /*DSP第14h号命令 ：单一周期回放*/

             if (i != (count - 1))
             {
                 WriteDSP( 0xC7FF & 0xFF );
                 WriteDSP( 0xC7FF >> 8);      
                 /*重置DSP*/
                 RestDSP(G_port); 
             fread(Wav_file->sample, 0xC7FF, 1, fp);
             }
             else
             {
             WriteDSP( romd & 0xFF );
                 WriteDSP( romd >> 8);        
                 /*重置DSP*/
                 RestDSP(G_port); 
             fread(Wav_file->sample, romd, 1, fp);
             }   
         }
         fclose(fp);
     }
    /*关闭声卡*/
    WriteDSP(0xD3);
    /*重置DSP*/
    RestDSP(G_port);   
}
int WriteDSP(int value)
{
    /*等待DSP接收一个字节*/
    while ((inportb(G_base + 0xC) & 0x80) == 0x80);
    /*发送字节*/
    outportb (G_base + 0xC, value);
}
void Destroy_wav(WAV *Wav_file)
{
     if (Wav_file)
     {
         free(Wav_file->sample);
         Wav_file->sample = NULL;
         free(Wav_file);
         Wav_file = NULL;
     }
}
void song(char* songname)
{
     WAV *Wav_file;         /* 申请音乐文件 */
     if (!install_DSP())         /* 初始化DSP */    
     {
         printf("can't install DSP!");
         getch();
         exit(0);
     }
                 /* 加载声音文件 */
     if ((Wav_file = Load_wav(songname)) == NULL)
     {
         printf("can't load wav!");
         getch();
         exit(0);
     }
     Play_wav(Wav_file);         /* 播放文件 */
     //getch();    
     Destroy_wav(Wav_file);     /* 销毁文件 */
}
