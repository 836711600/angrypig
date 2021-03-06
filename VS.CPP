#include"angrypig.h"
void vsmouse1(BOX *box1,PIG *pig1,BIRD *bird1,BOX *box2,PIG *pig2,BIRD *bird2);
void vsmouse2(BOX *box1,PIG *pig1,BIRD *bird1,BOX *box2,PIG *pig2,BIRD *bird2);
void vsfly1(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird);
void vsfly2(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird);
void vszanting(int *x,int *y,int *buttons);
void vshitcheckpig(BOX *box,PIG *pig1,BIRD *bird,int i,int j);
void vshitcheckbox(BIRD *bird,BOX *box,int i,int n);
void vscheckboxbz(BOX *box);
void vscheckbirdbz(BIRD *bird);
void vslaunch1(int *mx,int *my,int *mbutt,PIG *pig1);
void vslaunch2(int *mx,int *my,int *mbutt,PIG *pig1);
void vswinmouse();
int iswin(BIRD *bird,int i);

void vs()
{
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	PIG pig1={"tupian\\pig.bmp",100,217,482,0,0};
	BOX box1[6]={
			{100,900,565,0,0},
			{100,940,565,0,0},
			{100,940,525,0,0},
			{100,980,565,0,0},
			{100,980,525,0,0},
			{100,980,485,0,0}
		   };
	BIRD bird1[3]={
			{100,900,530,0,0},
			{100,940,490,0,0},
			{100,980,450,0,0}
		     };
	PIG pig2={"tupian\\pig.bmp",100,800,482,0,0};
	BOX box2[6]={
			{100,120,565,0,0},
			{100,80,565,0,0},
			{100,80,525,0,0},
			{100,40,565,0,0},
			{100,40,525,0,0},
			{100,40,485,0,0}
		   };
	BIRD bird2[3]={
			{100,120,530,0,0},
			{100,80,490,0,0},
			{100,40,450,0,0}
		     };
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,"tupian\\vs.bmp");
	ShowBmp24B(20,60,"tupian\\zanting.bmp");
	ShowBmp24B(80,60,"tupian\\again.bmp");
	out_hz(20,70,"���һ",*(unsigned long*)&color,10,2);
	out_hz(900,70,"��Ҷ�",*(unsigned long*)&color,10,2);
	VesaSetWriteMode(VESA_XOR);
	drawpig(&pig1);
	drawbird(bird1,3);
	drawbox(box1,6);
	drawpig(&pig2);
	drawbird(bird2,3);
	drawbox(box2,6);
	VesaRect(890,65,1020,105);
	vsmouse1(box1,&pig1,bird1,box2,&pig2,bird2);
}

void vsmouse1(BOX *box1,PIG *pig1,BIRD *bird1,BOX *box2,PIG *pig2,BIRD *bird2)
{
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=768; 
	int buttons=0,xm,ym,x0,y0,x,y;
	xmin=1;
	xmax=x_max-1;
	ymin=1;
	ymax=y_max-2;
	VesaSetWriteMode(VESA_XOR);
	VesaRect(890,65,1020,105);
	VesaRect(10,65,140,105);
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
		if(abs(x-217)<85&&abs(y-482)<85&&buttons)
		{
			cursor(x,y);
			x=217;
			y=482;
			song("music\\launch.wav");			
			for(;;)
			{
				vslaunch1(&x,&y,&buttons,pig1);
				if(buttons==0)
				{
					pig1->x=x;
					pig1->y=y;
					pig1->life=100;
					vsfly1(&x,&y,&buttons,box1,pig1,bird1);
					cursor(x,y);
					if(iswin(bird1,3))
					{
						ShowBmp24B(300,450,"tupian\\vswin1.bmp");
						vswinmouse();
					}
					vsmouse2(box2,pig2,bird2,box1,pig1,bird1);
				}
			}
		}
		else
		{
			if(x>=20&&x<=60&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		vszanting(&x,&y,&buttons);
                	}  
			if(x>=80&&x<=120&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		vs();
                	}  
			newxy(&x,&y,&buttons);  
		}
	}	         
}

