#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include "vesa24b1.h"

//VBE��Ϣ�����ݽṹ
struct VBEController
{
  char signature[4];						//�ַ���"VESA"
  unsigned short version;					//VBE�汾,BCD��
  char  far *oem_string;					//ָ��OEM���̵ı�־����ָ��
  long capabilities;						//��ʾ����
  unsigned far *video_mode;					//ָ����֧�ֵ���ʾģʽ�б���ָ��
  unsigned short total_memory;				//�Կ��ڴ��С����λ64K
//VBE2.0���Ͻӿ�
  unsigned short oem_software_rev;			//VBE������OEM�޶��汾��,BCD��
  char far *oem_vendor_name;				//ָ���Կ������̵��ַ�����ָ��
  char far *oem_product_name;				//ָ���Կ���Ʒ���Ƶ��ַ�����ָ��
  char far *oem_product_rev;				//ָ���Կ��޶��汾�Ż��Ʒ�ȼ����ַ�����ָ��
  char reserved[222];						//����
  char oem_data[256];						//VESA2.0�����϶���
};

//VBE �ض�ģʽ��Ϣ�����ݽṹ
struct VBEMode
{
  unsigned short mode_attributes;			//ģʽ������
  unsigned char  win_a_attributes;			//����A������
  unsigned char  win_b_attributes;			//����B������
  unsigned short win_granularity;			//���ڵ�����,��λKB
  unsigned short win_size;					//���ڵĴ�С,��λKB
  unsigned short win_a_segment;				//����A����ʼ��ַ
  unsigned short win_b_segment;				//����B����ʼ��ַ
  void (far *win_func_ptr)(void);			//��ҳ�������
  unsigned short bytes_per_scanline;		//ÿһ��ˮƽɨ������ռ�õ��ֽ���

  unsigned short x_resolution;				//ˮƽ����ķֱ���
  unsigned short y_resolution;				//��ֱ����ķֱ���
  unsigned char x_char_size;				//�ַ��Ŀ���
  unsigned char y_char_size;				//�ַ��ĸ߶�
  unsigned char number_of_planes;			//λƽ��ĸ���
  unsigned char bits_per_pixel;				//ÿ���ص�λ��
  unsigned char number_of_banks;			//CGA�߼�ɨ���߷�����
  unsigned char memory_model;				//��ʾ�ڴ�ģʽ
  unsigned char bank_size;					//CGAÿ��ɨ���ߵĴ�С
  unsigned char number_of_image_pages;		//��ͬʱ������������ͼ����
  unsigned char reserved0;					//Ϊҳ�湦�ܱ���

  // ��ֱ��д��ɫģʽ�Ķ�������
  unsigned char red_mask_size;				//��ɫ��ռ�õ�λ��
  unsigned char red_field_position;			//��ɫ�������Чλλ��
  unsigned char green_mask_size;			//��ɫ��ռ�õ�λ��
  unsigned char green_field_position;		//��ɫ�������Чλλ��
  unsigned char blue_mask_size;				//��ɫ��ռ�õ�λ��
  unsigned char blue_field_position;		//��ɫ�������Чλλ��
  unsigned char reserved_mask_size;			//����ɫռ�õ�λ��
  unsigned char reserved_field_position;	//����ɫ�������Чλλ��
  unsigned char direct_color_mode_info;		//ֱ����ɫģʽ����

  //VBE2.0���Ͻӿ�
  char far *phys_base;						//��ʹ�ô��������ʱΪָ�����׵�ַ��32λ������ַ
  long reserved1;							//�������׵�ַ��32λƫ����
  unsigned short reversed2;					//���õġ���������ʾ������,KB��λ

  //VBE3.0���Ͻӿ�
  unsigned short linear_bytes_per_scanline;	//���λ�������ÿһ��ɨ���ߵĳ���,�ֽ�Ϊ��λ
  char banked_number_of_image_pages;		//ʹ�ô��ڹ���ʱ����ʾҳ����
  char linear_number_of_image_pages;		//ʹ�ô�����λ�����ʱ����ʾҳ����
  char linear_red_mask_size;				//ʹ�ô�����λ�����ʱ�ĺ�ɫ��ռ��λ��
  char linear_red_field_position;			//ʹ�ô�����λ�����ʱ�ĺ�ɫ�������Чλλ��
  char linear_green_mask_size;				//ʹ�ô�����λ�����ʱ����ɫ��ռ��λ��
  char linear_green_field_position;			//ʹ�ô�����λ�����ʱ����ɫ�������Чλλ��
  char linear_blue_mask_size;				//ʹ�ô�����λ�����ʱ����ɫ��ռ��λ��
  char linear_blue_field_position;			//ʹ�ô�����λ�����ʱ����ɫ�������Чλλ��
  char linear_reserved_mask_size;			//ʹ�ô�����λ�����ʱ�ı���ɫ��ռ��λ��
  char linear_reserved_field_position;		//ʹ�ô�����λ�����ʱ�ı���ɫ�������Чλλ��
  unsigned long max_pixel_clock;			//ͼ��ģʽ���������ʱ��,��λHZ

