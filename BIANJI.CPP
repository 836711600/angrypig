#include"angrypig.h"
void yidongbird(int *mx,int *my,int *mbutt);
void yidongbox(int *mx,int *my,int *mbutt);
void bianjimouse(BOX *box,BIRD *bird);

void bianji()
{
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
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,"tupian\\bianji.bmp");
	ShowBmp24B(20,60,"tupian\\zanting.bmp");
	ShowBmp24B(80,60,"tupian\\again.bmp");
	VesaSetWriteMode(VESA_XOR);
	drawbird(bird,2);
	drawbox(box,6);
	bianjimouse(box,bird);
}


void bianjimouse(BOX *box,BIRD *bird)
{
	FILE *fp;
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=575;
	
	int buttons=0,xm,ym,x0,y0,i=0,j,k,x,y;
	BOX *box1;
	BIRD *bird1;

	box1=box;
	bird1=bird;

	xmin=1;
	xmax=x_max-1;
	ymin=1;
	ymax=y_max-2;

	if(init(xmin,xmax,ymin,ymax)==0)
	{
		printf("Mouse or Mouse Driver Absent,P1ease install!");
		delay(5000);
		exit(1);
	}
	x=512;
	y=384;
	cursor(x,y);
	for(;;)
	{	
		box1=box;
		bird1=bird;
		for(j=0;j<6;j++)
		{	
			if(abs(x-box1->x)<20&&abs(y-box1->y)<20&&buttons)
			{
				if(i++==0)
				{
					cursor(x,y);
					x=box1->x;
					y=box1->y;			
				}
				for(;;)
				{
					yidongbox(&x,&y,&buttons);
					if(buttons==0)
					{
						box1->x=x;
						box1->y=y;
						box1->life=100;
						box1->vx=0;
						box1->vy=0;
						cursor(box1->x,box1->y);
						i=0;
						break;
					}
				}
					
			}
			box1++;
		}

		for(k=0;k<2;k++)
		{	
			if(abs(x-bird1->x)<15&&abs(y-bird1->y)<15&&buttons)
			{
				if(i++==0)
				{
					cursor(x,y);
					x=bird1->x;
					y=bird1->y;			
				}
				for(;;)
				{
					yidongbird(&x,&y,&buttons);
					if(buttons==0)
					{
						bird1->x=x;
						bird1->y=y;
						bird1->life=100;
						bird1->vx=0;
						bird1->vy=0;
						cursor(bird1->x,bird1->y);
						i=0;
						break;
					}
				}
			}
			bird1++;
		}

		if(x>=20&&x<=60&&y>=20&&y<=60&&buttons)
              	{ 
			box1=box;
			bird1=bird;
			delay(50); 
			if((fp=fopen("file\\zdybox.txt","wb+"))==NULL)
			{
				printf("zdybox.txt unopen!!");
				getch();
				exit(0);
			}  
			for(i=0;i<6;i++) 
			{
				fwrite(box1,sizeof(BOX),1,fp);
				box1++;
			}
			fclose(fp);

			if((fp=fopen("file\\zdybird.txt","wb+"))==NULL)
			{
				printf("zdybird.txt unopen!!");
				getch();
				exit(0);
			}   
			fwrite(bird1,sizeof(BIRD),2,fp);
			fclose(fp);
			selfset();
                }  
		if(x>=80&&x<=120&&y>=20&&y<=60&&buttons)
              	{
                	delay(100);    
                       	bianji();
                }  
		newxy(&x,&y,&buttons);  
	}	         
}


void yidongbird(int *mx,int *my,int *mbutt)
{
	ICON birdbmp;
	int xx0=*mx,yy0=*my;
	int xm,ym;
	read(&xm,&ym,mbutt);
	ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
        ShowIcon24B(xx0-15,yy0+15,&birdbmp);
        ShowIcon24B(xm-15,ym+15,&birdbmp);
	CloseIcon(&birdbmp);
        *mx=xm;
        *my=ym;
}

void yidongbox(int *mx,int *my,int *mbutt)
{
	ICON boxbmp;
	int xx0=*mx,yy0=*my;
	int xm,ym;
	read(&xm,&ym,mbutt);
	ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
        ShowIcon24B(xx0-20,yy0+20,&boxbmp);
        ShowIcon24B(xm-20,ym+20,&boxbmp);
	CloseIcon(&boxbmp);
        *mx=xm;
        *my=ym;
}