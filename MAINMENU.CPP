#include"angrypig.h"
void mainmouse();
void mainmenu()
{
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	for(;;)
	{
		VesaSetWriteMode(VESA_COPY);
		ShowBmp24B(0,767,".\\tupian\\mainmenu.bmp");
		VesaRect(415,283,628,337);
		VesaRect(415,370,628,429);
		VesaRect(415,463,628,520);
		VesaRect(415,558,628,609);
		VesaRect(740,425,870,465);
		VesaRect(155,420,280,460);
		mainmouse();
	}
}


void mainmouse()
{
	
	int xmin,xmax,ymin,ymax,x_max=1023,y_max=767; 
	int buttons,xm,ym,x0,y0,x,y;
	int flag;
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
	x=20;y=20;
	cursor(x,y); 
	for(;;)
	{
		newxy(&x,&y,&buttons);
 		if(x>=415&&x<=628&&y>=283&&y<=337&&buttons)
                {
                       flag=1;
			for(;;)
		       {
				switch(flag)
				{
					case 0:return;
					case 1:flag=level();
						break;
					case 2:flag=level1();
						break;
					case 3:flag=level2();
						break;
					case 4:flag=level3();
						break;
					case 5:flag=level4();
						break;							
				}
		       }
                }
                if(x>=415&&x<=628&&y>=370&&y<=429&&buttons)
                {
                       delay(100);
		       highscore();
			return;
                }
		if(x>=415&&x<=628&&y>=463&&y<=520&&buttons) 
		{
                       delay(100);
                       help();
			return;
                }
		if(x>=415&&x<=628&&y>=558&&y<=609&&buttons) 
		{
                       delay(100);
                       exit(0);
                }
		if(x>=740&&x<=870&&y>=425&&y<=465&&buttons) 
		{
                       delay(100);
                       selfset();
                }
		if(x>=155&&x<=280&&y>=420&&y<=460&&buttons) 
		{
                       delay(100);
                       vs();
                }
         } 
}
