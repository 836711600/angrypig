#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include "vesa24b1.h"

//VBE信息块数据结构
struct VBEController
{
  char signature[4];						//字符串"VESA"
  unsigned short version;					//VBE版本,BCD码
  char  far *oem_string;					//指向OEM厂商的标志串的指针
  long capabilities;						//显示特性
  unsigned far *video_mode;					//指向所支持的显示模式列表的指针
  unsigned short total_memory;				//显卡内存大小，单位64K
//VBE2.0以上接口
  unsigned short oem_software_rev;			//VBE软件的OEM修订版本号,BCD码
  char far *oem_vendor_name;				//指向显卡制造商的字符串的指针
  char far *oem_product_name;				//指向显卡产品名称的字符串的指针
  char far *oem_product_rev;				//指向显卡修订版本号或产品等级的字符串的指针
  char reserved[222];						//保留
  char oem_data[256];						//VESA2.0版以上定义
};

//VBE 特定模式信息块数据结构
struct VBEMode
{
  unsigned short mode_attributes;			//模式的属性
  unsigned char  win_a_attributes;			//窗口A的属性
  unsigned char  win_b_attributes;			//窗口B的属性
  unsigned short win_granularity;			//窗口的粒度,单位KB
  unsigned short win_size;					//窗口的大小,单位KB
  unsigned short win_a_segment;				//窗口A的起始地址
  unsigned short win_b_segment;				//窗口B的起始地址
  void (far *win_func_ptr)(void);			//换页调用入口
  unsigned short bytes_per_scanline;		//每一条水平扫描线所占用的字节数

  unsigned short x_resolution;				//水平方向的分辨率
  unsigned short y_resolution;				//垂直方向的分辨率
  unsigned char x_char_size;				//字符的宽度
  unsigned char y_char_size;				//字符的高度
  unsigned char number_of_planes;			//位平面的个数
  unsigned char bits_per_pixel;				//每像素的位数
  unsigned char number_of_banks;			//CGA逻辑扫描线分组数
  unsigned char memory_model;				//显示内存模式
  unsigned char bank_size;					//CGA每组扫描线的大小
  unsigned char number_of_image_pages;		//可同时载入的最大满屏图像数
  unsigned char reserved0;					//为页面功能保留

  // 对直接写颜色模式的定义区域
  unsigned char red_mask_size;				//红色所占用的位数
  unsigned char red_field_position;			//红色的最低有效位位置
  unsigned char green_mask_size;			//绿色所占用的位数
  unsigned char green_field_position;		//绿色的最低有效位位置
  unsigned char blue_mask_size;				//蓝色所占用的位数
  unsigned char blue_field_position;		//蓝色的最低有效位位置
  unsigned char reserved_mask_size;			//保留色占用的位数
  unsigned char reserved_field_position;	//保留色的最低有效位位置
  unsigned char direct_color_mode_info;		//直接颜色模式属性

  //VBE2.0以上接口
  char far *phys_base;						//可使用大的祯缓存时为指向其首地址的32位物理地址
  long reserved1;							//祯缓存首地址的32位偏移量
  unsigned short reversed2;					//可用的、连续的显示缓冲区,KB单位

  //VBE3.0以上接口
  unsigned short linear_bytes_per_scanline;	//线形缓冲区中每一条扫描线的长度,字节为单位
  char banked_number_of_image_pages;		//使用窗口功能时的显示页面数
  char linear_number_of_image_pages;		//使用大的线形缓冲区时的显示页面数
  char linear_red_mask_size;				//使用大的线形缓冲区时的红色所占的位数
  char linear_red_field_position;			//使用大的线形缓冲区时的红色的最低有效位位置
  char linear_green_mask_size;				//使用大的线形缓冲区时的绿色所占的位数
  char linear_green_field_position;			//使用大的线形缓冲区时的绿色的最低有效位位置
  char linear_blue_mask_size;				//使用大的线形缓冲区时的蓝色所占的位数
  char linear_blue_field_position;			//使用大的线形缓冲区时的蓝色的最低有效位位置
  char linear_reserved_mask_size;			//使用大的线形缓冲区时的保留色所占的位数
  char linear_reserved_field_position;		//使用大的线形缓冲区时的保留色的最低有效位位置
  unsigned long max_pixel_clock;			//图形模式下最大像素时钟,单位HZ

