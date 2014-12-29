#ifndef __ANAMODULE_FUNC_GET_BUFFER_H_
#define __ANAMODULE_FUNC_GET_BUFFER_H_

#define FGB_BUFFERTYPE_CSS 	1
#define FGB_BUFFERTYPE_JS	2
#define FGB_BUFFERTYPE		0xF
#define FGB_BUFFERDOWNLOAD  1<<4

typedef void* (*func_get_buf)(void *handle, void*, int, int*, int/* type=0*/);
//old typedef void* (*func_get_buf)(void *handle, void*, int, int*);
#endif
