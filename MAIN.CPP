#include"angrypig.h"

void main()
{
	char *s[4]={"file\\level1.txt","file\\level2.txt","file\\level3.txt","file\\level4.txt"};
	int i;
	if(fopen("file\\lock.txt","rt")==NULL)
		lockstart();
	for(i=0;i<4;i++)
	{
		if(fopen(s[i],"rb")==NULL)
			filestart();
	}
	startdonghua();
	mainmenu();
}