  char reserved3[189];						//保留
};

typedef struct tagNODE
{
    float curx;
    float dx;
    int   maxy;
    struct tagNODE *next;
} NODE, *PNODE;
/***********************************全 局 变 量  ******************************************/

struct VBEController far Vbeinfo;	//VBE信息块
struct VBEMode far Modeinfo;		//VBE特定模式信息块
unsigned char far * G_VWIN1;			//64K模式显存首地址
static int  bytesperline;			//每一条水平扫描线所占用的字节数
static int  g_cur_vbe_page=0;		//当前页
static unsigned int  bankshift;		//页粒度

static int  vesa_xmax;				//水平方向的分辨率
static int  vesa_ymax;				//垂直方向的分辨率
static int  DrawMode=VESA_COPY;		//划线模式
static RGBBMP24  DrawColor={0,0,0};	//画图颜色色
/*****************************************************************************************/


/*****************************************************************************************/
//1、函数名：
//			Getvbeinfo

//2、功能：
//			取当前显卡VBE信息块数据

//3、参数：
//		输入参数：
//			AX=0X4F00		VBE功能00H
//			ES:DI			指向VBE信息块指针
//		输出参数：
//			AX=VBE			返回VBE信息块

//4、返回值：
//			-1：			没有VESA VBE信息
//			 1：			正确
/*****************************************************************************************/
static int far Getvbeinfo(void)
{
	union REGS in,out;
	struct SREGS segs;
	char far *p=(char far *)&Vbeinfo;
	in.x.ax = 0x4F00;
	in.x.di = FP_OFF(p);
	segs.es = FP_SEG(p);
	int86x(0x10, &in, &out, &segs);
	if(out.x.ax==0x4F)
		return 1;
	return -1;
}

/*****************************************************************************************/
//1、函数名：
//			Getmodeinfo

//2、功能：
//			取当前显卡VBE特定显示模式信息块数据

//3、参数：
//		输入参数：
//			AX=0X4F01		VBE功能01H
//			CX=模式号		显示模式
//			ES:DI			指向VBE特定显示模式信息块指针
//			mode			显示模式
//		输出参数：
//			AX=VBE			返回VBE特定显示模式信息块

//4、返回值：
//			-1：			取当前显卡VBE特定显示模式信息块数据错误
//			 1：			取当前显卡VBE特定显示模式信息块数据正确
/*****************************************************************************************/
static int far Getmodeinfo(int mode)
{
   union REGS in,out;
   struct SREGS segs;
   char far *p=(char far *)&Modeinfo;
   if (mode < 0x100)
   {
      return -1;
   }
   in.x.ax = 0x4F01;
   in.x.cx = mode;
   in.x.di = FP_OFF(p);
   segs.es = FP_SEG(p);
   int86x(0x10, &in, &out, &segs);
   if(out.x.ax != 0x4F)
	   return -1;

   if((Modeinfo.mode_attributes&0x01)
	   &&(Modeinfo.memory_model==6)
	   &&(Modeinfo.bits_per_pixel==32)
	   &&(Modeinfo.number_of_planes==1))
	   return 1;
   return -1;

}

/*****************************************************************************************/
//1、函数名：
//			SetVBEMode

//2、功能：
//			设置VESA VBE显示模式

//3、参数：
//		输入参数：
//			AX=0X4F02		VBE功能02H
//			BX=模式号		显示模式
//			mode			显示模式
//		输出参数：
//			AX=VBE			VBE返回值
//			AH				返回错误号

//4、返回值：
//			-1：			不支持当前设置的显示模式
//			 1：			支持当前设置的显示模式
/*****************************************************************************************/
static int far SetVBEMode(int mode)
{
	_AX = 0x4F02;
    _BX = mode;
    __int__(0x10);
    if (_AH != 0)
    {
        return -1;
    }
	return 1;
}

/*****************************************************************************************/
//1、函数名：
//			InitVesa

//2、功能：
//			初始化VESA VBE显示模式

