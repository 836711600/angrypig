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
/* wav�ļ��ṹ */
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
/* wavͷ��Ϣ�ṹ */
typedef struct HeaderType
{
    long            riff;        /*RIFF����Դ�ļ�ͷ��*/
    unsigned long   file_len;    /*�ļ�����*/
    char            wave[4];     /*"WAVE"��־*/
    char            fmt [4];     /*"fmt"��־*/
    char            NI1 [4];     /*�����ֽ�*/
    unsigned short format_type;/*��ʽ���(10HΪPCM��ʽ����������)*/
    unsigned short Channels;    /*Channels 1 = ������; 2 = ������*/
    long            frequency;   /*����Ƶ��*/
    long            trans_speed;/*��Ƶ���ݴ�������*/
    char            NI2 [2];     /*�����ֽ�*/
    short           sample_bits;/*����������λ��(8/16)*/
    char            data[4];     /*���ݱ�Ƿ�"data"*/
    unsigned long   wav_len;     /*�������ݵĳ���*/
    char            NI3 [4];     /*�����ֽ�*/
} HEAD_WAV;
int G_base;              /* ��¼DSPоƬ�Ļ�ַ */
int G_port;              /* �˿ڵĻ�ַ */        
int install_DSP();           /* ��ʼ������ */
int RestDSP(int Test);           /* ��λDSPоƬ */
int WriteDSP(int value);         /* ��DSPоƬд���� */
WAV *Load_wav(char *name_wav);       /* ����WAV�����Ϣ */
void Play_wav(WAV *Wav_file);        /* ���ż��غõ� WAV �ļ� */
void Destroy_wav(WAV *Wav_file);     /* �ͷ������ļ� */