  char reserved3[189];						//����
};

typedef struct tagNODE
{
    float curx;
    float dx;
    int   maxy;
    struct tagNODE *next;
} NODE, *PNODE;
/***********************************ȫ �� �� ��  ******************************************/

struct VBEController far Vbeinfo;	//VBE��Ϣ��
struct VBEMode far Modeinfo;		//VBE�ض�ģʽ��Ϣ��
unsigned char far * G_VWIN1;			//64Kģʽ�Դ��׵�ַ
static int  bytesperline;			//ÿһ��ˮƽɨ������ռ�õ��ֽ���
static int  g_cur_vbe_page=0;		//��ǰҳ
static unsigned int  bankshift;		//ҳ����

static int  vesa_xmax;				//ˮƽ����ķֱ���
static int  vesa_ymax;				//��ֱ����ķֱ���
static int  DrawMode=VESA_COPY;		//����ģʽ
static RGBBMP24  DrawColor={0,0,0};	//��ͼ��ɫɫ
/*****************************************************************************************/


/*****************************************************************************************/
//1����������
//			Getvbeinfo

//2�����ܣ�
//			ȡ��ǰ�Կ�VBE��Ϣ������

//3��������
//		���������
//			AX=0X4F00		VBE����00H
//			ES:DI			ָ��VBE��Ϣ��ָ��
//		���������
//			AX=VBE			����VBE��Ϣ��

//4������ֵ��
//			-1��			û��VESA VBE��Ϣ
//			 1��			��ȷ
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
//1����������
//			Getmodeinfo

//2�����ܣ�
//			ȡ��ǰ�Կ�VBE�ض���ʾģʽ��Ϣ������

//3��������
//		���������
//			AX=0X4F01		VBE����01H
//			CX=ģʽ��		��ʾģʽ
//			ES:DI			ָ��VBE�ض���ʾģʽ��Ϣ��ָ��
//			mode			��ʾģʽ
//		���������
//			AX=VBE			����VBE�ض���ʾģʽ��Ϣ��

//4������ֵ��
//			-1��			ȡ��ǰ�Կ�VBE�ض���ʾģʽ��Ϣ�����ݴ���
//			 1��			ȡ��ǰ�Կ�VBE�ض���ʾģʽ��Ϣ��������ȷ
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
//1����������
//			SetVBEMode

//2�����ܣ�
//			����VESA VBE��ʾģʽ

//3��������
//		���������
//			AX=0X4F02		VBE����02H
//			BX=ģʽ��		��ʾģʽ
//			mode			��ʾģʽ
//		���������
//			AX=VBE			VBE����ֵ
//			AH				���ش����

//4������ֵ��
//			-1��			��֧�ֵ�ǰ���õ���ʾģʽ
//			 1��			֧�ֵ�ǰ���õ���ʾģʽ
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
//1����������
//			InitVesa

//2�����ܣ�
//			��ʼ��VESA VBE��ʾģʽ

//3��������
//		���������
//			mode			��ʾģʽ
//		���������
//			��

//4������ֵ��
//			-1��			������ʾģʽʧ��
//			 1��			������ʾģʽ�ɹ�
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
//1����������
//			CloseVesa

//2�����ܣ�
//			����VGA�ı���ʾģʽ

//3��������
//		���������
//			AX=0X03
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
void far CloseVesa()
{
	_AX = 0x3;
   __int__(0x10);
}

/*****************************************************************************************/
//1����������
//			Setvbepage

//2�����ܣ�
//			�Կ���ҳ

//3��������
//		���������
//			AX=0X4F05		VBE����05H
//			BX=0
//			DX=page			��ҳ��ҳ��
//			page			��ҳ��ҳ��
//		���������
//			��

//4������ֵ��
//			��
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
//1����������
//			VesaPutPixel

//2�����ܣ�
//			���õ�ǰ���ص���ɫ

//3��������
//		���������
//			x				��ǰ���ص�X����
//			y				��ǰ���ص�Y����
//			color			������ɫ
//			mode			����ģʽ(COPY��NOT��OR��XOR��AND)
//		���������
//			��

//4������ֵ��
//			��
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
//1����������
//			VesaPutBMP

//2�����ܣ�
//			���õ�ǰ���ص���ɫ

//3��������
//		���������
//			x				��ǰ���ص�X����
//			y				��ǰ���ص�Y����
//			color			������ɫ
//			mode			����ģʽ(COPY��NOT��OR��XOR��AND)
//		���������
//			��

//4������ֵ��
//			��
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

	if(width%4)                    // ƴ��4�ֽڱ���
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
//1����������
//			VesaGetBMP

//2�����ܣ�
//			���õ�ǰ���ص���ɫ

//3��������
//		���������
//			x				��ǰ���ص�X����
//			y				��ǰ���ص�Y����
//			color			������ɫ
//			mode			����ģʽ(COPY��NOT��OR��XOR��AND)
//		���������
//			��

//4������ֵ��
//			��
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

	if(width%4)                    // ƴ��4�ֽڱ���
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
//1����������
//			VesaGetPixel

//2�����ܣ�
//			ȡ��ǰ���ص���ɫ

//3��������
//		���������
//			x				��ǰ���ص�X����
//			y				��ǰ���ص�Y����
//		���������
//			��

//4������ֵ��
//			��ǰ���ص���ɫ
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
//1����������
//			VesaVline

//2�����ܣ�
//			����ֱֱ��

//3��������
//		���������
//			x				��ֱֱ��X����
//			y1				��ֱֱ�����Y����
//			y2				��ֱֱ���յ�Y����
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
void far VesaVline(int x, int y1, int y2)
{
	int i;
	for (i = y1; i <= y2; i++)
	VesaPutPixel(x,i,*(unsigned long *)&DrawColor);
}

/*****************************************************************************************/
//1����������
//			VesaHline

//2�����ܣ�
//			��ˮƽֱ��

//3��������
//		���������
//			x1				ˮƽֱ�����X����
//			y				ˮƽֱ��Y����
//			x2				ˮƽֱ���յ�X����
//			color			������ɫ
//			mode			����ģʽ(COPY��NOT��OR��XOR��AND)
//		���������
//			��

//4������ֵ��
//			��
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
//1����������
//			VesaLine

//2�����ܣ�
//			������ֱ��

//3��������
//		���������
//			x1				ֱ�����X����
//			y1				ֱ�����Y����
//			x2				ֱ���յ�X����
//			y2				ֱ���յ�Y����
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
void far VesaLine(int x1,int y1,int x2,int y2)
{
	int step_x,step_y,change;
	int dt_x=x2-x1;
	int dt_y=y2-y1;
	if (dt_x<0)		//����������
	{
		dt_x=-dt_x;
		step_x=-1;
	}
	else			//�������һ���
		step_x=1;
	if (dt_y<0)		//�������ϻ���
	{
		dt_y=-dt_y;
		step_y = -1;
	}
	else			//�������»���
		step_y=1;
	if (dt_x > dt_y)    //x�ı�ñ�y��
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
	else                 //y�ı�ñ�x��
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
//1����������
//			VesaRect

//2�����ܣ�
//			������

//3��������
//		���������
//			x0				�������Ͻ�X����
//			y0				�������Ͻ�Y����
//			x1				�������½�X����
//			y1				�������½�Y����
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
void far VesaRect(int x0, int y0, int x1, int y1)
{
	VesaHline(x0, y0, x1);
	VesaHline(x0, y1, x1);
	VesaVline(x0, y0, y1);
	VesaVline(x1, y0, y1);
}

/*****************************************************************************************/
//1����������
//			VesaBar

//2�����ܣ�
//			������

//3��������
//		���������
//			x1				�������Ͻ�X����
//			y1				�������Ͻ�Y����
//			x2				�������½�X����
//			y2				�������½�Y����
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
void far VesaBar(int x1, int y1, int x2, int y2)
{
	int i;
    for (i = y1; i <= y2; ++i)
        VesaHline(x1, i, x2);
}

/*****************************************************************************************/
//1����������
//			VesaCircle

//2�����ܣ�
//			��Բ

//3��������
//		���������
//			c_x				Բ��X����
//			c_y				Բ��Y����
//			r				�뾶
//		���������
//			��

//4������ֵ��
//			��
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
//1����������
//			VesaCircleFill

//2�����ܣ�
//			���Բ

//3��������
//		���������
//			x0				Բ��X����
//			y0				Բ��Y����
//			r				�뾶
//		���������
//			��

//4������ֵ��
//			��
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
//1����������
//			VesaFillPoly

//2�����ܣ�
//			//���ƺ��������

//3��������
//		���������
//			n				�����
//			pp				��������(n*2��)��ÿһ�����Ը�������ε�һ�������X��Y����
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
//���ƺ��������
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

	// ���� pgminy �� pgmaxy
    pgminy = pp[0 * 2 + 1];
    pgmaxy = pp[0 * 2 + 1];
    for (i=0; i<n; i++)
    {
        if (pp[i * 2 + 1] < pgminy) pgminy = pp[i * 2 + 1];
        if (pp[i * 2 + 1] > pgmaxy) pgmaxy = pp[i * 2 + 1];
    }

    // �������귽��Զ���ν���ƽ�Ʊ任
    for (i=0; i<n; i++) pp[i * 2 + 1] -= pgminy;

    // ��������ʼ���±߱�
    NET = (PNODE*)malloc((pgmaxy - pgminy + 1) * sizeof(PNODE));
    if (!NET)
		goto error_handler;
    else
		memset(NET, 0, (pgmaxy - pgminy + 1) * sizeof(PNODE));

    // �Ѷ���εĸ����߷����±߱���
    for (i=0; i<n; i++)
    {
        if (pp[i * 2 + 1] == pp[((i + 1) % n) * 2 + 1])
			continue;

        // �����½��
        pnew = (NODE*)malloc(sizeof(NODE));
        if (!pnew)
			goto error_handler;

        // ���㲢����½��
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

        //�����±߱���
        ppnode = &NET[miny];
        while (*ppnode != NULL && (*ppnode)->dx > pnew->dx)
			ppnode = &((*ppnode)->next);
        pnew->next = *ppnode;
        *ppnode = pnew;
    }

	// ������������ε�ÿһ��ɨ����
    for (i=0; i<=pgmaxy-pgminy; i++)
    {
        // ɾ����߱������ý��
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

        // ���±߱��еı߽������߱���
		while (NET[i] != NULL)
        {
            // �������λ��
            ppnode = &AET;
            while (*ppnode != NULL && NET[i]->curx > (*ppnode)->curx)
				ppnode = &((*ppnode)->next);

            // ������
            ptemp  = NET[i];
            NET[i] = ptemp->next;
            ptemp->next = *ppnode;
            *ppnode = ptemp;
        }

        // ���ƶ��������ɨ����
        flag = 1;
        for (ptemp = AET; ptemp != NULL && ptemp->next != NULL; ptemp = ptemp->next)
        {
			if (flag)
				VesaHline((int)ptemp->curx, i + pgminy, (int)ptemp->next->curx);
			flag= (-1)*flag;
        }

        // ���»�߱��еĽ��
        ptemp = AET;
        while (ptemp != NULL)
        {
            ptemp->curx += ptemp->dx;
            ptemp = ptemp->next;
        }
    }

error_handler:
    // �ͷ��ڴ�
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

    // �������귽��Զ���ν���ƽ�Ʊ任
    for (i=0; i<n; i++)
		pp[i * 2 + 1] += pgminy;
}
/*****************************************************************************************/
//1����������
//			VesaSetColor

