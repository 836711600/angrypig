#include"angrypig.h"
void startdonghua()
{
	ICON tupian;
	ICON tupian1;
	int y=368,y1=388;
	int x=76,x1=830;
	int i,j=0;
	int keyhit,keynum;
	char *s[10]={
			".\\tupian\\pig1.bmp",
			".\\tupian\\1.bmp",
			".\\tupian\\2.bmp",
			".\\tupian\\3.bmp",
			".\\tupian\\4.bmp",
			".\\tupian\\5.bmp",
			".\\tupian\\6.bmp",
			".\\tupian\\7.bmp",
			".\\tupian\\3.bmp",
			".\\tupian\\pig2.bmp"
		  };
	char *music[2]={
			".\\music\\1.wav",
			".\\music\\2.wav"
		  };
       	RGBBMP24 color;
	if(InitVesa(VBE1024X768X16M)<0)
	{
		puts("This Video Card is not support!\n");
		getch();
		return;
	}
	VesaSetWriteMode(VESA_COPY);
	ShowBmp24B(0,767,".\\tupian\\donghua.bmp");
	for(i=0;i<5;i++,j++)
	{
        	keyhit=bioskey(1);
        	if(keyhit==0)
		{
			ReadIconBmp24B(&tupian, s[i]);
			ShowIcon24B(x,y,&tupian);
			CloseIcon(&tupian);

			ReadIconBmp24B(&tupian1, s[9-i]);
			ShowIcon24B(x1,y1,&tupian1);
			CloseIcon(&tupian1);

			song(music[j%2]);
		}		
		else if(keyhit!=0)
		{
			return;
		}

		switch(i)
		{
			case 0:
				x+=86;
				y+=20;
				x1-=86;
				y1-=36;
				break;
			case 1:
				x+=86;
				y-=37;
				x1-=50;
				y1+=34;
				break;
			case 2:
				x+=86;
				y+=30;
				x1-=86;
				y1+=30;
				break;
			case 3:
				x+=86;
				y+=44;
				x1-=110;
				y1+=30;
				break;
		}
	}
	return;
}