//3、参数：
//		输入参数：
//			mode			显示模式
//		输出参数：
//			无

//4、返回值：
//			-1：			设置显示模式失败
//			 1：			设置显示模式成功
/*****************************************************************************************/
int far InitVesa(int mode)
{
	if(Getvbeinfo()==-1)
	{
		printf("No VESA VBE detected\n");
		return -1;
	}
    if(Getmodeinfo(mode)==-1)
	{
		printf("Valid video mode not find\n");
		return -1;
	}
	bytesperline=Modeinfo.bytes_per_scanline;
	vesa_xmax=Modeinfo.x_resolution;
	vesa_ymax=Modeinfo.y_resolution;
	bankshift=0;
	while((unsigned)(64>>bankshift)!=Modeinfo.win_granularity)
		bankshift++;
	g_cur_vbe_page=-1;
	G_VWIN1=(unsigned char far *)(((long)0xA000)<<16|0);
	if(SetVBEMode(mode)==-1)
	{
		printf("Mode is not support\n");
		return -1;
	}

	return 1;
}


/*****************************************************************************************/
//1、函数名：
//			CloseVesa

//2、功能：
//			返回VGA文本显示模式

//3、参数：
//		输入参数：
//			AX=0X03
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far CloseVesa()
{
	_AX = 0x3;
   __int__(0x10);
}

/*****************************************************************************************/
//1、函数名：
//			Setvbepage

//2、功能：
//			显卡换页

//3、参数：
//		输入参数：
//			AX=0X4F05		VBE功能05H
//			BX=0
//			DX=page			换页的页号
//			page			换页的页号
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far Setvbepage(int page)
{
	if(g_cur_vbe_page != page)
	{
	  g_cur_vbe_page=page;
	  page<<=bankshift;
	  _BX = 0;
	  _DX =page;
	  _AX = 0x4F05;
	  __int__(0x10);
	}
}

/*****************************************************************************************/
//1、函数名：
//			VesaPutPixel

//2、功能：
//			设置当前像素点颜色

//3、参数：
//		输入参数：
//			x				当前像素点X坐标
//			y				当前像素点Y坐标
//			color			画笔颜色
//			mode			操作模式(COPY、NOT、OR、XOR、AND)
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaPutPixel(int x,int y,unsigned long color)
{
	unsigned int page,off;
	unsigned long addr;
	unsigned char far * video_start_ptr=G_VWIN1;
	if(x>=vesa_xmax||y>=vesa_ymax)
		return;
	addr=(unsigned long)y*bytesperline+x*4;
	page = (unsigned int)(addr >> 16);
	off = addr&0xffff;
	Setvbepage(page);
	switch(DrawMode)
	{
	case VESA_AND:
		*((unsigned long far *)(video_start_ptr + off)) &= color;
		break;
	case VESA_OR:
		*((unsigned long far *)(video_start_ptr + off)) |= color;
		break;
	case VESA_NOT:
		*((unsigned long far *)(video_start_ptr + off)) =~ *((unsigned long far *)(video_start_ptr + off));
		break;
	case VESA_XOR:
		*((unsigned long far *)(video_start_ptr + off)) ^= color;
		break;
	case VESA_COPY:
		*((unsigned long far *)(video_start_ptr + off)) = color;
		break;
	}
}

/*****************************************************************************************/
//1、函数名：
//			VesaPutBMP

//2、功能：
//			设置当前像素点颜色

//3、参数：
//		输入参数：
//			x				当前像素点X坐标
//			y				当前像素点Y坐标
//			color			画笔颜色
//			mode			操作模式(COPY、NOT、OR、XOR、AND)
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/

