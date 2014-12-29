/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include <iconv.h>
#define OUTLEN 255
}

#include "include/libKeywordExtract.h"
#include "include/sysutil.h"
#include "include/anamodule_util.h"

extern "C" {
#include "php_cntwd.h"
}

ZEND_DECLARE_MODULE_GLOBALS(cntwd)

/* True global resources - no need for thread safety here */
static int le_cntwd;

/*
static void *handle; 
static initfunc pfninit;
static runfunc pfnrun;
static runfuncEx pfnrunEx;
static closefunc pfnclose;
*/
/* {{{ cntwd_functions[]
 *
 * Every user visible function must have an entry in cntwd_functions[].
 */
zend_function_entry cntwd_functions[] = {
	PHP_FE(cntwd,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in cntwd_functions[] */
};
/* }}} */

/* {{{ cntwd_module_entry
 */
zend_module_entry cntwd_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"cntwd",
	cntwd_functions,
	PHP_MINIT(cntwd),
	PHP_MSHUTDOWN(cntwd),
	NULL,
	//PHP_RINIT(cntwd),		/* Replace with NULL if there's nothing to do at request start */
	NULL,
	//PHP_RSHUTDOWN(cntwd),	/* Replace with NULL if there's nothing to do at request end */
	//PHP_MINFO(cntwd),
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CNTWD
BEGIN_EXTERN_C()
ZEND_GET_MODULE(cntwd)
END_EXTERN_C()
#endif

PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("cntwd.map_config_path", "/home/s/apps/conf/map_key_extract.ini", PHP_INI_ALL, OnUpdateString, map_config_path, zend_cntwd_globals, cntwd_globals)
STD_PHP_INI_ENTRY("cntwd.so_path", "/home/s/apps/conf/libKeywordExtract.so", PHP_INI_ALL, OnUpdateString, so_path, zend_cntwd_globals, cntwd_globals)
PHP_INI_END()

static void php_cntwd_init_globals(zend_cntwd_globals *cntwd_globals)
{
	cntwd_globals->map_config_path = "/home/s/apps/conf/map_key_extract.ini";
	cntwd_globals->so_path = "/home/s/apps/conf/libKeywordExtract.so";
}

int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	size_t tmp1 = (size_t)inlen;
	size_t tmp2 = (size_t)outlen;
	if (iconv(cd, pin, &tmp1, pout, &tmp2)==-1) return -1;
	iconv_close(cd);
	return 0;
}

/*UNICODE码转为GB2312码*/
int u2g(char *inbuf, int inlen, char *outbuf, int outlen)
{
	return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}
/*GB2312码转为UNICODE码*/
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(cntwd)
{
	FILE *file = fopen("/home/libofeng/info.txt", "at+");
	fwrite("minit...\n", 9, 1, file);
	//const char *the_config = INI_STR("cntwd.map_config_path");
	//php_printf("ini_get: %s\n", the_config);
	//const char *the_path = INI_STR("cntwd.so_path");
	//php_printf("%s\n", CNTWD_G(map_config_path));
	//php_printf("%s\n", CNTWD_G(so_path));


	REGISTER_INI_ENTRIES();
	if ((access(CNTWD_G(so_path), 0) != 0)) {
		return FAILURE;
		fwrite("so_path\n", 8, 1, file);
		fclose(file);
	}

	if ((access(CNTWD_G(map_config_path), 0) != 0)) {
		return FAILURE;
		fwrite("map_config_path\n", 16, 1, file);
		fclose(file);
	}

	//php_printf("%s\n", CNTWD_G(map_config_path));
	//php_printf("%s\n", CNTWD_G(so_path));
	CNTWD_G(handle) = dlopen(CNTWD_G(so_path), RTLD_LAZY);
	if(!CNTWD_G(handle)) {
		return FAILURE;
	}

	fclose(file);
	dlerror();

	//const char *the_path = INI_STR("cntwd.so_path");
	//php_printf("ini_get: %s\n", the_path);
	//php_printf("%s\n", CNTWD_G(so_path));
	CNTWD_G(pfninit) = (initfunc)dlsym(CNTWD_G(handle),"anamod_init");
	if (NULL == CNTWD_G(pfninit)) {
		return FAILURE;
	}

	//php_printf("%s\n", CNTWD_G(so_path));
	CNTWD_G(pfnrun) = (runfunc)dlsym(CNTWD_G(handle),"anamod_run");
	if (NULL == CNTWD_G(pfnrun)) {
		return FAILURE;
	}

	CNTWD_G(pfnrunEx) = (runfuncEx)dlsym(CNTWD_G(handle),"anamod_runEx");
	if (NULL == CNTWD_G(pfnrunEx)) {
		return FAILURE;
	}

	CNTWD_G(pfnclose) = (closefunc)dlsym(CNTWD_G(handle),"anamod_close");
	if (NULL == CNTWD_G(pfnclose)) {
		return FAILURE;
	}

	CNTWD_G(pfninit)(CNTWD_G(map_config_path));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(cntwd)
{
	UNREGISTER_INI_ENTRIES();
	CNTWD_G(pfnclose)();
	dlclose(CNTWD_G(handle));
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(cntwd)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(cntwd)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(cntwd)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "cntwd support", "enabled");
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_cntwd_compiled(string arg)
   Return a string to confirm that the module is compiled in */
/*
PHP_FUNCTION(confirm_cntwd_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "cntwd", arg);
	RETURN_STRINGL(strg, len, 0);
}
*/
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto array cntwd(string input)
    */
PHP_FUNCTION(cntwd)
{
	char *input = NULL;
	int argc = ZEND_NUM_ARGS();
	int input_len;

	array_init(return_value);   

	if (zend_parse_parameters(argc TSRMLS_CC, "s", &input, &input_len) == FAILURE) 
		return;

	char out[input_len];
	u2g(input, input_len, out, input_len);
	input = out;

	const int INPUTLEN = 1024 * 1024 * 4;
	char * inbuffer = (char *)emalloc(INPUTLEN * sizeof(char));//new char[INPUTLEN];
	memset(inbuffer, 0, INPUTLEN);

	if(strcmp((input), "")==0){
		input = '\0';
	}

	DataPackText pack(inbuffer, INPUTLEN);
	pack.Put("title", 5,  input, strlen(input));
	pack.Put("body", 4,  input, strlen(input));
	pack.Put("count", 5,  "20", 2);

	WordInfo words[10] = {};
	int r = CNTWD_G(pfnrunEx)("test", (char*)pack.GetBuffer(), pack.GetLength(), words, 10);

	char word_tmp[input_len];
	for (int i = 0; i < r; ++i){
		word_tmp[0] = '\0';
		g2u(words[i].word, strlen(words[i].word), word_tmp, input_len);

		add_index_string(return_value, i, word_tmp, 1);
		//add_index_string(return_value, i, words[i].word, 1);
	}

	efree(inbuffer);
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
