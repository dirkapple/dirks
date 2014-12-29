#ifndef _LIBKEYWORDEXTRACT_H_
#define _LIBKEYWORDEXTRACT_H_

extern "C" int anamod_init(const char* pszini);
extern "C" int anamod_run(const char* service, char* psz, int iLenSrc, char* pszDest, int *piLenDest);
extern "C" void anamod_close(void);

struct WordInfo
{
	float wgt;
	unsigned int tf;
	unsigned int idf;
	unsigned int pos;
	char info;
	char word[20];
};

extern "C" int anamod_runEx(const char* service, char* psz, int iLenSrc, WordInfo *, int piLenDest);

typedef int  (*runfunc)(const char*, char*, int, char*, int*);
typedef int  (*runfuncEx)(const char*, char*, int, WordInfo*, int);
typedef int  (*initfunc)(const char*);
typedef void (*closefunc)(void);


#endif

