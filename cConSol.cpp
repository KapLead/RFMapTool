#include <stdafx.h>
#include "cConsol.h"

cConsol	g_console;

cConsol::cConsol()
{
	__fStdOut = NULL;
	__hStdOut = NULL;
}

cConsol::~cConsol()
{

}


void cConsol::Init(char* TitleName,int width, int height, char* fname)
{
#ifdef _DEBUG
	AllocConsole();
	SetConsoleTitle(TitleName);

	__hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD co = {width,height};
	SetConsoleScreenBufferSize(__hStdOut, co);

	if(fname)
		__fStdOut = fopen(fname, "w");

#endif
}


int cConsol::printf(char *fmt, ...)
{
#ifdef _DEBUG
	char s[300];
	va_list argptr;
	int cnt;

	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);

	DWORD cCharsWritten;

	if(__hStdOut)
		WriteConsole(__hStdOut, s, strlen(s), &cCharsWritten, NULL);

	if(__fStdOut)
		fprintf(__fStdOut, s);

	return(cnt);

#else
	return 0;
#endif
}

int  cConsol::xyPrintf(LPCTSTR s, SHORT x, SHORT y, WORD wAtt)
{
	DWORD dwCellsWritten = 0;	
	DWORD dwLen;
	COORD c;	
	
	dwLen = _tcslen(s);
	if (dwLen == 0)
		return 0;

	c.X = x;
	c.Y = y;

	WriteConsoleOutputCharacter(__hStdOut, s, dwLen, c, &dwCellsWritten);

/*
	if (wAtt != NORMAL_TEXT)
      WriteAttributesAt(wAtt, dwLen, x, y);
*/
	return dwCellsWritten;
}

/*
DWORD cConsole::WriteAt(LPCTSTR s, SHORT x, SHORT y, WORD wAtt)
{
   // Write a string at a position, and return number of characters written.
   DWORD dwCellsWritten = 0;
   DWORD dwLen;
   COORD c;

   dwLen = _tcslen(s);
   if (dwLen == 0)
      return 0;

   c.X = x;
   c.Y = y;

   // Write string
   WriteConsoleOutputCharacter(hActive, s, dwLen, c, &dwCellsWritten);

   // If the attributes are anything other than normal text, output them too
   if (wAtt != NORMAL_TEXT)
      WriteAttributesAt(wAtt, dwLen, x, y);

   return dwCellsWritten;
} 

*/