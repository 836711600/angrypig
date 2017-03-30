#include"angrypig.h"
int level1mouse(BOX *box,PIG *pig1,BIRD *bird,unsigned int *score);
void checkscore1(unsigned int score);
void checkscoreun1(unsigned int score);
int fly1(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird,unsigned int *score);
int unpass1(int *x,int *y,int *buttons);
int pass1(int *x,int *y,int *buttons,unsigned int *score,int i);
int zanting1(int *x,int *y,int *buttons);
int passmouse1(int *x,int *y,int *buttons);

int level1()
{
	unsigned int score=0,i=1;
	PIG pig1={"tupian\\pig.bmp",100,217,482,0,0};
	BOX box[6]={
			{100,820,565,0,0},
			{100,820,525,0,0},
			{100,820,485,0,0},
			{100,820,445,0,0},
			{100,940,565,0,0},
			{100,940,525,0,0}
		   };
	BIRD bird[2]={
			{100,940,490,0,0},
			{100,820,410,0,0}
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
	drawbird(bird,2);
	drawbox(box,6);
	return level1mouse(box,&pig1,bird,&score);
}

int level1mouse(BOX *box,PIG *pig1,BIRD *bird,unsigned int *score)
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
					flag=fly1(&x,&y,&buttons,box,pig1,bird,score);
					if(flag!=30)
						return flag;
					VesaSetWriteMode(VESA_COPY);
					ShowBmp24B(420-70*j,60,"tupian\\cha.bmp");
					VesaSetWriteMode(VESA_XOR);
					if(ispass(bird,2))
					{
						song("music\\success.wav");
						return pass1(&x,&y,&buttons,score,j);
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
			return unpass1(&x,&y,&buttons);
		}
		else
		{
			if(x>=20&&x<=60&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		flag=zanting1(&x,&y,&buttons);
				if(flag!=30)
					return flag;
                	}  
			if(x>=80&&x<=120&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		return 2;
                	}  
			newxy(&x,&y,&buttons);  
		}
	}	         
}


int fly1(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird,unsigned int *score)
{
	ICON pigbmp,birdbmp,boxbmp;
	int flag;
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
                	delay(100);    
                      	flag=zanting1(mx,my,buttons);
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
                       	return 2;
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
	return 30;	
}



int unpass1(int *x,int *y,int *buttons)
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
			return 2;
		}
	}
}

int zanting1(int *x,int *y,int *buttons)
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
			return 2;
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

int pass1(int *x,int *y,int *buttons,unsigned int *score,int i)
{
	FILE *fp;
	char s[15],str[6];
	int lock[4],*lock1;
	lock1=lock;
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if((fp=fopen(".\\file\\lock.txt","rt+"))==NULL)
	{
		printf("不能打开LOCK.TXT！！");
		getch();
		exit(0);
	}
	fread(lock1,sizeof(int),4,fp);
	lock[1]=1;
	rewind(fp);
	fwrite(lock1,sizeof(int),4,fp);
	fclose(fp);
	VesaSetWriteMode(VESA_XOR);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	*score=*score+3000*(3-i);
	sprintf(str,"%d",*score);
	textxy(700,20,str,*(unsigned long*)&color,1);
	if(compare(*score,"file\\level1.txt"))
	{
		VesaSetWriteMode(VESA_XOR);
		cursor(*x,*y);
		ShowBmp24B(300,450,"tupian\\gaofen.bmp");
		putin(370,370,s);
		paixu(s,*score,"file\\level1.txt");
		VesaSetWriteMode(VESA_COPY);
		checkscore1(*score);
		VesaSetWriteMode(VESA_XOR);
		return passmouse1(x,y,buttons);
	}
	else
	{
		cursor(*x,*y);
		VesaSetWriteMode(VESA_COPY);
		checkscoreun1(*score);
		VesaSetWriteMode(VESA_XOR);
		return passmouse1(x,y,buttons);
	}
}

int passmouse1(int *x,int *y,int *buttons)
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
			return 2;
		}
		if(*x>=610&&*x<=695&&*y>=425&&*y<=500&&*buttons)
		{
			delay(100);
			return 3;
		}
	}
}

void checkscore1(unsigned int score)
{
	char str[6];
	RGBBMP24 color;	
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if(score>12000)
	{
		ShowBmp24B(300,500,"tupian\\pass3.bmp");
	}
	else if(score>10000)
	{
		ShowBmp24B(300,500,"tupian\\pass2.bmp");
	}
	else if(score>8000)
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


void checkscoreun1(unsigned int score)
{
	char str[6];
	RGBBMP24 color;	
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if(score>12000)
	{
		ShowBmp24B(300,500,"tupian\\passun3.bmp");
	}
	else if(score>10000)
	{
		ShowBmp24B(300,500,"tupian\\passun2.bmp");
	}
	else if(score>8000)
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


