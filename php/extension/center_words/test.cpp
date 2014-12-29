#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <string.h>
#include "include/libKeywordExtract.h"
#include "include/sysutil.h"
#include "include/anamodule_util.h"
#include <algorithm>
#include <iostream>


int GetFileRecord(std::ifstream &infile, char *ppszOneLine, size_t maxlen)
{
  infile.getline(ppszOneLine, maxlen - 1);
  if(infile.fail()) 
  {
	ppszOneLine[0] = 0;
    return -1;
  } 
  return 1;
}


const int MAX_FILEPATH_LEN = 256 ;
int main(int argc, char ** argv)
{
  char c;
  char szSrcFile[MAX_FILEPATH_LEN+1] = {0};

  while(-1 != (c = getopt(argc, argv,"s:"))) 
  {
    switch(c) 
    {
      case 's':
        strncpy(szSrcFile,optarg,MAX_FILEPATH_LEN);
        break;
      default:
		break;
    }
  }
  if ( (0 == strlen(szSrcFile)) ) 
  { 
    fprintf(stderr, "Usage: %s  -s txt-file.\n", argv[0]);
    return -1;
  }
 
  std::ifstream infile(szSrcFile);
  if( infile.fail() ) 
  {
    fprintf(stderr, "open file [%s] fail. error message : %s\n", szSrcFile, strerror(errno));
    return -1;
  }

  initfunc pfninit;
  runfunc pfnrun;
  runfuncEx pfnrunEx;
  closefunc pfnclose;


  void *handle = dlopen("lib/libKeywordExtract.so",RTLD_LAZY);

  if(!handle)
  {
	  printf("Failed to load libKeywordExtract.so :  %s\n",dlerror());
	  exit(0);
  }
  dlerror();//clear errors
  pfninit = (initfunc)dlsym(handle,"anamod_init");

  if (NULL == pfninit){
	  printf("pfninit = NULL\n");
	  exit(0);
  }

  pfnrun  = (runfunc)dlsym(handle,"anamod_run");
  if (NULL == pfnrun){
	  printf("Error,  pfnrun = null\n");
	  exit(0);
  }

  pfnrunEx = (runfuncEx)dlsym(handle,"anamod_runEx");
  if (NULL == pfnrunEx){
	  printf("Error,  pfnrunEx = null\n");
	  exit(0);
  }

  pfnclose= (closefunc)dlsym(handle,"anamod_close");
  if (NULL == pfnclose){
	  printf("Error, pfnclose = null\n");
	  exit(0);
  }

  pfninit("config/map_key_extract.ini");

  const int INPUTLEN = 1024 * 1024 * 4;
  char * inbuffer = new char[INPUTLEN];
  char * outbuffer = new char[INPUTLEN];
  memset(inbuffer, 0, INPUTLEN);
  memset(outbuffer, 0, INPUTLEN);

  int iRet;
  int len = 1024 * 4 * sizeof(char);
  char pszTitle[1024] = {0};
  char pszContent[1024 * 4] = {0};
  char* pszDest = (char *)malloc(len);
  while(1) 
  {
    memset(pszDest, 0, len);

	memset(pszTitle, 0, sizeof(pszTitle));
	memset(pszContent, 0, sizeof(pszContent));

    iRet = GetFileRecord(infile, pszTitle, sizeof(pszTitle));
    if((iRet!=1) || strcmp(pszTitle, "")==0){
      //break;
		pszTitle[0] = '\0';
	}

    iRet = GetFileRecord(infile, pszContent, sizeof(pszContent));
    if(iRet == -1)
      break;
    else if(iRet == 0)
      pszContent[0] = 0;

	fprintf(stderr, "start to _TagRoot_ExtractTerm #####\n");
    //_TagRoot_ExtractTerm(my_handle, pszTitle, pszContent, 20, pszDest, 1024*4);

	DataPackText pack(inbuffer, INPUTLEN);
	pack.Put("title", 5,  pszTitle, strlen(pszTitle));
	pack.Put("body", 4,  pszContent, strlen(pszContent));
	pack.Put("count", 5,  "20", 2);

	int outLen = 0;
	int r = pfnrun("test", (char*)pack.GetBuffer(), pack.GetLength(), outbuffer, &outLen);
	printf("outLen = %d\n", outbuffer);
	outbuffer[outLen] = '\0';
    fprintf(stderr, "%s\n", outbuffer);

	WordInfo words[10] = {};
	r = pfnrunEx("test", (char*)pack.GetBuffer(), pack.GetLength(), words, 10);
	printf("out words count=%d\n", r);
	for (int i = 0; i < r; ++i){

		printf("Word %d : %s\n", i, words[i].word);

	}

  }

  infile.close();
  free(pszDest);

  //_TagRoot_Finalize(my_handle);
  return 1;
}


