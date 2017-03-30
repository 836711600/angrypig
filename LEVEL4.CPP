#include"angrypig.h"
int level4mouse(BOX *box,PIG *pig1,BIRD *bird,unsigned int *score);
void checkscore4(unsigned int score);
void checkscoreun4(unsigned int score);
int fly4(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird,unsigned int *score);
int unpass4(int *x,int *y,int *buttons);
int pass4(int *x,int *y,int *buttons,unsigned int *score,int i);
int zanting4(int *x,int *y,int *buttons);
int passmouse4(int *x,int *y,int *buttons);
void baozha(int x,int y);
void baozhacheck(PIG *pig1,BOX *box,BIRD *bird,int i,int j,unsigned int *score);
void stoppig4(PIG *pig1,BOX *box,BIRD *bird,unsigned int *score);
void hitcheckpig4(BOX *box,PIG *pig1,BIRD *bird,int i,int j,unsigned int *score);

int level4()
{
	unsigned int score=0,i=1;
	PIG pig1={"tupian\\pig44.bmp",100,217,482,0,0};
	BOX box[9]={
			{100,700,565,0,0},
			{100,700,525,0,0},
			{100,700,485,0,0},
			{100,820,565,0,0},
			{100,820,525,0,0},
			{100,820,485,0,0},
			{100,940,565,0,0},
			{100,940,525,0,0},
			{100,940,485,0,0}
		   };
	BIRD bird[3]={
			{100,750,570,0,0},
			{100,870,570,0,0},
			{100,990,570,0,0}
		     };
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
	drawbird(bird,3);
	drawbox(box,9);
	if(compare(1,"file\\level4.txt"))
	{
		ShowBmp24B(300,400,"tupian\\baozha.bmp");
		delay(1500);
		ShowBmp24B(300,400,"tupian\\baozha.bmp");	
	}
	return level4mouse(box,&pig1,bird,&score);
}

int level4mouse(BOX *box,PIG *pig1,BIRD *bird,unsigned int *score)
{
	RGBBMP24 color;
	char str[6];
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=768; 
	int buttons=0,xm,ym,x0,y0,i=0,j=0,x,y;
	int flag;
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
					flag=fly4(&x,&y,&buttons,box,pig1,bird,score);
					if(flag!=30)
						return flag;
					VesaSetWriteMode(VESA_COPY);
					ShowBmp24B(420-70*j,60,"tupian\\cha.bmp");
					VesaSetWriteMode(VESA_XOR);
					if(ispass(bird,3))
					{
						song("music\\success.wav");
						return pass4(&x,&y,&buttons,score,j);
					}
					j++;
					i=0;
					break;
				}
			}
		}
		if(j==4)
		{
			song("music\\fail.wav");
			return unpass4(&x,&y,&buttons);
		}
		else
		{
			if(x>=20&&x<=60&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		flag=zanting4(&x,&y,&buttons);
				if(flag!=30)
					return flag;
                	}  
			if(x>=80&&x<=120&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		return 5;
                	}  
			newxy(&x,&y,&buttons);  
		}
	}	         
}


int fly4(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird,unsigned int *score)
{
	ICON pigbmp,birdbmp,boxbmp;
	int i=0,flag;
	pig1->vx=210-pig1->x+30;
        pig1->vy=pig1->y-500;
	cursor(pig1->x,pig1->y);
	ReadIconBmp24B(&pigbmp,pig1->bmp);
	ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
	ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
	for(;stopcheckbox(box,9)||pig1->vx!=0||pig1->vy!=0||stopcheckbird(bird,3);)
	{ 
		newxy(mx,my,buttons); 
		if(*mx>=20&&*mx<=60&&*my>=20&&*my<=60&&*buttons)
              	{
                	delay(100);    
                      	flag=zanting4(mx,my,buttons);
			if(flag!=30)
			{
				CloseIcon(&pigbmp);
				CloseIcon(&birdbmp);
				CloseIcon(&boxbmp);
				return flag;
			}
                }  
		if(*mx>=80&&*mx<=120&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
                       	return 5;
                } 
		drawmovepig(pig1,&pigbmp);
		drawmovebox(box,9,&boxbmp);
		drawmovebird(bird,3,&birdbmp);
		if(*buttons!=0&&i==0)
		{
			drawpig(pig1);
			pig1->vx=0;
			pig1->vy=0;
			pig1->life=0;
			baozha(pig1->x,pig1->y);
			baozhacheck(pig1,box,bird,9,3,score);
			i=1;
		}
		hitcheckpig4(box,pig1,bird,9,3,score);
		hitcheckbox(bird,box,9,3,score);
		checkunder(bird,box,9,3);
		stopbox(box,9);
		stopbird(bird,3);
		delay(25);
	}
	ShowIcon24B(202,497,&pigbmp);
	CloseIcon(&pigbmp);
	CloseIcon(&birdbmp);
	CloseIcon(&boxbmp);
	return 30;	
}