void vsmouse2(BOX *box1,PIG *pig1,BIRD *bird1,BOX *box2,PIG *pig2,BIRD *bird2)
{
	int xmin,xmax,ymin,ymax,x_max=1024,y_max=768; 
	int buttons=0,xm,ym,x0,y0,x,y;
	xmin=1;
	xmax=x_max-1;
	ymin=1;
	ymax=y_max-2;
	VesaSetWriteMode(VESA_XOR);
	VesaRect(10,65,140,105);
	VesaRect(890,65,1020,105);
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
		if(abs(x-800)<85&&abs(y-482)<85&&buttons)
		{
			cursor(x,y);
			x=800;
			y=482;	
			song("music\\launch.wav");		
			for(;;)
			{
				vslaunch2(&x,&y,&buttons,pig1);
				if(buttons==0)
				{
					pig1->x=x;
					pig1->y=y;
					pig1->life=100;
					vsfly2(&x,&y,&buttons,box1,pig1,bird1);
					cursor(x,y);
					if(iswin(bird1,3))
					{
						ShowBmp24B(300,450,"tupian\\vswin2.bmp");
						vswinmouse();
					}
					vsmouse1(box2,pig2,bird2,box1,pig1,bird1);
				}
			}
		}
		else
		{
			if(x>=20&&x<=60&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		vszanting(&x,&y,&buttons);
                	}  
			if(x>=80&&x<=120&&y>=20&&y<=60&&buttons)
              		{
                		delay(100);    
                       		vs();
                	}  
			newxy(&x,&y,&buttons);  
		}
	}	         
}

void vsfly1(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird)
{
	ICON pigbmp,birdbmp,boxbmp;
	pig1->vx=210-pig1->x+30;
        pig1->vy=pig1->y-482;
	cursor(pig1->x,pig1->y);
	ReadIconBmp24B(&pigbmp,pig1->bmp);
	ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
	ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
	for(;stopcheckbox(box,6)||pig1->vx!=0||pig1->vy!=0||stopcheckbird(bird,3);)
	{
		if(*mx>=20&&*mx<=60&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
                      	vszanting(mx,my,buttons);
                }  
		if(*mx>=80&&*mx<=120&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
                       	vs();
                }  
		newxy(mx,my,buttons); 
		drawmovepig(pig1,&pigbmp);
		drawmovebox(box,6,&boxbmp);
		drawmovebird(bird,3,&birdbmp);
		vshitcheckpig(box,pig1,bird,6,3);
		vshitcheckbox(bird,box,6,3);
		checkunder(bird,box,6,3);
		stopbox(box,6);
		stopbird(bird,3);
		delay(25);
	}
	ShowIcon24B(202,497,&pigbmp);
	CloseIcon(&pigbmp);
	CloseIcon(&birdbmp);
	CloseIcon(&boxbmp);	
}

void vsfly2(int *mx,int *my,int *buttons,BOX *box,PIG *pig1,BIRD *bird)
{
	ICON pigbmp,birdbmp,boxbmp;
	pig1->vx=800-pig1->x-30;
        pig1->vy=pig1->y-482;
	cursor(pig1->x,pig1->y);
	ReadIconBmp24B(&pigbmp,pig1->bmp);
	ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
	ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
	for(;stopcheckbox(box,6)||pig1->vx!=0||pig1->vy!=0||stopcheckbird(bird,3);)
	{
		if(*mx>=20&&*mx<=60&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
                      	vszanting(mx,my,buttons);
                }  
		if(*mx>=80&&*mx<=120&&*my>=20&&*my<=60&&*buttons)
              	{
			CloseIcon(&pigbmp);
			CloseIcon(&birdbmp);
			CloseIcon(&boxbmp);
                	delay(100);    
                       	vs();
                }  
		newxy(mx,my,buttons); 
		drawmovepig(pig1,&pigbmp);
		drawmovebox(box,6,&boxbmp);
		drawmovebird(bird,3,&birdbmp);
		vshitcheckpig(box,pig1,bird,6,3);
		vshitcheckbox(bird,box,6,3);
		checkunder(bird,box,6,3);
		stopbox(box,6);
		stopbird(bird,3);
		delay(25);
	}
	ShowIcon24B(785,497,&pigbmp);
	CloseIcon(&pigbmp);
	CloseIcon(&birdbmp);
	CloseIcon(&boxbmp);	
}

void vszanting(int *x,int *y,int *buttons)
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
			mainmenu();
		}
		if(*x>=45&&*x<=130&&*y>=100&&*y<=200&&*buttons)
		{
			delay(100);
			vs();
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

void vshitcheckpig(BOX *box,PIG *pig1,BIRD *bird,int i,int j)
{
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
							vscheckboxbz(box);
						}
						else
						{
							pig1->vx=pig1->vx*0.5;
							pig1->vy=pig1->vy*0.5;
							box->life=0;
							vscheckboxbz(box);
						}
					}
					else
					{
						if(fabs(pig1->vx)<35)
						{
							pig1->vx=pig1->vx*(-0.5);
							box->life=box->life-50;
							vscheckboxbz(box);
						}
						else if(fabs(pig1->vx)<90)
						{
							box->vx=pig1->vx*(0.5);
							pig1->vx=pig1->vx*(-0.5);
							box->life=box->life-60;
							vscheckboxbz(box);
						}
						else
						{
							pig1->vx=pig1->vx*0.6;
							pig1->vy=pig1->vy*1.5;
							box->life=0;
							vscheckboxbz(box);
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
					vscheckbirdbz(bird);
				}
			}
		}
		pig1->vy=pig1->vy-7*0.1;
		stoppig(pig1);
	}
}

