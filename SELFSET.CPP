#include"angrypig.h"
void selfsetmouse();
void selfset()
{
	RGBBMP24 color;
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,"tupian\\help.bmp");
	color.r=100;
	color.g=200;
	color.b=200;
	VesaSetColor(*(unsigned long*)&color);
	VesaBar(740,290,840,340);
	VesaBar(200,290,500,340);	
	VesaBar(800,590,900,640);
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);
	out_hz(750,300,"编辑",*(unsigned long*)&color,10,2);
	out_hz(210,300,"进入自定义游戏",*(unsigned long*)&color,10,2);
	out_hz(810,600,"返回",*(unsigned long*)&color,10,2);
	selfsetmouse();
}

void selfsetmouse()
{
	RGBBMP24 color;
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
		 if(x>=740&&x<=840&&y>=290&&y<=340&&buttons)
    		{
			delay(100);
			bianji();
    		} 

		if(x>=200&&x<=500&&y>=290&&y<=340&&buttons)
                {
                 	delay(100);    
			levelzdy();
                }  
		if(x>=800&&x<=900&&y>=590&&y<=640&&buttons)
                {
                 	delay(100);    
                        mainmenu();
                } 	         
	}
}