void far VesaPutBMP(int x,int y, int width, int hight, RGBBMP24 *buf,long transparent)
{
	int i,j,k,width1,hight1,bmplinebytes;
	unsigned long addr,m;
	unsigned int page,off,n;
	unsigned char far * video_start_ptr=G_VWIN1;
	unsigned long far * v;
	RGBBMP24 *p;
	char *s;

	if(width%4)                    // 拼凑4字节倍数
		bmplinebytes = width*3+(4-(width*3)%4);
	else
		bmplinebytes = width*3;

	if(x>=vesa_xmax||y>=vesa_ymax||width<=0||hight<=0)
		return;
	width1=width;
	if(x+width>=vesa_xmax)
		width1=vesa_xmax-x;
	hight1=hight;
	if(y+1-hight<0)
		hight1=y+1;
	for(j=0,s=(char *) buf;j<hight1;j++,s+=bmplinebytes)
	{
		addr=(unsigned long)(y-j)*bytesperline+x*4;
		page=(unsigned int)(addr >> 16);
		off=addr&0xffff;
		n=off+width1*4;
		if(n<off) n=0;
		n=(n-off)/4;
		p=(RGBBMP24 *) s;
		for(k=width1;k>0;)
		{
			v=(unsigned long far *) (video_start_ptr+off);
			Setvbepage(page);
			for(i=0;i<n;i++,v++,p++)
			{
				m = (* (unsigned long *) p)&0xffffff;

				if(m!=transparent)
				{
					switch(DrawMode)
					{
						case VESA_AND:
							*v &= m;
							break;
						case VESA_OR:
							*v |= m;
							break;
						case VESA_NOT:
							*v = ~ *v;
							break;
						case VESA_XOR:
							*v ^= m;
							break;
						case VESA_COPY:
							*v = m;
							break;
					}
				}
			}
			k-=n;
			n=k;
			page++;
			off=0;
		}
	}
}

/*****************************************************************************************/
//1、函数名：
//			VesaGetBMP

//2、功能：
//			设置当前像素点颜色

//3、参数：
//		输入参数：
//			x				当前像素点X坐标
//			y				当前像素点Y坐标
//			color			画笔颜色
//			mode			操作模式(COPY、NOT、OR、XOR、AND)
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/

void far VesaGetBMP(int x,int y, int width, int hight, RGBBMP24 *buf)
{
	int i,j,k,width1,hight1,bmplinebytes;
	unsigned long addr;
	unsigned int page,off,n;
	unsigned char far * video_start_ptr=G_VWIN1;
	unsigned long far * v;
	RGBBMP24 *p;
	char *s;

	if(width%4)                    // 拼凑4字节倍数
		bmplinebytes = width*3+(4-(width*3)%4);
	else
		bmplinebytes = width*3;

	if(x>=vesa_xmax||y>=vesa_ymax||width<=0||hight<=0)
		return;
	width1=width;
	if(x+width>=vesa_xmax)
		width1=vesa_xmax-x;
	hight1=hight;
	if(y+1-hight<0)
		hight1=y+1;
	for(j=0,s=(char *) buf;j<hight1;j++,s+=bmplinebytes)
	{
		addr=(unsigned long)(y-j)*bytesperline+x*4;
		page=(unsigned int)(addr >> 16);
		off=addr&0xffff;
		n=off+width1*4;
		if(n<off) n=0;
		n=(n-off)/4;
		p=(RGBBMP24 *) s;
		for(k=width1;k>0;)
		{
			v=(unsigned long far *) (video_start_ptr+off);
			Setvbepage(page);
			for(i=0;i<n;i++,p++,v++)
				*p = * (RGBBMP24 *) v;
			k-=n;
			n=k;
			page++;
			off=0;
		}
	}
}

/*****************************************************************************************/
//1、函数名：
//			VesaGetPixel

//2、功能：
//			取当前像素点颜色

//3、参数：
//		输入参数：
//			x				当前像素点X坐标
//			y				当前像素点Y坐标
//		输出参数：
//			无

//4、返回值：
//			当前像素点颜色
/*****************************************************************************************/
unsigned long far VesaGetPixel(int x, int y)
{
	unsigned int page,off;
	unsigned long addr;
	unsigned char far * video_start_ptr=G_VWIN1;
	addr=(unsigned long)y*bytesperline+x*4;
	page = (unsigned int)(addr >> 16);
	off = addr & 0xffff;
	Setvbepage(page);
	return *((unsigned long far *)(video_start_ptr + off));
}

/*****************************************************************************************/
//1、函数名：
//			VesaVline

//2、功能：
//			画垂直直线