void vshitcheckbox(BIRD *bird,BOX *box,int i,int n)
{
	int j=i,k=i,a;
	BOX *start=box,*temp=box;
	BIRD *birdtemp;
	for(;i>0;i--,box++)
	{
		if(box->life>0)
		{
			for(birdtemp=bird,a=n;a>0;a--,birdtemp++)
			{
				if(birdtemp->life>0&&abs(box->x-birdtemp->x)<35&&(birdtemp->y-box->y<35)&&(birdtemp->y-box->y>20))
				{
					birdtemp->life=0;
					vscheckbirdbz(birdtemp);
				}
			}
		}
		if(box->life>0&&box->vx!=0)
		{
			for(birdtemp=bird,a=n;a>0;a--,birdtemp++)
			{
				if(abs(box->x-birdtemp->x)<33&&abs(box->y-birdtemp->y)<33&&birdtemp->life>0)
				{
					box->life=0;
					vscheckboxbz(box);
					birdtemp->life=0;
					vscheckbirdbz(birdtemp);
					break;
				}
			}
			for(j=k,temp=start;j>0;j--,temp++)
			{
				if(temp->life>0)
				{
					if(abs(box->x-temp->x)<40&&abs(box->y-temp->y)<30&&((box->x-temp->x)!=0||(box->y-temp->y)!=0))
					{
						box->life=0;
						vscheckboxbz(box);
						break;
					}
				}
			}
			if(box->x<20||box->x>1004)
			{
				box->life=0;
				vscheckboxbz(box);
			}
		}
	}
}

void vscheckboxbz(BOX *box)
{
	ICON boxbmp;
	if(box->life<1)
	{
		ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
		ShowIcon24B(box->x-20,box->y+20,&boxbmp);
		CloseIcon(&boxbmp);
		boom(box->x,box->y);
	}
}

void vscheckbirdbz(BIRD *bird)
{
	ICON birdbmp;
	if(bird->life<1)
	{
		ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
		ShowIcon24B(bird->x-15,bird->y+15,&birdbmp);
		CloseIcon(&birdbmp);
		boompig(bird->x,bird->y);
	}
}

void vslaunch1(int *mx,int *my,int *mbutt,PIG *pig1)
{
	ICON pigbmp;
	int xx0=*mx,yy0=*my;
	int xm,ym;
	read(&xm,&ym,mbutt);
	if(ym>570)
	{
		ym=570;
	}
	if(ym<400)
	{
		ym=400;
	}
	if(xm>300)
	{
		xm=300;
	}
	if(xm<130)
	{
		xm=130;
	}
	ReadIconBmp24B(&pigbmp,pig1->bmp);
        ShowIcon24B(xx0-15,yy0+15,&pigbmp);
        ShowIcon24B(xm-15,ym+15,&pigbmp);
	CloseIcon(&pigbmp);
        *mx=xm;
        *my=ym;
}

void vslaunch2(int *mx,int *my,int *mbutt,PIG *pig1)
{
	ICON pigbmp;
	int xx0=*mx,yy0=*my;
	int xm,ym;
	read(&xm,&ym,mbutt);
	if(ym>570)
	{
		ym=570;
	}
	if(ym<400)
	{
		ym=400;
	}
	if(xm>900)
	{
		xm=900;
	}
	if(xm<700)
	{
		xm=700;
	}
	ReadIconBmp24B(&pigbmp,pig1->bmp);
        ShowIcon24B(xx0-15,yy0+15,&pigbmp);
        ShowIcon24B(xm-15,ym+15,&pigbmp);
	CloseIcon(&pigbmp);
        *mx=xm;
        *my=ym;
}

void vswinmouse()
{
	int x=512,y=400,buttons=0;
	cursor(x,y);
	for(;;)
	{
		newxy(&x,&y,&buttons);
		if(x>=375&&x<=490&&y>=390&&y<=415&&buttons)
		{
			delay(100);
			mainmenu();
		}
		if(x>=565&&x<=670&&y>=390&&y<=415&&buttons)
		{
			delay(100);
			vs();
		}
	}
}

int iswin(BIRD *bird,int i)
{
	for(;i>0;i--,bird++)
	{
		if(bird->life>0)
		{
			break;
		}
	}
	if(i==0)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}