//2�����ܣ�
//			���û�ͼ��ɫ

//3��������
//		���������
//			index				��ɫRGB
//		���������
//			��

//4������ֵ��
//			��
/*****************************************************************************************/
void far VesaSetColor(unsigned long index)
{
	memcpy(&DrawColor,&index,sizeof(RGBBMP24));
//	DrawColor.r=index.r;
//	DrawColor.g=index.g;
//	DrawColor.b=index.b;
}



/*****************************************************************************************/
//1����������
//			VesaSetWriteMode

//2�����ܣ�
//			���û�ͼ�߼���־ģʽ

//3��������
//		���������
//			mode				�߼���־ģʽ
//		���������
//			��

//4������ֵ��
//		    ��
/*****************************************************************************************/
void  far VesaSetWriteMode(int mode)
{
	if((mode>=VESA_COPY)&&(mode<=VESA_NOT))
		DrawMode=mode;
}

/*****************************************************************************************/
//1����������
//			VesaGetWriteMode

//2�����ܣ�
//			ȡ����ͼ�߼���־ģʽ

//3��������
//		���������
//			��
//		���������
//			��

//4������ֵ��
//		    ��ͼ�߼���־ģʽ
/*****************************************************************************************/
int far VesaGetWriteMode()
{
	return DrawMode;
}


/*****************************************************************************************/
//1����������
//			VesaSetFillStyle

//2�����ܣ�
//			�������ģʽ

//3��������
//		���������
//			mode:			û���ã���bc31ͼ�ο�
//			color:			�����ɫ
//		���������
//			��

//4������ֵ��
//		    ��
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
	fseek(fp, 18L, SEEK_SET);       // ��ȡ�ļ�����
	fread(&Width, sizeof(Width), 1, fp);
	fread(&Height, sizeof(Height), 1, fp);
	if(Width%4)                    // ƴ��4�ֽڱ���
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
		fread(buffer, bmplinebytes, Lines, fp);// һ�ζ�ȡһ��
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
	fseek(fp, 18L, SEEK_SET);       // ��ȡ�ļ�����
	fread(&icon->width, 2, 1, fp);
	fseek(fp, 22L, SEEK_SET);       // ��ȡ�ļ�����
	fread(&icon->height, 2, 1, fp);
	if(icon->width%4)                    // ƴ��4�ֽڱ���
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
	fread(icon->buf, bmplinebytes, icon->height, fp);// һ�ζ�ȡһ��
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