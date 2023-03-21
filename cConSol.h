//*************************************************
// file : CCONSOL.H
//	
// 2001. G.V inc.
//
// code by Woochul Choe ( wcchoe@hotmail.com )
//
// history
//*************************************************
// 2001.07.26	Start
// 2001.09.21   Modify
#ifndef	__CCONSOL_H__
#define	__CCONSOL_H__

#include <stdio.h>
//#include <windows.h>
#include <tchar.h>

#include <afxwin.h>

const WORD NORMAL_TEXT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

class cConsol{
protected:
	FILE*		__fStdOut;
	HANDLE		__hStdOut;

public:
	cConsol();
	virtual ~cConsol();

	void Init(char* TitleName = "TitleName",int width = 80, int height = 25,char* fname = NULL);
	int	 printf(char *fmt, ...);
	int  xyPrintf(LPCTSTR s, SHORT x, SHORT y, 
                 WORD att = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
};


extern cConsol	g_console;

//***************************************************************************
//
#endif	//	End header file.
//
//***************************************************************************