//3、参数：
//		输入参数：
//			x				垂直直线X坐标
//			y1				垂直直线起点Y坐标
//			y2				垂直直线终点Y坐标
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaVline(int x, int y1, int y2)
{
	int i;
	for (i = y1; i <= y2; i++)
	VesaPutPixel(x,i,*(unsigned long *)&DrawColor);
}

/*****************************************************************************************/
//1、函数名：
//			VesaHline

//2、功能：
//			画水平直线

//3、参数：
//		输入参数：
//			x1				水平直线起点X坐标
//			y				水平直线Y坐标
//			x2				水平直线终点X坐标
//			color			画笔颜色
//			mode			操作模式(COPY、NOT、OR、XOR、AND)
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaHline(int x1, int y, int x2)
{
	int i,width;
	unsigned long addr,m;
	unsigned int page,off,n;
	unsigned char far * video_start_ptr=G_VWIN1;
	unsigned long far * v1;

	width=x2-x1;
	addr=(unsigned long) y*bytesperline+x1*4;
	page=(unsigned int)(addr >> 16);
	off=addr&0xffff;
	n=off+width*4;
	if(n<off) n=0;
	n=(n-off)/4;
	m=*(unsigned long *) &DrawColor;
	while(width)
	{
		v1=(unsigned long far *) (video_start_ptr+off);
		Setvbepage(page);
		for(i=0;i<n;i++,v1++)
		{
			switch(DrawMode)
			{
				case VESA_AND:
					*v1 &= m;
					break;
				case VESA_OR:
					*v1 |= m;
					break;
				case VESA_NOT:
					*v1 = ~ *v1;
					break;
				case VESA_XOR:
					*v1 ^= m;
					break;
				case VESA_COPY:
					*v1 = m;
					break;
			}
		}
		width-=n;
		n=width;
		page++;
		off=0;
	}
}

/*****************************************************************************************/
//1、函数名：
//			VesaLine

//2、功能：
//			画任意直线

//3、参数：
//		输入参数：
//			x1				直线起点X坐标
//			y1				直线起点Y坐标
//			x2				直线终点X坐标
//			y2				直线终点Y坐标
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaLine(int x1,int y1,int x2,int y2)
{
	int step_x,step_y,change;
	int dt_x=x2-x1;
	int dt_y=y2-y1;
	if (dt_x<0)		//从右向左画线
	{
		dt_x=-dt_x;
		step_x=-1;
	}
	else			//从左向右画线
		step_x=1;
	if (dt_y<0)		//从下向上画线
	{
		dt_y=-dt_y;
		step_y = -1;
	}
	else			//从上向下画线
		step_y=1;
	if (dt_x > dt_y)    //x改变得比y快
	{
		change = dt_x >> 1;
		while (x1 != x2)
		{
			VesaPutPixel(x1, y1, *(unsigned long *)&DrawColor);
			x1 += step_x;
			change += dt_y;
			if(change > dt_x)
			{
				y1 += step_y;
				change -= dt_x;
			}
		}
	}
	else                 //y改变得比x快
	{
		change = dt_y >> 1;
		while(y1 != y2)
		{
			VesaPutPixel(x1, y1, *(unsigned long *)&DrawColor);
			y1 += step_y;
			change += dt_x;
			if(change > dt_y)
			{
				x1 += step_x;
				change -= dt_y;
			}
		}
	}
	VesaPutPixel(x2, y2, *(unsigned long *)&DrawColor);
}

/*****************************************************************************************/
//1、函数名：
//			VesaRect

//2、功能：
//			画矩形

//3、参数：
//		输入参数：
//			x0				矩形左上角X坐标
//			y0				矩形左上角Y坐标
//			x1				矩形右下角X坐标
//			y1				矩形右下角Y坐标
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaRect(int x0, int y0, int x1, int y1)
{
	VesaHline(x0, y0, x1);
	VesaHline(x0, y1, x1);
	VesaVline(x0, y0, y1);
	VesaVline(x1, y0, y1);
}

/*****************************************************************************************/
//1、函数名：
//			VesaBar

//2、功能：
//			填充矩形

//3、参数：
//		输入参数：
//			x1				矩形左上角X坐标
//			y1				矩形左上角Y坐标
//			x2				矩形右下角X坐标
//			y2				矩形右下角Y坐标
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaBar(int x1, int y1, int x2, int y2)
{
	int i;
    for (i = y1; i <= y2; ++i)
        VesaHline(x1, i, x2);
}

