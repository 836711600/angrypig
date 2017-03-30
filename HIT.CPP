#include"angrypig.h"

void drawmovepig(PIG *pig1,ICON *pigbmp)
{
	if(pig1->life>0)
	{
		ShowIcon24B(pig1->x-15,pig1->y+15,pigbmp);
		pig1->x=pig1->vx*0.1+pig1->x;
		pig1->y=-pig1->vy*0.1+0.5*7*0.1*0.1+pig1->y;
		ShowIcon24B(pig1->x-15,pig1->y+15,pigbmp);
	}
}

void drawpig(PIG *pig1)
{
	ICON pigbmp;
	if(pig1->life>0)
	{
		ReadIconBmp24B(&pigbmp,pig1->bmp);
		ShowIcon24B(pig1->x-15,pig1->y+15,&pigbmp);
		CloseIcon(&pigbmp);
	}
}

void drawmovebox(BOX *box,int i,ICON *boxbmp)
{
	for(;i>0;i--)
	{
		if(box->life>0&&(box->vx!=0||box->vy!=0))
		{
			ShowIcon24B(box->x-20,box->y+20,boxbmp);
			box->x=box->x+box->vx*0.1;
			box->y=box->y+box->vy*0.1;
			ShowIcon24B(box->x-20,box->y+20,boxbmp);
		}
		box++;
	}
}

void drawbox(BOX *box,int i)
{
	ICON boxbmp;
	ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
	for(;i>0;i--)
	{
		if(box->life>0)
		{
			ShowIcon24B(box->x-20,box->y+20,&boxbmp);
		}
		box++;
	}
	CloseIcon(&boxbmp);
}

void drawmovebird(BIRD *bird,int i,ICON *birdbmp)
{
	for(;i>0;i--)
	{
		if(bird->life>0&&bird->vy!=0)
		{
			ShowIcon24B(bird->x-15,bird->y+15,birdbmp);
			bird->y=bird->y+bird->vy*0.1;
			ShowIcon24B(bird->x-15,bird->y+15,birdbmp);
		}
		bird++;
	}
}

void drawbird(BIRD *bird,int i)
{
	ICON birdbmp;
	ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
	for(;i>0;i--)
	{
		if(bird->life>0)
		{
			ShowIcon24B(bird->x-15,bird->y+15,&birdbmp);
		}
		bird++;
	}
	CloseIcon(&birdbmp);
}

void hitcheckpig(BOX *box,PIG *pig1,BIRD *bird,int i,int j,unsigned int *score)
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
		stoppig(pig1);
	}
}

void hitcheckbox(BIRD *bird,BOX *box,int i,int n,unsigned int *score)
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
					checkbirdbz(birdtemp,score);
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
					checkboxbz(box,score);
					birdtemp->life=0;
					checkbirdbz(birdtemp,score);
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
						checkboxbz(box,score);
						break;
					}
				}
			}
			if(box->x<20||box->x>1004)
			{
				box->life=0;
				checkboxbz(box,score);
			}
		}
	}
}

void checkunder(BIRD *bird,BOX *box,int i,int n)
{
	int j=i,k=i,m=i;
	BOX *start=box,*temp=box;
	for(;i>0;i--,box++)
	{
		if(box->life>0)
		{
			for(j=k,temp=start;j>0;j--,temp++)
			{
				if(temp->life>0)
				{
					if(abs(temp->x-box->x)<=20&&(temp->y-box->y)<41&&(temp->y-box->y)>20)
					{
						box->vy=0;
						break;
					}
					if(box->y>=565)
					{
						box->vx=0;
						box->vy=0;
						break;
					}
				}
			}
			if(j==0)
			{
				box->vy=box->vy+0.9;
			}
		}
	}
	for(;n>0;n--,bird++)
	{
		for(box=start,k=m;k>0;k--,box++)
		{
			if(box->life>0)
			{
				if(bird->life>0)
				{
					if(abs(bird->x-box->x)<20&&box->y-bird->y<38&&box->y-bird->y>32)
					{
						bird->vy=0;
						break;
					}
				}
			}
			if(bird->y>569)
			{
				bird->vy=0;
				break;
			}
		}
		if(k==0)
		{
			bird->vy=bird->vy+0.7;
		}
	}	
}

int stopcheckbox(BOX *box,int i)
{
	for(;i>0;i--,box++)
	{
		if(box->vx!=0||box->vy!=0)
		{
			break;
		}
	}
	if(i==0) 
		return(0);
	else 
		return(1);
}

int stopcheckbird(BIRD *bird,int i)
{
	for(;i>0;i--,bird++)
	{
		if(bird->vx!=0||bird->vy!=0)
		{
			break;
		}
	}
	if(i==0) 
		return(0);
	else 
		return(1);
}

void stopbox(BOX *box,int i)
{
	for(;i>0;i--,box++)
	{
		if(box->life<1)
		{
			box->vx=0;
			box->vy=0;
		}
	}	
}

