
#ifndef __JERROR_H__
#define __JERROR_H__

#include <windows.h>
#include <windowsx.h>


void error_malloc(char *);
void FatalError(char *,char *);		//ġ������ �����ϰ�� ����ض�.
void error_fatal_printf(char *,char *,...);
void Error(char *mes,char *mes1);
void Warning(char *mes,char *mes1);
void PutMessage(char *mes,char *mes1);

//������ ������ �޽��� �Լ� �����͸� �������ش�.
//���׽� ������ �޽��� �Լ� �����͸� �������ش�.
void SetErrorMessageProc(void (*proc)(char *));
void SetWarningMessageProc(void (*proc)(char *));

extern int No_warning;


#endif