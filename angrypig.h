#include<stdio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include <process.h>
#include <bios.h>
#include <alloc.h>
#include <string.h>
#include <math.h>
#include<io.h>
#include<mem.h>
#include<fcntl.h>
#include <stdarg.h>
#include <time.h>
#include "vesa24b1.h"
#include "event.h"

typedef struct tagPIG
{
	char bmp[25];
	int life; 
	int x;
	int y;
	float vx;
	float vy; 
}PIG;

typedef struct tagBOX
{
	int life;
	int x;
	int y;
	float vx;
	float vy;
}BOX;

typedef struct tagBIRD
{
	int life;
	int x;
	int y;
	float vx;
	float vy;
}BIRD;

struct data
{
	char name[15];
	unsigned int grade;
};

/*功能*/
void drawbox(BOX *box,int i);
void drawbird(BIRD *bird,int i);
void drawpig(PIG *pig1);
void drawmovepig(PIG *pig1,ICON *pigbmp);
void drawmovebox(BOX *box,int i,ICON *boxbmp);
void drawmovebird(BIRD *bird,int i,ICON *birdbmp);
void hitcheckpig(BOX *box,PIG *pig1,BIRD *bird,int i,int j,unsigned int *score);
void hitcheckbox(BIRD *bird,BOX *box,int i,int n,unsigned int *score);
void checkunder(BIRD *bird,BOX *box,int i,int n);
int stopcheckbox(BOX *box,int i);
int stopcheckbird(BIRD *bird,int i);
void stopbox(BOX *box,int i);
void stoppig(PIG *pig1);
void stopbird(BIRD *bird,int i);
int ispass(BIRD *bird,int i);
void putin(int xmi,int ymi,char *s);
void boom(int x,int y);
void boompig(int x,int y);
void checkboxbz(BOX *box,unsigned int *score);
void checkbirdbz(BIRD *bird,unsigned int *score);
void launch(int *mx,int *my,int *mbutt,PIG *pig1);

/*界面*/
void startdonghua();
void mainmenu();
void help();
int level();
int level1();
int level2();
int level3();
int level4();
void vs();
void highscore();
void ceshi();

/*鼠标*/
void cursor(int x,int y);
int init(int xmi,int xma,int ymi,int yma);
int read(int *mx,int *my,int *mbutt);
void newxy(int *mx,int *my,int *mbutt);
/*输出*/
void out_hz(int x0,int y0,char *s,unsigned long _color,int width,int times);
void textxy(int x,int y,char *str,unsigned long _color,int n);
/*排行榜*/
void filestart();
void lockstart();
void lockstart();
int compare(unsigned int score,char *s);
void paixu(char name[15],unsigned int score,char *s);
/*自设地图*/
void selfset();
void bianji();
void levelzdy();
/*song*/
void song(char* songname);