/*****************************************************************************************/
//1、函数名：
//			VesaCircle

//2、功能：
//			画圆

//3、参数：
//		输入参数：
//			c_x				圆心X坐标
//			c_y				圆心Y坐标
//			r				半径
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaCircle(int c_x, int c_y, int r)
{
	int x = 0, y = r, d0, d1, d2, direction;
    d0 = (1-r) << 1;
    while (y >= 0)
    {
	VesaPutPixel(c_x + x, c_y + y, *(unsigned long *)&DrawColor);
	VesaPutPixel(c_x - x, c_y - y, *(unsigned long *)&DrawColor);
	VesaPutPixel(c_x + x, c_y - y, *(unsigned long *)&DrawColor);
	VesaPutPixel(c_x - x, c_y + y, *(unsigned long *)&DrawColor);
        if (d0 < 0)
        {
            d1 = ((d0 + y) << 1) -1;
            if (d1 <= 0) direction = 1;
            else direction = 2;
        }
        else if (d0 > 0)
		{
            d2 = ((d0 - x) << 1) - 1;
            if (d2 <= 0) direction = 2;
            else direction = 3;
        }
        else direction = 2;

        switch(direction)
        {
            case 1 : ++x;
                d0 += (x<<1) + 1;
                break;
            case 2 : ++x;
                --y;
                d0+=(x-y+1)<<1;
                break;
            case 3 : --y;
                d0 += -(y<<1) + 1;
				break;
        }
    }
}


/*****************************************************************************************/
//1、函数名：
//			VesaCircleFill

//2、功能：
//			填充圆

//3、参数：
//		输入参数：
//			x0				圆心X坐标
//			y0				圆心Y坐标
//			r				半径
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaCircleFill(int x0, int y0, int r)
{
    int x,y;
    int deltax,deltay;
    int d;

    x = 0;
    y = r;
    deltax = 3;
    deltay = 2 - (r << 1);
    d = 1 - r;

	while (x <= y)
    {
        VesaHline(x0-x, y0+y, x0+x);
        VesaHline(x0-x, y0-y, x0+x);
        VesaHline(x0-y, y0-x, x0+y);
        VesaHline(x0-y, y0+x, x0+y);
        VesaHline(x0-x, y0+y, x0+x);
        if(d<0)
        {
            d += deltax;
            deltax += 2;
            x++;
        }
        else
        {
            d += deltax+deltay;
            deltax += 2;
            deltay += 2;
			x++;
            y--;
         }
    }
}
/*****************************************************************************************/
//1、函数名：
//			VesaFillPoly

//2、功能：
//			//绘制和填充多变形

