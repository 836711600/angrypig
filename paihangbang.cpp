#include"angrypig.h"

int compare(unsigned int score,char *s)
{
		struct data people[10],*str,*str1;
		FILE *fp;
		int j;
		str=people;
		if((fp=fopen(s,"rb"))==NULL)
		{
			printf("不能打开文件，按任意键退出!!");
			getch();
			exit(1);	
		}
		fread(str,sizeof(struct data),10,fp);
		fclose(fp);
		if(score>people[0].grade||
			 score>people[1].grade||
			 score>people[2].grade||
			 score>people[3].grade||
			 score>people[4].grade||
			 score>people[5].grade||
			 score>people[6].grade||
			 score>people[7].grade||
			 score>people[8].grade||
			 score>people[9].grade)
			 	return 1;
		else 
			return 0;
}

void paixu(char *name,unsigned int score,char *s)
{

	struct data people[10],new1,*str;
	FILE *fp;
	int i,k;
	str=people;
	new1.grade=score;
	strcpy(new1.name,name);

	if((fp=fopen(s,"rb"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}
	fread(str,sizeof(struct data),10,fp);
	fclose(fp);
	for(k=0;k<10;k++)
	{
		if(new1.grade>people[k].grade)
		{
			if(k==9)
			{
				people[9]=new1;
			}
			else
			{
				for(i=9;i>k;i--)
					people[i]=people[i-1];
				people[k]=new1;
				break;
			}
		}
	}
	str=people;
	
	if((fp=fopen(s,"wb+"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}
	fwrite(str,sizeof(struct data),10,fp);
	fclose(fp);
}


void filestart()
{
	FILE *fp;
	struct data boy[10]={
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0},
			{"unrecord",0}
		};
	struct data *pp;
	
	pp=boy;
	if((fp=fopen(".\\file\\level1.txt","wb+"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}
	fwrite(pp,sizeof(struct data),10,fp);
	fclose(fp);

	if((fp=fopen(".\\file\\level2.txt","wb+"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}
	fwrite(pp,sizeof(struct data),10,fp);
	fclose(fp);

	if((fp=fopen(".\\file\\level3.txt","wb+"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}
	fwrite(pp,sizeof(struct data),10,fp);
	fclose(fp);

	if((fp=fopen(".\\file\\level4.txt","wb+"))==NULL)
	{
		printf("不能打开文件，按任意键退出!!");
		getch();
		exit(1);	
	}
	fwrite(pp,sizeof(struct data),10,fp);
	fclose(fp);
}

void lockstart()
{
	FILE *fp;
	int lock[4]={1,0,0,0},*lock1;
	lock1=lock;
	if((fp=fopen(".\\file\\lock.txt","wt+"))==NULL)
	{
		printf("初始化LOCK.TXT失败！！");
		getch();
		exit(0);
	}
	fwrite(lock1,sizeof(int),4,fp);
	fclose(fp);
}
