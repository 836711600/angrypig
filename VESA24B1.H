

#ifndef _VESA_GUI_HPP_
#define _VESA_GUI_HPP_

#if __STDC__
#define _Cdecl
#else
#define _Cdecl	cdecl
#endif

#ifndef __PAS__
#define _CType _Cdecl
#else
#define _CType pascal
#endif

#ifndef NULL
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#define	NULL	0
#else
#define	NULL	0L
#endif
#endif

// 用于读取24位实际图像数据
typedef struct tagRGBBMP
{
    unsigned char   b;
    unsigned char   g;
    unsigned char   r;
} RGBBMP24;

typedef struct tagICON
{
	unsigned int width,height;
	unsigned long transparent;
	RGBBMP24 *buf;
} ICON;

//定义24位显示模式
#define  VBE640X480X16M   0X112
#define  VBE800X600X16M   0X115
#define  VBE1024X768X16M  0X118
#define  VBE1280X1024X16M 0X11B

//逻辑标志
#define VESA_COPY	0  //覆盖
#define VESA_XOR	1  //异或
#define VESA_OR		2  //或
#define VESA_AND	3  //与
#define VESA_NOT	4  //非

#ifdef __cplusplus
extern "C" {
#endif

//初始化VESA显卡显示模式
int _Cdecl far InitVesa(int mode);

//关闭显示模式，返回文本模式
void _Cdecl far CloseVesa(void);

//设置画图页面
int _Cdecl far VesaSetDrawPage(int page);

//设置显示页面
int _Cdecl far VesaSetDispPage(int page);

//设置背景页面
int _Cdecl far VesaSetBackPage(int page);

//整屏清空
void _Cdecl far VesaClearPage(long color);


//画点函数
void _Cdecl far VesaPutPixel(int x,int y,unsigned long color);

//取像素点颜色
unsigned long _Cdecl far VesaGetPixel(int x, int y);

//画任意直线
void _Cdecl far VesaLine(int x1,int y1,int x2,int y2);

//画矩形
void _Cdecl far VesaRect(int x0, int y0, int x1, int y1);

//画水平线
void _Cdecl far VesaHline(int x1, int y, int x2);

//画垂直线
void _Cdecl far VesaVline(int x, int y1, int y2);

//填充矩形
void _Cdecl far VesaBar(int x1, int y1, int x2, int y2);

//画圆
void _Cdecl far VesaCircle(int c_x, int c_y, int r);

//填充圆
void _Cdecl far VesaCircleFill(int x0, int y0, int r);

//绘制和填充多变形
void _Cdecl far VesaFillPoly(int n,int far *pp);

//设置画图颜色
void _Cdecl far VesaSetColor(unsigned long index);

//设置画图逻辑标志模式
void _Cdecl far VesaSetWriteMode(int mode);

//取画图逻辑标志模式
int _Cdecl far VesaGetWriteMode();

//设置填充模式
void _Cdecl far VesaSetFillStyle(int mode,unsigned long color);

//设置线形
void _Cdecl far VesaSetLineStyle(int reserve1,int reserve2,int reserve3);

void _Cdecl far VesaPutBMP(int x,int y, int width, int hight, RGBBMP24 *buf,long transparent);

void _Cdecl far VesaGetBMP(int x,int y, int width, int hight, RGBBMP24 *buf);

int _Cdecl far ShowBmp24B(int x,int y,char* FileName);

int _Cdecl far ShowIcon24B(int x,int y,ICON * icon);

int _Cdecl far ReadIconBmp24B(ICON *icon, char* FileName);

int _Cdecl far CloseIcon(ICON *cion);

#ifdef __cplusplus
}
#endif

#endif
