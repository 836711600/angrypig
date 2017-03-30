#include"angrypig.h"
void levelzdymouse(BOX *box,PIG *pig1,BIRD *bird,unsigned int *score);
void checkscorezdy(unsigned int score);
void checkscoreunzdy(unsigned int score);
void flyzdy(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird,unsigned int *score);
void unpasszdy(int *x,int *y,int *buttons);
void passzdy(int *x,int *y,int *buttons,unsigned int *score,int i);
void zantingzdy(int *x,int *y,int *buttons);
void passmousezdy(int *x,int *y,int *buttons);

void levelzdy()
{
	FILE *fp;
	unsigned int score=0;
	int i=1;
	PIG pig1={"tupian\\pig.bmp",100,217,482,0,0};
	BOX box[6];
	BIRD bird[2];

	if((fp=fopen("file\\zdybox.txt","rb"))==NULL)
	{
		printf("zdybox.dat unopen!!");
		getch();
		exit(0);
	}  
	fread(box,sizeof(BOX),6,fp);
	fclose(fp);

	if((fp=fopen("file\\zdybird.txt","rb"))==NULL)
	{
		printf("zdybird.dat unopen!!");
		getch();
		exit(0);
	}   
	fread(bird,sizeof(BIRD),2,fp);
	fclose(fp);

	RGBBMP24 color;
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,"tupian\\level1.bmp");
	ShowBmp24B(20,60,"tupian\\zanting.bmp");
	ShowBmp24B(80,60,"tupian\\again.bmp");
	for(;i<5;i++)
	{
		ShowBmp24B(140+70*i,60,"tupian\\cishu.bmp");
	}
	VesaSetWriteMode(VESA_XOR);
	drawpig(&pig1);
	drawbird(bird,2);
	drawbox(box,6);
	levelzdymouse(box,&pig1,bird,&score);
}

void levelzdymouse(BOX *box,PIG *pig1,BIRD *bird,unsigned int *score)
{
	RGBBMP24 color;
	char str[6];
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=768; 
	int buttons=0,xm,ym,x0,y0,i=0,j=0,x,y;
	xmin=1;
	xmax=x_max-1;
	ymin=1;
	ymax=y_max-2;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	VesaSetWriteMode(VESA_XOR);
	if(init(xmin,xmax,ymin,ymax)==0) 
	{
		printf("Mouse or Mouse Driver Absent,P1ease install!");
		delay(5000);
		exit(1);
	}
	x=512;
	y=384;
	cursor(x,y);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	for(;j<4;)
	{	
		if(abs(x-217)<85&&abs(y-482)<85&&buttons)
		{
			if(i++==0)
			{
				cursor(x,y);
				x=217;
				y=482;			
			}
			song("music\\launch.wav");
			for(;;)
			{
				launch(&x,&y,&buttons,pig1);
				if(buttons==0)
				{
					pig1->x=x;
					pig1->y=y;
					pig1->life=100;
					flyzdy(&x,&y,&buttons,box,pig1,bird,score);
					VesaSetWriteMode(VESA_COPY);
					ShowBmp24B(420-70*j,60,"tupian\\cha.bmp");
					VesaSetWriteMode(VESA_XOR);
					if(ispass(bird,2))
					{
						passzdy(&x,&y,&buttons,score,j);
					}
					j++;
					i=0;
					break;
				}
			}
		}
		if(j==4)
		{
			unpasszdy(&x,&y,&buttons);
		}
		else
		{
			if(x>=20&&x<=60&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		zantingzdy(&x,&y,&buttons);
                	}  
			if(x>=80&&x<=120&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		levelzdy();
                	}  
			newxy(&x,&y,&buttons);  
		}
	}	         
}


void flyzdy(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird,unsigned int *score)
{
	ICON pigbmp,birdbmp,boxbmp;
	pig1->vx=210-pig1->x+30;
        pig1->vy=pig1->y-500;
	cursor(pig1->x,pig1->y);
	ReadIconBmp24B(&pigbmp,pig1->bmp);
	ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
	ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
	for(;stopcheckbox(box,6)||pig1->vx!=0||pig1->vy!=0||stopcheckbird(bird,2);)
	{
		if(*mx>=20&&*mx<=60&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
			zantingzdy(mx,my,buttons);
                }  
		if(*mx>=80&&*mx<=120&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
                       	levelzdy();
                }  
		newxy(mx,my,buttons); 
		drawmovepig(pig1,&pigbmp);
		drawmovebox(box,6,&boxbmp);
		drawmovebird(bird,2,&birdbmp);
		hitcheckpig(box,pig1,bird,6,2,score);
		hitcheckbox(bird,box,6,2,score);
		checkunder(bird,box,6,2);
		stopbox(box,6);
		stopbird(bird,2);
		delay(25);
	}
	ShowIcon24B(202,497,&pigbmp);
	CloseIcon(&pigbmp);
	CloseIcon(&birdbmp);
	CloseIcon(&boxbmp);	
}


void zantingzdy(int *x,int *y,int *buttons)
{
	ShowBmp24B(20,60,"tupian\\zanting.bmp");
	ShowBmp24B(80,60,"tupian\\again.bmp");
	ShowBmp24B(0,550,"tupian\\zanting1.bmp");
	for(;;)
	{
		newxy(x,y,buttons);
		if(*x>=45&&*x<=130&&*y>=330&&*y<=425&&*buttons)
		{
			delay(100);
			mainmenu();
		}
		if(*x>=45&&*x<=130&&*y>=210&&*y<=310&&*buttons)
		{
			delay(100);
			selfset();
		}
		if(*x>=45&&*x<=130&&*y>=100&&*y<=200&&*buttons)
		{
			delay(100);
			levelzdy();
		}
		if(*x>=140&&*x<=200&&*y>=240&&*y<=310&&*buttons)
		{
			ShowBmp24B(0,550,"tupian\\zanting1.bmp");
			ShowBmp24B(20,60,"tupian\\zanting.bmp");
			ShowBmp24B(80,60,"tupian\\again.bmp");
			delay(100);
			return;
		}
	}
}
void unpasszdy(int *x,int *y,int *buttons)
{

	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(300,500,"tupian\\zdyfail.bmp");
	VesaSetWriteMode(VESA_XOR);
	for(;;)
	{
		newxy(x,y,buttons);
		if(*x>=370&&*x<=500&&*y>=440&&*y<=470&&*buttons)
		{
			delay(100);
			levelzdy();
		}
		if(*x>=577&&*x<=710&&*y>=440&&*y<=470&&*buttons)
		{
			delay(100);
			selfset();
		}
	}
}


void passzdy(int *x,int *y,int *buttons,unsigned int *score,int i)
{
	RGBBMP24 color;
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);

	char str[6];
	VesaSetWriteMode(VESA_XOR);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	*score=*score+3000*(3-i);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);

	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(300,450,"tupian\\zdytg.bmp");
	sprintf(str,"%d",*score);
	textxy(407,327,str,*(unsigned long*)&color,1);
	VesaSetWriteMode(VESA_XOR);
	for(;;)
	{
		passmousezdy(x,y,buttons);
	}
}

void passmousezdy(int *x,int *y,int *buttons)
{
	newxy(x,y,buttons);
	if(*x>=368&&*x<=495&&*y>=398&&*y<=422&&*buttons)
	{
		delay(100);
		levelzdy();
	}
	if(*x>=537&&*x<=706&&*y>=396&&*y<=421&&*buttons)
	{
		delay(100);
		selfset();
	}
}





