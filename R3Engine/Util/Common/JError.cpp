// Error handling functions
// (c) 1997 Stuart Abercrombie

// TODO - Get these doing something useful
// Why the hell can't on_error be accessed?

#include "JError.h"
#include <stdio.h>

// Error handling in the engine (eg debugging history)
extern void on_error(void);

int No_warning=0;
void error_malloc(char *function)
{
	//on_error();
	MessageBox(NULL,"Out of memory","Error",MB_OK+
					MB_ICONSTOP);
	exit(1);
}


void Error(char *mes,char *mes1)
{
	FILE *fp;
	char imsi[256];
	strcpy(imsi,mes);
	strcat(imsi,mes1);

	fp = fopen("error_message.txt","wt");
	fprintf(fp,"%s%s",mes,mes1);
	fclose(fp);
	MessageBox(NULL,imsi,"message",MB_OK+
					MB_ICONSTOP);
	exit(1);
}
void Warning(char *mes,char *mes1)
{
	char imsi[256];
	if(No_warning)
		return;
	strcpy(imsi,mes);
	strcat(imsi,mes1);
	MessageBox(NULL,imsi,"message",MB_OK);
}

void PutMessage(char *mes,char *mes1)
{
	char imsi[256];
	strcpy(imsi,mes);
	strcat(imsi,mes1);
	MessageBox(NULL,imsi,"message",MB_OK);
}

void error_fatal(char *message,char *function)
{
	//on_error();
	MessageBox(NULL,"message","Fatal error in function",MB_OK+
					MB_ICONSTOP);
	exit(1);
}

void error_fatal_printf(char *function,char *format,...)
{
	FILE *error_file;
	char message[512];
	va_list data_in;
	va_start(data_in,format);
	
	//on_error();

	// Get formatted output
	vsprintf(message,format,data_in);

	// Write error message to fatal error file
	error_file=fopen("error.txt","wt");
	if (error_file!=NULL)
	{
		fprintf(error_file,"Error in function %s:\n",function);
		fprintf(error_file,message);
		fclose(error_file);
	}

	// Tell them about it
	MessageBox(NULL,message,"Fatal error",MB_OK+MB_ICONSTOP);
	exit(1);
}
