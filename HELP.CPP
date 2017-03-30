#include"angrypig.h"
void helpmouse();
void help()
{
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,".\\tupian\\help.bmp");
	RGBBMP24 color;
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);
	out_hz(200,240,"按键：鼠标操作",*(unsigned long*)&color,10,2);
	out_hz(200,285,"操作：鼠标左键按住小猪拖动，",*(unsigned long*)&color,10,2);
	out_hz(328,330,"调节发射角度，力度，",*(unsigned long*)&color,10,2);
	out_hz(328,375,"松开鼠标发射",*(unsigned long*)&color,10,2);
	color.r=100;
	color.g=200;
	color.b=50;
	VesaSetColor(*(unsigned long*)&color);
	VesaBar(600,460,735,500);
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);
	out_hz(610,465,"主菜单",*(unsigned long*)&color,10,2);
	helpmouse();
	delay(50);
	return;	
}


void helpmouse()
{
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=768;
	int buttons,xm,ym,x0,y0,x,y;;
	xmin=1;
	xmax=x_max-1;
	ymin=1;
	ymax=y_max-2;
	VesaSetWriteMode(VESA_XOR);
	if(init(xmin,xmax,ymin,ymax)==0 ) 
	{
		printf("Mouse or Mouse Driver Absent,P1ease install!");
		delay(5000);
		exit(1);
	}
	x=100;y=40;
	cursor(x,y); 
	for(;;)
	{
		newxy(&x,&y,&buttons);
        	if(x>=600&&x<=735&&y>=460&&y<=500&&buttons)
			return;	         
	}
}
