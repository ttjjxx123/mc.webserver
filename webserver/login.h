#include<stdio.h>
#include<windows.h>
#include<string.h>
#define MAX 20

char* get_s(char* p, int n)
{
	fgets(p, n, stdin);
	char* pt = NULL;
	if (pt = strchr(p, '\n'))
	{
		*pt = '\0';
	}
	return p;
}
char* fget_s(char* p, int n, FILE* fp)
{
	fgets(p, n, fp);
	char* pt = NULL;
	if (pt = strchr(p, '\n'))
	{
		*pt = '\0';
	}
	return p;
}
