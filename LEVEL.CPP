#include"angrypig.h"
int levelmouse(int a1,int a2,int a3,int a4);
int level()
{
	FILE *fp;
	RGBBMP24 color;
	unsigned int highscore[4];
	int lock[4],*lock1;
	lock1=lock;
	struct data a,*score;
	score=&a;
	char s[10];
	int k;
	char *grade1[4]={".\\file\\level1.txt",".\\file\\level2.txt",".\\file\\level3.txt",".\\file\\level4.txt"};
	for(k=0;k<4;k++)
	{
		if((fp=fopen(grade1[k],"rb"))==NULL)
		{
			printf("不能打开%s!!",grade1[k]);
			getch();
			exit(0);
		}
		fread(score,sizeof(struct data),1,fp);
		highscore[k]=score->grade;
		fclose(fp);
	}		
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,".\\tupian\\level.bmp");
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	out_hz(440,10,"关卡选择",*(unsigned long*)&color,10,2);

	//VesaRect(30,60,482,330);
	out_hz(170,70,"第一关",*(unsigned long*)&color,10,2);
	out_hz(40,180,"最高分：",*(unsigned long*)&color,10,2);


	//VesaRect(542,60,994,330);
	out_hz(670,70,"第二关",*(unsigned long*)&color,10,2);
	out_hz(552,180,"最高分：",*(unsigned long*)&color,10,2);

	//VesaRect(30,370,482,640);
	out_hz(170,380,"第三关",*(unsigned long*)&color,10,2);
	out_hz(40,500,"最高分：",*(unsigned long*)&color,10,2);

	//VesaRect(542,370,994,640);
	out_hz(670,380,"第四关",*(unsigned long*)&color,10,2);
	out_hz(552,500,"最高分：",*(unsigned long*)&color,10,2);

	sprintf(s,"%d",highscore[0]);
	textxy(300,190,s,*(unsigned long*)&color,1);
	sprintf(s,"%d",highscore[1]);
	textxy(800,190,s,*(unsigned long*)&color,1);
	sprintf(s,"%d",highscore[2]);
	textxy(300,500,s,*(unsigned long*)&color,1);
	sprintf(s,"%d",highscore[3]);
	textxy(800,500,s,*(unsigned long*)&color,1);
	
	color.r=100;
	color.g=200;
	color.b=200;
	VesaSetColor(*(unsigned long*)&color);
	VesaBar(800,690,900,740);
	VesaBar(200,690,300,740);
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);
	out_hz(810,700,"返回",*(unsigned long*)&color,10,2);
	out_hz(210,700,"重置",*(unsigned long*)&color,10,2);
	if((fp=fopen(".\\file\\lock.txt","rt"))==NULL)
	{
		printf("不能打开LOCK.TXT！！");
		getch();
		exit(0);
	}
	fread(lock1,sizeof(int),4,fp);
	fclose(fp);
	if(lock[0]==0)
		ShowBmp24B(30,330,".\\tupian\\lock.bmp");
	if(lock[1]==0)
		ShowBmp24B(542,330,".\\tupian\\lock.bmp");
	if(lock[2]==0)
		ShowBmp24B(30,640,".\\tupian\\lock.bmp");
	if(lock[3]==0)
		ShowBmp24B(542,640,".\\tupian\\lock.bmp");
		
	return levelmouse(lock[0],lock[1],lock[2],lock[3]);
}


int levelmouse(int a1,int a2,int a3,int a4)
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
   		 if(x>=30&&x<=482&&y>=60&&y<=330&&buttons&&a1)
    		{
			delay(100);
			return 2;
    		} 
		if(x>=542&&x<=994&&y>=60&&y<=330&&buttons&&a2)
                {
                 	delay(100);    
                        return 3;
                } 
		if(x>=30&&x<=482&&y>=370&&y<=640&&buttons&&a3)
                {
                 	delay(100);    
                        return 4;
                } 
		if(x>=552&&x<=994&&y>=370&&y<=640&&buttons&&a4)
                {
                 	delay(100);    
                        return 5;
                } 
		if(x>=800&&x<=900&&y>=680&&y<=730&&buttons)
                {
                 	delay(100);    
                        return 0;
                } 
		if(x>=200&&x<=300&&y>=680&&y<=730&&buttons)
                {
                 	delay(100);    
                        lockstart();
			return 1;
                }  	         
	}
}