//3、参数：
//		输入参数：
//			n				点个数
//			pp				整数序列(n*2个)，每一整数对给出多边形的一个顶点的X和Y坐标
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
//绘制和填充多变形
void far VesaFillPoly(int n,int far *pp)
{
	PNODE  *NET  = NULL;
    PNODE  AET  = NULL;
    char   flag;
    NODE   *pnew;
    NODE   *ptemp;
    PNODE  *ppnode;
    int    pgminy;
    int    pgmaxy;
    int    miny;
    int    i;

	// 计算 pgminy 和 pgmaxy
    pgminy = pp[0 * 2 + 1];
    pgmaxy = pp[0 * 2 + 1];
    for (i=0; i<n; i++)
    {
        if (pp[i * 2 + 1] < pgminy) pgminy = pp[i * 2 + 1];
        if (pp[i * 2 + 1] > pgmaxy) pgmaxy = pp[i * 2 + 1];
    }

    // 在纵坐标方向对多边形进行平移变换
    for (i=0; i<n; i++) pp[i * 2 + 1] -= pgminy;

    // 创建并初始化新边表
    NET = (PNODE*)malloc((pgmaxy - pgminy + 1) * sizeof(PNODE));
    if (!NET)
		goto error_handler;
    else
		memset(NET, 0, (pgmaxy - pgminy + 1) * sizeof(PNODE));

    // 把多边形的各个边放入新边表中
    for (i=0; i<n; i++)
    {
        if (pp[i * 2 + 1] == pp[((i + 1) % n) * 2 + 1])
			continue;

        // 创建新结点
        pnew = (NODE*)malloc(sizeof(NODE));
        if (!pnew)
			goto error_handler;

        // 计算并填充新结点
        pnew->dx  = (float)(pp[i * 2 + 0] - pp[((i + 1) % n) * 2 + 0]);
        pnew->dx /= (float)(pp[i * 2 + 1] - pp[((i + 1) % n) * 2 + 1]);
        if (pp[i * 2 + 1] > pp[((i + 1) % n) * 2 + 1])
        {
            miny = pp[((i + 1) % n) * 2 + 1];
			pnew->curx = (float)pp[((i + 1) % n) * 2 + 0];
            pnew->maxy = pp[i * 2 + 1];
        }
        else
        {
            miny = pp[i * 2 + 1];
            pnew->curx = (float)pp[i * 2 + 0];
            pnew->maxy = pp[((i + 1) % n) * 2 + 1];
        }

        //加入新边表中
        ppnode = &NET[miny];
        while (*ppnode != NULL && (*ppnode)->dx > pnew->dx)
			ppnode = &((*ppnode)->next);
        pnew->next = *ppnode;
        *ppnode = pnew;
    }

	// 处理经过多边形的每一条扫描线
    for (i=0; i<=pgmaxy-pgminy; i++)
    {
        // 删除活边表中无用结点
        ppnode = &AET;
        while (*ppnode != NULL)
        {
            if ((*ppnode)->maxy <= i)
            {
                ptemp   = *ppnode;
                *ppnode = ptemp->next;
                free(ptemp);
            }
            else
				ppnode = &((*ppnode)->next);
        }

        // 把新边表中的边结点插入活边表中
		while (NET[i] != NULL)
        {
            // 计算插入位置
            ppnode = &AET;
            while (*ppnode != NULL && NET[i]->curx > (*ppnode)->curx)
				ppnode = &((*ppnode)->next);

            // 插入结点
            ptemp  = NET[i];
            NET[i] = ptemp->next;
            ptemp->next = *ppnode;
            *ppnode = ptemp;
        }

        // 绘制多边形填充的扫描线
        flag = 1;
        for (ptemp = AET; ptemp != NULL && ptemp->next != NULL; ptemp = ptemp->next)
        {
			if (flag)
				VesaHline((int)ptemp->curx, i + pgminy, (int)ptemp->next->curx);
			flag= (-1)*flag;
        }

        // 更新活边表中的结点
        ptemp = AET;
        while (ptemp != NULL)
        {
            ptemp->curx += ptemp->dx;
            ptemp = ptemp->next;
        }
    }

error_handler:
    // 释放内存
    for (i=0; i<=pgmaxy-pgminy; i++)
    {
		while (NET[i])
        {
            ptemp  = NET[i];
            NET[i] = ptemp->next;
            free(ptemp);
        }
    }
    while (AET)
    {
        ptemp = AET;
        AET   = ptemp->next;
        free(ptemp);
    }
    free(NET);

    // 在纵坐标方向对多边形进行平移变换
    for (i=0; i<n; i++)
		pp[i * 2 + 1] += pgminy;
}
/*****************************************************************************************/
//1、函数名：
//			VesaSetColor

//2、功能：
//			设置画图颜色

//3、参数：
//		输入参数：
//			index				颜色RGB
//		输出参数：
//			无

//4、返回值：
//			无
/*****************************************************************************************/
void far VesaSetColor(unsigned long index)
{
	memcpy(&DrawColor,&index,sizeof(RGBBMP24));
//	DrawColor.r=index.r;
//	DrawColor.g=index.g;
//	DrawColor.b=index.b;
}



/*****************************************************************************************/
//1、函数名：
//			VesaSetWriteMode

//2、功能：
//			设置画图逻辑标志模式

//3、参数：
//		输入参数：
//			mode				逻辑标志模式
//		输出参数：
//			无

