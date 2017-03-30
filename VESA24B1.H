

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

// ���ڶ�ȡ24λʵ��ͼ������
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

//����24λ��ʾģʽ
#define  VBE640X480X16M   0X112
#define  VBE800X600X16M   0X115
#define  VBE1024X768X16M  0X118
#define  VBE1280X1024X16M 0X11B

//�߼���־
#define VESA_COPY	0  //����
#define VESA_XOR	1  //���
#define VESA_OR		2  //��
#define VESA_AND	3  //��
#define VESA_NOT	4  //��

#ifdef __cplusplus
extern "C" {
#endif

//��ʼ��VESA�Կ���ʾģʽ
int _Cdecl far InitVesa(int mode);

//�ر���ʾģʽ�������ı�ģʽ
void _Cdecl far CloseVesa(void);

//���û�ͼҳ��
int _Cdecl far VesaSetDrawPage(int page);

//������ʾҳ��
int _Cdecl far VesaSetDispPage(int page);

//���ñ���ҳ��
int _Cdecl far VesaSetBackPage(int page);

//�������
void _Cdecl far VesaClearPage(long color);


//���㺯��
void _Cdecl far VesaPutPixel(int x,int y,unsigned long color);

//ȡ���ص���ɫ
unsigned long _Cdecl far VesaGetPixel(int x, int y);

//������ֱ��
void _Cdecl far VesaLine(int x1,int y1,int x2,int y2);

//������
void _Cdecl far VesaRect(int x0, int y0, int x1, int y1);

//��ˮƽ��
void _Cdecl far VesaHline(int x1, int y, int x2);

//����ֱ��
void _Cdecl far VesaVline(int x, int y1, int y2);

//������
void _Cdecl far VesaBar(int x1, int y1, int x2, int y2);

//��Բ
void _Cdecl far VesaCircle(int c_x, int c_y, int r);

//���Բ
void _Cdecl far VesaCircleFill(int x0, int y0, int r);

//���ƺ��������
void _Cdecl far VesaFillPoly(int n,int far *pp);

//���û�ͼ��ɫ
void _Cdecl far VesaSetColor(unsigned long index);

//���û�ͼ�߼���־ģʽ
void _Cdecl far VesaSetWriteMode(int mode);

//ȡ��ͼ�߼���־ģʽ
int _Cdecl far VesaGetWriteMode();

//�������ģʽ
void _Cdecl far VesaSetFillStyle(int mode,unsigned long color);

//��������
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