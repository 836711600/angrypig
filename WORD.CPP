#include"angrypig.h"
void textxy(int x,int y,char *str,unsigned long _color,int n)
{
	char far *VGA16;
	int addr;
	int i,j,k,l;
	char byte;
	asm {
		PUSH BP
		MOV BX,0x0600
		MOV AX,0x1130
		INT 0x10
		MOV AX,BP
		POP BP
		MOV WORD PTR VGA16,AX
		MOV AX,ES
		MOV WORD PTR VGA16+2,AX 
		}
	while((addr=*str++<<4)!=NULL)
	{
		switch(n)
		{
			case 1:
				for(j=y;j<y+16;j++)
				{
    					byte=*(VGA16+addr++);
   					for(i=x;i<x+8;i++)
					{
						if(byte&0x80)
							VesaPutPixel(i,j,_color);
						byte<<=1;
					}
				}
				x+=8;
				break;
    			case 2:
				for(j=y;j<y+16*4;j=j+4)
				{
    					byte=*(VGA16+addr++);
   					for(i=x;i<x+8*4;i=i+2)
					{
						if(byte&0x80)
						{ 
							for(l=0;l<4;l++)
								for(k=0;k<4;k++)
									VesaPutPixel(i+l,j+k,_color);

						}byte<<=1;
					}
				}
				x+=8*2;
				break;

		}
	}
}