void stopbird(BIRD *bird,int i)
{
	for(;i>0;i--,bird++)
	{
		if(bird->life<1)
		{
			bird->vx=0;
			bird->vy=0;
		}
	}	
}

int ispass(BIRD *bird,int i)
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

void putin(int xmi,int ymi,char *s)
{
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);

	int i=0,j,key;
     	char key1,key2,s1[15];
     	char buffer[2];
     	for(;;)
     	{
		key=(bioskey(0)&255);
		key1=(char)key;
		if(key==8||key==13||(key>47&&key<58)||(key>64&&key<91)||(key>96&&key<123))
		{
			if(i==15)
			{
				s1[i]='\0';
				break;
			}
			
			else if(key==13)
	  		{
	    			s1[i]='\0';
	    			break;
	  		}
	  		else if(key==8)
	  		{
	    			if(i>=1)
	    			{
	      				i--;
	      				key2=s1[i];
	      				sprintf(buffer,"%c",key2);
					textxy(xmi+12*i,ymi+5,buffer,*(unsigned long*)&color,1);
	      				s1[i]='\0';
	    			}
	 		}
	 		else if((key>47&&key<58)||(key>64&&key<91)||(key>96&&key<123))
	 		{
					s1[i]=key1 ;
					sprintf(buffer,"%c",key1);
					textxy(xmi+12*i,ymi+5,buffer,*(unsigned long*)&color,1);
	      				i++;
	 		}
      		}
    	}
   	strcpy(s,s1);
}

void boom(int x,int y)
{
	
	VesaSetWriteMode(VESA_XOR);
song("music\\bom.wav");
	ShowBmp24B(x-20,y+20,"tupian\\boxbz1.bmp");
delay(30);
	ShowBmp24B(x-20,y+20,"tupian\\boxbz1.bmp");

	ShowBmp24B(x-20,y+20,"tupian\\boxbz2.bmp");
delay(30);	
	ShowBmp24B(x-20,y+20,"tupian\\boxbz2.bmp");

	ShowBmp24B(x-20,y+20,"tupian\\boxbz3.bmp");
delay(30);	
	ShowBmp24B(x-20,y+20,"tupian\\boxbz3.bmp");

	ShowBmp24B(x-20,y+20,"tupian\\boxbz4.bmp");
delay(30);	
	ShowBmp24B(x-20,y+20,"tupian\\boxbz4.bmp");
}

void boompig(int x,int y)
{
	song("music\\birdbom.wav");
	VesaSetWriteMode(VESA_XOR);
	ShowBmp24B(x-20,y+20,"tupian\\yun1.bmp");
	delay(40);	
	ShowBmp24B(x-20,y+20,"tupian\\yun1.bmp");
	ShowBmp24B(x-20,y+20,"tupian\\yun2.bmp");
	delay(40);	
	ShowBmp24B(x-20,y+20,"tupian\\yun2.bmp");
	ShowBmp24B(x-20,y+20,"tupian\\yun3.bmp");
	delay(40);	
	ShowBmp24B(x-20,y+20,"tupian\\yun3.bmp");
}

void checkboxbz(BOX *box,unsigned int *score)
{
	char str[6];
	ICON boxbmp;
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if(box->life<1)
	{
		ReadIconBmp24B(&boxbmp,"tupian\\box.bmp");
		ShowIcon24B(box->x-20,box->y+20,&boxbmp);
		CloseIcon(&boxbmp);
		boom(box->x,box->y);
		sprintf(str,"%d",*score);
		textxy(700,20,str,*(unsigned long*)&color,1);
		*score=*score+500;
		sprintf(str,"%d",*score);
		textxy(700,20,str,*(unsigned long*)&color,1);
	}
}

void checkbirdbz(BIRD *bird,unsigned int *score)
{
	char str[6];
	ICON birdbmp;
	RGBBMP24 color;
	color.r=255;
	color.g=255;
	color.b=255;
	VesaSetColor(*(unsigned long*)&color);
	if(bird->life<1)
	{
		ReadIconBmp24B(&birdbmp,"tupian\\bird.bmp");
		ShowIcon24B(bird->x-15,bird->y+15,&birdbmp);
		CloseIcon(&birdbmp);
		boompig(bird->x,bird->y);
		sprintf(str,"%d",*score);
		textxy(700,20,str,*(unsigned long*)&color,1);
		*score=*score+2000;
		sprintf(str,"%d",*score);
		textxy(700,20,str,*(unsigned long*)&color,1);
	}
}


void stoppig(PIG *pig1)
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
		boompig(pig1->x,pig1->y);
	}
	if(pig1->x<20||pig1->x>1039)
	{
		pig1->vx=0;
		pig1->vy=0;
		pig1->life=0;
		ReadIconBmp24B(&pigbmp,pig1->bmp);
		ShowIcon24B(pig1->x-15,pig1->y+15,&pigbmp);
		CloseIcon(&pigbmp);
		boompig(pig1->x,pig1->y);
	}
}


void launch(int *mx,int *my,int *mbutt,PIG *pig1)
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