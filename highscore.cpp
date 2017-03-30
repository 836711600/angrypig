#include"angrypig.h"
void hsmouse();
void xianshi0();
void xianshi(char *s);

void highscore()
{
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,"tupian\\paihang1.bmp");
	xianshi0();
	xianshi("file\\level1.txt");
	hsmouse();
	return;
}


void hsmouse()
{
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=768; 
	int buttons,xm,ym,x,y;
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
		if(x>=0&&x<=243&&y>=106&&y<=222&&buttons)
                {
			VesaSetWriteMode(VESA_COPY);
			ShowBmp24B(0,767,"tupian\\paihang1.bmp");
			xianshi("file\\level1.txt");
			VesaSetWriteMode(VESA_XOR);
			cursor(x,y);
                } 
        	if(x>=244&&x<=514&&y>=106&&y<=222&&buttons)
                { 
			VesaSetWriteMode(VESA_COPY);
			ShowBmp24B(0,767,"tupian\\paihang2.bmp");
			xianshi("file\\level2.txt"); 
			VesaSetWriteMode(VESA_XOR);  
			cursor(x,y);
                        
                } 
		if(x>=515&&x<=760&&y>=106&&y<=222&&buttons)
                {  
			VesaSetWriteMode(VESA_COPY);
			ShowBmp24B(0,767,"tupian\\paihang3.bmp");
			xianshi("file\\level3.txt"); 
			VesaSetWriteMode(VESA_XOR);
			cursor(x,y);
                       
                } 
		if(x>=761&&x<=1024&&y>=106&&y<=222&&buttons)
                { 
			VesaSetWriteMode(VESA_COPY); 
			ShowBmp24B(0,767,"tupian\\paihang4.bmp");
			xianshi("file\\level4.txt"); 
			VesaSetWriteMode(VESA_XOR); 
			cursor(x,y);
		} 
		if(x>=836&&x<=991&&y>=703&&y<=747&&buttons)
                {
                 	delay(100);    
                        return;
                } 
		if(x>=140&&x<=705&&y>=703&&y<=747&&buttons)
                {
                 	delay(100);    
                        filestart();
			VesaSetWriteMode(VESA_COPY);
			ShowBmp24B(0,767,"tupian\\paihang1.bmp");
			xianshi0();
			xianshi("file\\level1.txt");
			VesaSetWriteMode(VESA_XOR);
			cursor(x,y);
                }	         
	}
}


void xianshi(char *s)
{
	struct data people[10],*str;
	char a[6];
	FILE *fp;
	int j,i,y=0;
	str=people;
	RGBBMP24 color;
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);
	xianshi0();
	if((fp=fopen(s,"rb"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}

	fread(str,sizeof(struct data),10,fp);
	fclose(fp);
	for(i=0;i<5;i++)
	{	
		textxy(126,280+y,people[i].name,*(unsigned long*)&color,2);
		sprintf(a, "%d ",people[i].grade);
		textxy(330,280+y,a,*(unsigned long*)&color,2);
		y+=82;
	}
	y=0;
	for(i=5;i<10;i++)
	{	
		textxy(600,290+y,people[i].name,*(unsigned long*)&color,2);
		sprintf(a, "%d ",people[i].grade);
		textxy(810,290+y,a,*(unsigned long*)&color,2);
		y+=82;
	}    
}

void xianshi0()
{
	RGBBMP24 color;
	color.r=100;
	color.g=200;
	color.b=200;
	VesaSetColor(*(unsigned long*)&color);
	VesaBar(836,703,991,747);
	VesaBar(136,703,271,747);
	color.r=0;
	color.g=0;
	color.b=0;
	VesaSetColor(*(unsigned long*)&color);
	out_hz(840,705,"主菜单",*(unsigned long*)&color,10,2);
	out_hz(140,705,"重置",*(unsigned long*)&color,10,2);
}