int unpass4(int *x,int *y,int *buttons)
{
	ShowBmp24B(300,500,"tupian\\unpass.bmp");
	for(;;)
	{
		newxy(x,y,buttons);
		if(*x>=350&&*x<=440&&*y>=410&&*y<=500&&*buttons)
		{
			delay(100);
			return 1;
		}
		if(*x>=480&&*x<=570&&*y>=410&&*y<=500&&*buttons)
		{
			delay(100);
			return 5;
		}
	}
}

int zanting4(int *x,int *y,int *buttons)
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
			return 0;
		}
		if(*x>=45&&*x<=130&&*y>=210&&*y<=310&&*buttons)
		{
			delay(100);
			return 1;
		}
		if(*x>=45&&*x<=130&&*y>=100&&*y<=200&&*buttons)
		{
			delay(100);
			return 5;
		}
		if(*x>=140&&*x<=200&&*y>=240&&*y<=310&&*buttons)
		{
			ShowBmp24B(0,550,"tupian\\zanting1.bmp");
			ShowBmp24B(20,60,"tupian\\zanting.bmp");
			ShowBmp24B(80,60,"tupian\\again.bmp");
			delay(100);
			return 30;
		}
	}
}

int pass4(int *x,int *y,int *buttons,unsigned int *score,int i)
{
	char str[6],s[15];
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	VesaSetWriteMode(VESA_XOR);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	*score=*score+3000*(3-i);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	if(compare(*score,"file\\level4.txt"))
	{
		VesaSetWriteMode(VESA_XOR);
		cursor(*x,*y);
		ShowBmp24B(300,450,"tupian\\gaofen.bmp");
		putin(370,370,s);
		paixu(s,*score,"file\\level4.txt");
		VesaSetWriteMode(VESA_COPY);
		checkscore4(*score);
		VesaSetWriteMode(VESA_XOR);
		return passmouse4(x,y,buttons);
	}
	else
	{
		cursor(*x,*y);
		VesaSetWriteMode(VESA_COPY);
		checkscoreun4(*score);
		VesaSetWriteMode(VESA_XOR);
		return passmouse4(x,y,buttons);
	}
}

int passmouse4(int *x,int *y,int *buttons)
{
	cursor(*x,*y);
	for(;;)
	{
		newxy(x,y,buttons);
		if(*x>=355&&*x<=440&&*y>=425&&*y<=500&&*buttons)
		{
			delay(100);
			return 1;
		}
		if(*x>=485&&*x<=570&&*y>=425&&*y<=500&&*buttons)
		{
			delay(100);
			return 5;
		}
		if(*x>=610&&*x<=695&&*y>=425&&*y<=500&&*buttons)
		{
			delay(100);
			return 2;
		}
	}
}

void checkscore4(unsigned int score)
{
	char str[6];
	RGBBMP24 color;	
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if(score>15000)
	{
		ShowBmp24B(300,500,"tupian\\pass3.bmp");
	}
	else if(score>13000)
	{
		ShowBmp24B(300,500,"tupian\\pass2.bmp");
	}
	else if(score>11000)
	{
		ShowBmp24B(300,500,"tupian\\pass1.bmp");
	}
	else
	{
		ShowBmp24B(300,500,"tupian\\pass0.bmp");
	}
	sprintf(str,"%d",score);
	textxy(400,350,str,*(unsigned long*)&color,1);	
}


void checkscoreun4(unsigned int score)
{
	char str[6];
	RGBBMP24 color;	
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if(score>15000)
	{
		ShowBmp24B(300,500,"tupian\\passun3.bmp");
	}
	else if(score>13000)
	{
		ShowBmp24B(300,500,"tupian\\passun2.bmp");
	}
	else if(score>11000)
	{
		ShowBmp24B(300,500,"tupian\\passun1.bmp");
	}
	else
	{
		ShowBmp24B(300,500,"tupian\\passun0.bmp");
	}	
	sprintf(str,"%d",score);
	textxy(400,350,str,*(unsigned long*)&color,1);
}

