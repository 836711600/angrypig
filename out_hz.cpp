#include "angrypig.h"
FILE *hzk_p; 
void open_hzk();  
void get_hz(char incode[],char bytes[]);
void dishz(int x0,int y0,char code[],unsigned long color,int n);
void out_hz(int x0,int y0,char *s,unsigned long _color,int width,int times);
void out_hz(int x0,int y0,char *s,unsigned long _color,int width,int times)
{
	int x=x0;
        int y=y0;
     	open_hzk();
        while(*s!=NULL)
       {
	        while(x<1024&&(*s!=NULL))
	       {
			dishz(x,y,s,_color,times);
		        x+=(width+16*times);
	                s+=2;
	        }
	        x=20;y+=20;
        }
        fclose(hzk_p);
}
void open_hzk()
{
	hzk_p=fopen("HZK16","rb");
        if(!hzk_p)
        {
	        printf("the file HZK16 not exist! enter to syetem\n");
                getch();
                closegraph();
                exit(1);
        }
}
void get_hz(char incode[],char bytes[])
{
        unsigned char qh ,wh;
        unsigned long offset;
        qh=incode[0]-0xa0;
        wh=incode[1]-0xa0;
        offset=(94*(qh-1)+(wh-1))*32L;
        fseek(hzk_p,offset,SEEK_SET);
        fread(bytes,32,1,hzk_p);
}
void dishz( int x0,int y0,char code[], unsigned long color,int n)
{
	unsigned char mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
        int i,j,x,y,pos,k,l;
        char mat[32];
        get_hz(code ,mat);
        y=y0;
        for (i=0;i<16;i++)
	{
		x=x0;
                pos=2*i;
                for(j=0;j<16;++j)
	       {
		        if((mask[j%8]&mat[pos+j/8])!=NULL)
	                for(k=0;k<n;k++)
			{
			        for(l=0;l<n;l++)
			       {
					VesaPutPixel(x+k,y+l,color);
			        }
			}
	                x+=n;
		}
	        y+=n;
	}
}