int install_DSP()
{
     int i, g_address[6] = {0x210, 0x220, 0x230, 0x240, 0x250, 0x260};
     for (i = 0; i < 6; i++)
     {
         if (RestDSP(g_address[i]))    
         {
             G_port = g_address[i];     /* ��¼�˿ڻ���ַ */
             return 1;
         }
     } 
     return 0;
}
/****************************************************************************
���һ��������ַ�Ƿ���ڣ����������������λ                              *
****************************************************************************/
int RestDSP(int Test)
{
     /* ����DSP */
     outportb(Test + 0x6, 1);
     delay(5);   /* ��ʱ�Ǳ���� */
     outportb(Test + 0x6, 0);
     //delay(5);

     /* ������óɹ����� */
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
     /* �������ļ� */
     if ((fp = fopen(name_wav, "rb")) == NULL)
     {
         printf("can't open wav file!");
         return NULL;
     }
     /* ���ٿռ� */
     if ((Wav_file = (WAV *)malloc(sizeof(WAV))) == NULL)
     {
         printf("have't mem!");
         fclose(fp);
         return NULL;
     } 
     /* ��ȡ�ļ�ͷ��Ϣ */
     fread(&Wav_file_head, sizeof(HEAD_WAV), 1, fp);
/* ���RIFFͷ 0x46464952 ��ʵ�����ַ��� "RIFF" 
        �����ô���һЩ���Ͼ��Ƚ����ֱȱȽ��ַ���Ҫʡ�µĶ� */
     if (Wav_file_head.riff != 0x46464952)
     {
         printf("isn't wav file!");
         fclose(fp);
         return NULL;
     }
     /* ��ȡ�ļ��� */
     Wav_file->name_wav = name_wav;
     Wav_file->rate = Wav_file_head.frequency;        /* ����Ƶ�� */
     Wav_file->channels = Wav_file_head.Channels;     /* ���� */
     /* ������ʵ������ */
     Wav_file->time_constant = 256 - (1000000L / (Wav_file->rate * Wav_file->channels));
     /* ��ȡ�������ݳ��� */
     Wav_file->sample_lenth = Wav_file_head.file_len - 50;
     /* �����ڴ棬�Դ���������� */
     if ((Wav_file->sample = (char *)malloc(Wav_file->sample_lenth)) == NULL)
     {
         printf("have't mem!");
         fclose(fp);
         return NULL;
     }
   /* ���ļ����� */
     if (Wav_file_head.file_len - 50 > 0xC7FF)
     {
     /* ��ȡ�������� */
         fread(Wav_file->sample, 0xC7FF, 1, fp);
     }
     else
     {
     /* ��ȡ�������� */
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
     unsigned int     count;     /* ��¼����Ϊ���鲥�� */
     unsigned int     romd;     /* ��¼����һ������ݶγ��ȣ��Ա�������β��� */
   /* ��ȡ������������ */
     count = (int)(Wav_file->sample_lenth / 51199);
     romd   = (int)(Wav_file->sample_lenth % 51199); 
     if (romd != 0)
     {
         count++;
     }
     /* ����Ƶ��ָ��ת�������Ե�ַ */
     LinearAddress = FP_SEG(Wav_file->sample);
     LinearAddress = (LinearAddress << 4) + FP_OFF(Wav_file->sample);

     page    = LinearAddress >> 16;       /*����ҳ*/
     offset = LinearAddress & 0xFFFF;    /*����ҳƫ��*/


     /*ע�� ���������ֻ�ܹ�����DMA�ĵ�һͨ��*/
     outportb (0x0A, 5);                 /*Mask ��DMAͨ��һ*/
     outportb (0x0C, 0);                 /*���DMA�ڲ���ת��־*/
     outportb (0x0B, 0x49);              /*���óɻأ�������ģʽ*/
     /*
     ģʽ�����漸�����:
     0x49 = ������ 01 00 10 01
                   |   |   |   |
                   |   |   |   +- DMAͨ�� 01
                   |   |   +---- ������ (���ڴ浽DSP)
                   |   +------- ��һ���ڷ�ʽ
                   +---------- �鷽ʽ
     */
    
     outportb ( 0x83, page);              /*��ҳ��д��DMA������*/
     outportb ( 0x03, 0xC7FF & 0x100);
     outportb ( 0x03, 0xC7FF >> 8);
     /* �������� */
     WriteDSP(0xD1);    
   if (count == 1)              /* �ļ����� <= 51199bit */
     {
         WriteDSP(0x40);                      /* DSP��40h������ �����ò���Ƶ�� */
         WriteDSP(Wav_file->time_constant ); /* Write time constant */
         outportb ( 0x02, offset & 0x100);   /*��ƫ����д��DMA������*/
         outportb ( 0x02, offset >> 8);
         outportb ( 0x0A, 1 );                /*����DMAͨ��һ*/
         WriteDSP( 0x14 );                   /*DSP��14h������ ����һ���ڻط�*/
         WriteDSP( Wav_file->sample_lenth & 0xFF );
         WriteDSP( Wav_file->sample_lenth >> 8);  
     }
     else             /* �ļ����� > 51199bit */
     {
         fp = fopen(Wav_file->name_wav, "rb");
         fseek(fp, (long)(sizeof(HEAD_WAV) + 0xC7FF), SEEK_SET); 
       for (i = 0; i < count; i++)
         {
             WriteDSP(0x40);                      /* DSP��40h������ �����ò���Ƶ�� */
             WriteDSP(Wav_file->time_constant ); /* Write time constant */
             outportb ( 0x02, offset & 0x100);   /*��ƫ����д��DMA������*/
             outportb ( 0x02, offset >> 8);
   
             outportb ( 0x0A, 1 );                /*����DMAͨ��һ*/
             WriteDSP( 0x14 );                   /*DSP��14h������ ����һ���ڻط�*/

             if (i != (count - 1))
             {
                 WriteDSP( 0xC7FF & 0xFF );
                 WriteDSP( 0xC7FF >> 8);      
                 /*����DSP*/
                 RestDSP(G_port); 
             fread(Wav_file->sample, 0xC7FF, 1, fp);
             }
             else
             {
             WriteDSP( romd & 0xFF );
                 WriteDSP( romd >> 8);        
                 /*����DSP*/
                 RestDSP(G_port); 
             fread(Wav_file->sample, romd, 1, fp);
             }   
         }
         fclose(fp);
     }
    /*�ر�����*/
    WriteDSP(0xD3);
    /*����DSP*/
    RestDSP(G_port);   
}
int WriteDSP(int value)
{
    /*�ȴ�DSP����һ���ֽ�*/
    while ((inportb(G_base + 0xC) & 0x80) == 0x80);
    /*�����ֽ�*/
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
     WAV *Wav_file;         /* ���������ļ� */
     if (!install_DSP())         /* ��ʼ��DSP */    
     {
         printf("can't install DSP!");
         getch();
         exit(0);
     }
                 /* ���������ļ� */
     if ((Wav_file = Load_wav(songname)) == NULL)
     {
         printf("can't load wav!");
         getch();
         exit(0);
     }
     Play_wav(Wav_file);         /* �����ļ� */
     //getch();    
     Destroy_wav(Wav_file);     /* �����ļ� */
}