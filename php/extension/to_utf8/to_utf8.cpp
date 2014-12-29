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
#include "php_to_utf8.h"
#include <sys/shm.h>
#include <sys/stat.h>
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}
}

#include "source/utfdetect.h"
#include "source/gbkdetect.h"
#include "source/tl_codetbl.cpp"

extern unsigned short * _G_map_gbk_2_ucs;
extern unsigned short * _G_map_ucs_2_gbk;

/* If you declare any globals in php_to_utf8.h uncomment this:
*/
ZEND_DECLARE_MODULE_GLOBALS(to_utf8)

/* True global resources - no need for thread safety here */
static int le_to_utf8;
/* {{{ to_utf8_functions[]
 *
 * Every user visible function must have an entry in to_utf8_functions[].
 */
zend_function_entry to_utf8_functions[] = {
	PHP_FE(to_utf8,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in to_utf8_functions[] */
};
/* }}} */

/* {{{ to_utf8_module_entry
 */
zend_module_entry to_utf8_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"to_utf8",
	to_utf8_functions,
	PHP_MINIT(to_utf8),
	PHP_MSHUTDOWN(to_utf8),
	PHP_RINIT(to_utf8),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(to_utf8),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(to_utf8),
#if ZEND_MODULE_API_NO >= 20010901
	"0.2", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TO_UTF8
BEGIN_EXTERN_C()
ZEND_GET_MODULE(to_utf8)
END_EXTERN_C()
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("to_utf8.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_to_utf8_globals, to_utf8_globals)
    STD_PHP_INI_ENTRY("to_utf8.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_to_utf8_globals, to_utf8_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_to_utf8_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_to_utf8_init_globals(zend_to_utf8_globals *to_utf8_globals)
{
	to_utf8_globals->_G_map_gbk_2_ucs = 0;
	to_utf8_globals->_G_map_ucs_2_gbk = NULL;
}
}}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(to_utf8)
{
	int segment_id1;
	int segment_id2;
	int share_mem_key1 = 360000;
	int share_mem_key2 = 370000;
	const int shared_segment_size = 0x30000;

	segment_id1 = shmget(share_mem_key1, shared_segment_size, IPC_CREAT | S_IRUSR | S_IWUSR); /* 绑定到共享内存块 */
	segment_id2 = shmget(share_mem_key2, shared_segment_size, IPC_CREAT | S_IRUSR | S_IWUSR); /* 绑定到共享内存块 */
	_G_map_gbk_2_ucs = (unsigned short *)shmat(segment_id1, NULL, 0); // 共享内存标识 绑定到进程内存的地址 可读可写控制
	_G_map_ucs_2_gbk = (unsigned short *)shmat(segment_id2, NULL, 0); // 共享内存标识 绑定到进程内存的地址 可读可写控制

	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(to_utf8)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	shmdt(_G_map_gbk_2_ucs);
	shmdt(_G_map_ucs_2_gbk);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(to_utf8)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(to_utf8)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(to_utf8)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "to_utf8 support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto string to_utf8(string input, bool strict)
    */
PHP_FUNCTION(to_utf8)
{
	char *input = NULL;
	int argc = ZEND_NUM_ARGS();
	int input_len;
	zend_bool strict = true;

	if (zend_parse_parameters(argc TSRMLS_CC, "s|b", &input, &input_len, &strict) == FAILURE) 
		return;

	int isutf8 = 0;
	if (strict) {
		isutf8 = is_utf8_strict(input, input + strlen(input));
	} else {
		isutf8 = is_utf8_ignore(input, input + strlen(input), 1);
	}

	if (isutf8 > 0) {
		RETURN_STRINGL(input, strlen(input), 1);
	}
	
	int isgbk = 0;
	if (isutf8 < 0 || strict == true) {
		isgbk = is_gbk_encoding(input, input + strlen(input), 2);
		if (isgbk <= 0) {
			RETURN_STRINGL(input, strlen(input), 1);
		}
	}

	char *wbuff = (char *)emalloc((sizeof(char) * strlen(input) * 3 / 2) + 1);
	int res = encoding::conv_gbk_2_utf8(input, strlen(input), wbuff, (strlen(input) * 3 / 2) + 1);
	if (res > 0) {
		RETURN_STRINGL(wbuff, res, 1);
	}
	efree(wbuff);

	RETURN_NULL();
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
