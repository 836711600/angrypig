
#ifndef __EVENTDEF_H
#define __EVENTDEF_H

//一. 定义事情来源
#define	EV_SRC_CH0	0x100		//通道0事情
#define	EV_SRC_CH1	0x101		//通道1事情
#define	EV_SRC_CH2	0x102		//通道2事情
#define	EV_SRC_CH3	0x103		//通道3事情
#define EV_SRC_KBD	0x200		//键盘事情
//二. 定义事情代码
//	1. 定义键盘代码, 00组为ASCII代码，01至0x7f为组合按钮或扩展键

#define kbNoKey		0x0000
#define kbTab		0x0009
#define kbEnter		0x000d
#define kbSpace		0x0020

#define	kbCtrlA		0x1e01
#define kbCtrlB     0x3002
#define kbCtrlC     0x2e03
#define kbCtrlD     0x2004
#define kbCtrlE     0x1205
#define kbCtrlF     0x2106
#define kbCtrlG     0x2207
#define kbCtrlH     0x2308
#define kbCtrlI     0x1709
#define kbCtrlJ     0x240a
#define kbCtrlK     0x250b
#define kbCtrlL     0x260c
#define kbCtrlM     0x320d
#define kbCtrlN     0x310e
#define kbCtrlO     0x180f
#define kbCtrlP     0x1910
#define kbCtrlQ     0x1011
#define kbCtrlR     0x1312
#define kbCtrlS     0x1f13
#define kbCtrlT     0x1414
#define kbCtrlU     0x1615
#define kbCtrlV     0x2f16
#define kbCtrlW     0x1117
#define kbCtrlX     0x2d18
#define kbCtrlY     0x1519
#define kbCtrlZ     0x2c1a

#define kbEsc       0x011b
#define kbAltSpace  0x0200
#define kbCtrlIns   0x0400
#define kbShiftIns  0x0500
#define kbCtrlDel   0x0600
#define kbShiftDel  0x0700
#define kbBack      0x0e08
#define kbCtrlBack  0x0e7f
#define kbShiftTab  0x0f00
#define kbCtrlEnter 0x1c0a

#define kbAltQ      0x1000
#define kbAltW      0x1100
#define kbAltE      0x1200
#define kbAltR      0x1300
#define kbAltT      0x1400
#define kbAltY      0x1500
#define kbAltU      0x1600
#define kbAltI      0x1700
#define kbAltO      0x1800
#define kbAltP      0x1900
#define kbAltA      0x1e00
#define kbAltS      0x1f00
#define kbAltD      0x2000
#define kbAltF      0x2100
#define kbAltG      0x2200
#define kbAltH      0x2300
#define kbAltJ      0x2400
#define kbAltK      0x2500
#define kbAltL      0x2600
#define kbAltZ      0x2c00
#define kbAltX      0x2d00
#define kbAltC      0x2e00
#define kbAltV      0x2f00
#define kbAltB      0x3000
#define kbAltN      0x3100
#define kbAltM      0x3200
#define kbF1        0x3b00
#define kbF2        0x3c00
#define kbF3        0x3d00
#define kbF4        0x3e00
#define kbF5        0x3f00
#define kbF6        0x4000
#define kbF7        0x4100
#define kbF8        0x4200
#define kbF9        0x4300
#define kbF10       0x4400
#define kbHome      0x4700
#define kbUp        0x4800
#define kbPgUp      0x4900
#define kbGrayMinus 0x4a2d
#define kbLeft      0x4b00
#define kbRight     0x4d00
#define kbGrayPlus  0x4e2b
#define kbEnd       0x4f00
#define kbDown      0x5000
#define kbPgDn      0x5100
#define kbIns       0x5200
#define kbDel       0x5300
#define kbShiftF1   0x5400
#define kbShiftF2   0x5500
#define kbShiftF3   0x5600
#define kbShiftF4   0x5700
#define kbShiftF5   0x5800
#define kbShiftF6   0x5900
#define kbShiftF7   0x5a00
#define kbShiftF8   0x5b00
#define kbShiftF9   0x5c00
#define kbShiftF10  0x5d00
#define kbCtrlF1    0x5e00
#define kbCtrlF2    0x5f00
#define kbCtrlF3    0x6000
#define kbCtrlF4    0x6100
#define kbCtrlF5    0x6200
#define kbCtrlF6    0x6300
#define kbCtrlF7    0x6400
#define kbCtrlF8    0x6500
#define kbCtrlF9    0x6600
#define kbCtrlF10   0x6700
#define kbAltF1     0x6800
#define kbAltF2     0x6900
#define kbAltF3     0x6a00
#define kbAltF4     0x6b00
#define kbAltF5     0x6c00
#define kbAltF6     0x6d00
#define kbAltF7     0x6e00
#define kbAltF8     0x6f00
#define kbAltF9     0x7000
#define kbAltF10    0x7100
#define kbCtrlPrtSc 0x7200
#define kbCtrlLeft  0x7300
#define kbCtrlRight 0x7400
#define kbCtrlEnd   0x7500
#define kbCtrlPgDn  0x7600
#define kbCtrlHome  0x7700
#define kbAlt1      0x7800
#define kbAlt2      0x7900
#define kbAlt3      0x7a00
#define kbAlt4      0x7b00
#define kbAlt5      0x7c00
#define kbAlt6      0x7d00
#define kbAlt7      0x7e00
#define kbAlt8      0x7f00
#define kbAlt9      0x8000
#define kbAlt0      0x8100
#define kbAltMinus  0x8200
#define kbAltEqual  0x8300
#define kbCtrlPgUp  0x8400

#define Idle		0x7fff
#define RealRe		0x7ffe	//只要系统查询事件队列无条件执行

//2. 定义通道事情
#define	procBegin	0xa001
#define procEnd		0xa002
#define	procStart	0xa003	
#define	procStop	0xa004	
#define	procG92exe	0xa005	

#endif