//4、返回值：
//		    无
/*****************************************************************************************/
void  far VesaSetWriteMode(int mode)
{
	if((mode>=VESA_COPY)&&(mode<=VESA_NOT))
		DrawMode=mode;
}

/*****************************************************************************************/
//1、函数名：
//			VesaGetWriteMode

//2、功能：
//			取当画图逻辑标志模式

//3、参数：
//		输入参数：
//			无
//		输出参数：
//			无

//4、返回值：
//		    画图逻辑标志模式
/*****************************************************************************************/
int far VesaGetWriteMode()
{
	return DrawMode;
}


/*****************************************************************************************/
//1、函数名：
//			VesaSetFillStyle

//2、功能：
//			设置填充模式

//3、参数：
//		输入参数：
//			mode:			没有用，仿bc31图形库
//			color:			填充颜色
//		输出参数：
//			无

//4、返回值：
//		    无
/*****************************************************************************************/
void far VesaSetFillStyle(unsigned long color)
{
	VesaSetColor(color);
}

/*****************************************************************************************/
int far ShowBmp24B(int x,int y,char* FileName)
{
	FILE *fp;
	long  Width, Height, bmplinebytes;
	int i, j, Lines;
	long transparent;
	RGBBMP24 *buffer=NULL;

	if((fp = fopen(FileName, "rb")) == NULL)
	{
		printf("can't open the file!");
		getch();
		return -1;
	}
	fseek(fp, 18L, SEEK_SET);       // 读取文件宽高
	fread(&Width, sizeof(Width), 1, fp);
	fread(&Height, sizeof(Height), 1, fp);
	if(Width%4)                    // 拼凑4字节倍数
		bmplinebytes = Width*3+(4-(Width*3)%4);
	else
		bmplinebytes = Width*3;
	buffer=(RGBBMP24  *)malloc(bmplinebytes*16);
	if(buffer==NULL)
	{
		printf("NO memory");
		fclose(fp);
		getch();
		return -1;
	}

	fseek(fp, 54L, SEEK_SET);
	transparent=-1;
	for(i=0;i<Height;i+=16)
	{
		Lines=Height-i;
		if(Lines>16) Lines=16;
		fread(buffer, bmplinebytes, Lines, fp);// 一次读取一行
//		if(transparent==-1)
//			transparent=(* (unsigned long *) buffer)&0xffffff;
		VesaPutBMP(x, y-i, Width, Lines, buffer, transparent);
//		for (j = 0; j < Width; j++)
//		{
//			VesaPutPixel(x+j, y+i, *(unsigned long *)&buffer[j]);
//		}

	}
	fclose(fp);
	if(buffer)
		free(buffer);
	return 1;
}

int far ShowIcon24B(int x,int y,ICON* icon)
{

	VesaPutBMP(x, y, icon->width, icon->height, icon->buf, icon->transparent);
        return(1);
}

int far ReadIconBmp24B(ICON *icon, char* FileName)
{
	FILE *fp;
	unsigned long  bmplinebytes, total_length;

	if((fp = fopen(FileName, "rb")) == NULL)
	{
		printf("can't open the file!");
		getch();
		return -1;
	}
	fseek(fp, 18L, SEEK_SET);       // 读取文件宽高
	fread(&icon->width, 2, 1, fp);
	fseek(fp, 22L, SEEK_SET);       // 读取文件宽高
	fread(&icon->height, 2, 1, fp);
	if(icon->width%4)                    // 拼凑4字节倍数
		bmplinebytes = icon->width*3+(4-(icon->width*3)%4);
	else
		bmplinebytes = icon->width*3;
	total_length=bmplinebytes*icon->height;
	icon->buf=(RGBBMP24  *)malloc(total_length);
	if(icon->buf==NULL)
	{
		printf("NO memory");
		fclose(fp);
		getch();
		return -1;
	}

	fseek(fp, 54L, SEEK_SET);
	fread(icon->buf, bmplinebytes, icon->height, fp);// 一次读取一行
	icon->transparent=(* (unsigned long *) icon->buf)&0xffffff;
	fclose(fp);
	return 1;
}

int far CloseIcon(ICON *icon)
{
	if(icon->buf!=NULL)
		free(icon->buf);
	icon->buf=NULL;
	return(1);
}