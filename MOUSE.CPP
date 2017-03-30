#include"angrypig.h"

union REGS regs;
void cursor(int x,int y) /* 画光标函数 */

{
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	VesaLine(x,y,x,y+40);
	VesaLine(x,y+40,x+9,y+25);
	VesaLine(x+9,y+25,x+24,y+27);
	VesaLine(x+24,y+27,x,y);     
}

int init(int xmi,int xma,int ymi,int yma) /* 鼠标器初始化函数 */
{
	int retcode;
	regs.x.ax=0;
	int86(51,&regs,&regs);
	retcode=regs.x.ax;
	if(retcode==0)
	return 0; /* 返回0 表示鼠标或鼠标驱动程序未安装 */
	regs.x.ax=7;
	regs.x.cx=xmi;
	regs.x.dx=xma;
	int86(51,&regs,&regs);
	regs.x.ax=8;
	regs.x.cx=ymi;
	regs.x.dx=yma;
	int86(51,&regs,&regs); /* 表示鼠标器和驱动程序已安装 */
	return retcode;
}

int read(int *mx,int *my,int *mbutt) /* 读鼠标的位置和按钮状态函数 */
{
	int xx0=*mx,yy0=*my,but0=0,mb;
	int xnew, ynew;
	do 
	{
		regs.x.ax=3;
		int86(51,&regs,&regs);
		xnew=regs.x.cx;
		ynew=regs.x.dx;
		*mbutt=regs.x.bx;
	} while(xnew==xx0 && ynew==yy0 && *mbutt == but0 );
	*mx=xnew;
	*my=ynew;
	mb=(*mbutt);
	if(mb)
	{
		if(mb==1) return 1; /*左键按下*/
		if (mb==2) return 2; /*右键按下*/
		return 3; /*其它的按键情况*/
	}
	else
		return 0;
}
void newxy(int *mx,int *my,int *mbutt) /* 是否有按钮按下，若按下，在新位置画十字 */
{
	int xx0=*mx,yy0=*my,x,y;
	int xm,ym;
	read(&xm,&ym,mbutt);
        cursor(xx0,yy0);
        cursor(xm,ym);
        *mx=xm;
        *my=ym;
}