void baozha(int x,int y)
{
	VesaSetWriteMode(VESA_XOR);
	ShowBmp24B(x-25,y+25,"tupian\\baozha1.bmp");
	delay(30);	
	ShowBmp24B(x-25,y+25,"tupian\\baozha1.bmp");
	ShowBmp24B(x-25,y+25,"tupian\\baozha2.bmp");
	delay(30);	
	ShowBmp24B(x-25,y+25,"tupian\\baozha2.bmp");
	ShowBmp24B(x-25,y+25,"tupian\\baozha3.bmp");
	delay(30);	
	ShowBmp24B(x-25,y+25,"tupian\\baozha3.bmp");
	ShowBmp24B(x-25,y+25,"tupian\\baozha4.bmp");
	delay(30);	
	ShowBmp24B(x-25,y+25,"tupian\\baozha4.bmp");
}

void baozhacheck(PIG *pig1,BOX *box,BIRD *bird,int i,int j,unsigned int *score)
{	
	for(;i>0;i--,box++)
	{
		if(box->life>0)
		{
			if(abs(pig1->x-box->x)<80&&abs(pig1->y-box->y)<80)
			{
				box->life=0;
				checkboxbz(box,score);
			}
		}
	}
	for(;j>0;j--,bird++)
	{
		if(bird->life>0)
		{
			if(abs(pig1->x-bird->x)<80&&abs(pig1->y-bird->y)<80)
			{
				bird->life=0;
				checkbirdbz(bird,score);
			}
		}
	}
}


void hitcheckpig4(BOX *box,PIG *pig1,BIRD *bird,int i,int j,unsigned int *score)
{
	BOX *boxtemp=box;
	BIRD *birdtemp=bird;
	if(pig1->life>0)
	{
		for(;i>0;i--,box++)
		{
			if(box->life>0)
			{
				if(abs(box->x-pig1->x)<36&&abs(box->y-pig1->y)<36)
				{
					if(box->y-pig1->y>30)
					{
						if(fabs(pig1->vy)<40)
						{
							pig1->vy=pig1->vy*(-0.5);
							box->life=box->life-30;
							checkboxbz(box,score);
						}
						else
						{
							pig1->vx=pig1->vx*0.5;
							pig1->vy=pig1->vy*0.5;
							box->life=0;
							checkboxbz(box,score);
						}
					}
					else
					{
						if(fabs(pig1->vx)<35)
						{
							pig1->vx=pig1->vx*(-0.5);
							box->life=box->life-50;
							checkboxbz(box,score);
						}
						else if(fabs(pig1->vx)<90)
						{
							box->vx=pig1->vx*(0.5);
							pig1->vx=pig1->vx*(-0.5);
							box->life=box->life-60;
							checkboxbz(box,score);
						}
						else
						{
							pig1->vx=pig1->vx*0.6;
							pig1->vy=pig1->vy*1.5;
							box->life=0;
							checkboxbz(box,score);
						}
					}
					break;
				}
			}
		}
		for(;j>0;j--,bird++)
		{
			if(bird->life>0)
			{
				if(abs(bird->x-pig1->x)<30&&abs(bird->y-pig1->y)<30)
				{
					pig1->vy=pig1->vy*(-0.7);
					pig1->vx=pig1->vx*0.5;
					bird->life=0;
					checkbirdbz(bird,score);
				}
			}
		}
		pig1->vy=pig1->vy-7*0.1;
		stoppig4(pig1,boxtemp,birdtemp,score);
	}
}
void stoppig4(PIG *pig1,BOX *box,BIRD *bird,unsigned int *score)
{
	ICON pigbmp;
	if(pig1->y>575)
	{
			pig1->vy=pig1->vy*(-0.5);
			pig1->vx=pig1->vx*0.5;
	}
	if(fabs(pig1->vx)<15)
	{
		pig1->vx=0;
		pig1->vy=0;
		pig1->life=0;
		ReadIconBmp24B(&pigbmp,pig1->bmp);
		ShowIcon24B(pig1->x-15,pig1->y+15,&pigbmp);
		CloseIcon(&pigbmp);
		baozha(pig1->x,pig1->y);
		baozhacheck(pig1,box,bird,9,3,score);
	}
	if(pig1->x<20||pig1->x>1039)
	{
		pig1->vx=0;
		pig1->vy=0;
		pig1->life=0;
		ReadIconBmp24B(&pigbmp,pig1->bmp);
		ShowIcon24B(pig1->x-15,pig1->y+15,&pigbmp);
		CloseIcon(&pigbmp);
	}
}
