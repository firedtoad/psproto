/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_psproto.h"

/* If you declare any globals in php_psproto.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(psproto)
*/

/* True global resources - no need for thread safety here */
static int le_psproto;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("psproto.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_psproto_globals, psproto_globals)
    STD_PHP_INI_ENTRY("psproto.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_psproto_globals, psproto_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_psproto_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_psproto_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "psproto", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto resource ps_newproto(string buffer)
   create new sproto resource from string */
PHP_FUNCTION(ps_newproto)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t buffer_len;

	if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE) 
		return;

	php_error(E_WARNING, "ps_newproto: not yet implemented");
}
/* }}} */

/* {{{ proto resource ps_newproto_from_file(string file)
   create new sproto resource from file */
PHP_FUNCTION(ps_newproto_from_file)
{
	char *file = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t file_len;

	if (zend_parse_parameters(argc, "s", &file, &file_len) == FAILURE) 
		return;

	php_error(E_WARNING, "ps_newproto_from_file: not yet implemented");
}
/* }}} */

/* {{{ proto int ps_deleteproto(resource sp)
   release sproto resource  */
PHP_FUNCTION(ps_deleteproto)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "r", &sp) == FAILURE) 
		return;

	if (sp) {
		ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_deleteproto: not yet implemented");
}
/* }}} */

/* {{{ proto int ps_dumpproto(resource sp)
   dump  sproto   */
PHP_FUNCTION(ps_dumpproto)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "r", &sp) == FAILURE) 
		return;

	if (sp) {
		ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_dumpproto: not yet implemented");
}
/* }}} */

/* {{{ proto resource ps_querytype(resource sp, string name)
   query  sproto type */
PHP_FUNCTION(ps_querytype)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	size_t name_len;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "rs", &sp, &name, &name_len) == FAILURE) 
		return;

	if (sp) {
		ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_querytype: not yet implemented");
}
/* }}} */

/* {{{ proto array ps_decode(resource tp, string name)
   decode  sproto buffer */
PHP_FUNCTION(ps_decode)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	size_t name_len;
	zval *tp = NULL;

	if (zend_parse_parameters(argc, "rs", &tp, &name, &name_len) == FAILURE) 
		return;

	if (tp) {
		ZEND_FETCH_RESOURCE(???, ???, tp, tp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_decode: not yet implemented");
}
/* }}} */

/* {{{ proto string ps_encode(resource tp, string name)
   encode  sproto buffer */
PHP_FUNCTION(ps_encode)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	size_t name_len;
	zval *tp = NULL;

	if (zend_parse_parameters(argc, "rs", &tp, &name, &name_len) == FAILURE) 
		return;

	if (tp) {
		ZEND_FETCH_RESOURCE(???, ???, tp, tp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_encode: not yet implemented");
}
/* }}} */

/* {{{ proto array ps_protocol(resource sp, mixed arg)
   decode  sproto buffer */
PHP_FUNCTION(ps_protocol)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zval *sp = NULL;
	zval *arg = NULL;

	if (zend_parse_parameters(argc, "rz", &sp, &arg) == FAILURE) 
		return;

	if (sp) {
		ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_protocol: not yet implemented");
}
/* }}} */

/* {{{ proto resource ps_loadproto(int index)
   load proto */
PHP_FUNCTION(ps_loadproto)
{
	int argc = ZEND_NUM_ARGS();
	zend_long index;

	if (zend_parse_parameters(argc, "l", &index) == FAILURE) 
		return;

	php_error(E_WARNING, "ps_loadproto: not yet implemented");
}
/* }}} */

/* {{{ proto int ps_saveproto(resource sp, int index)
   save proto */
PHP_FUNCTION(ps_saveproto)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zend_long index;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "rl", &sp, &index) == FAILURE) 
		return;

	if (sp) {
		ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_saveproto: not yet implemented");
}
/* }}} */

/* {{{ proto array ps_default(resource tp)
   get default array */
PHP_FUNCTION(ps_default)
{
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	zval *tp = NULL;

	if (zend_parse_parameters(argc, "r", &tp) == FAILURE) 
		return;

	if (tp) {
		ZEND_FETCH_RESOURCE(???, ???, tp, tp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_default: not yet implemented");
}
/* }}} */

/* {{{ proto string ps_pack(string buffer)
   0'pack string  */
PHP_FUNCTION(ps_pack)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t buffer_len;

	if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE) 
		return;

	php_error(E_WARNING, "ps_pack: not yet implemented");
}
/* }}} */

/* {{{ proto string ps_unpack(string buffer)
   0'unpack string  */
PHP_FUNCTION(ps_unpack)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t buffer_len;

	if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE) 
		return;

	php_error(E_WARNING, "ps_unpack: not yet implemented");
}
/* }}} */


/* {{{ php_psproto_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_psproto_init_globals(zend_psproto_globals *psproto_globals)
{
	psproto_globals->global_value = 0;
	psproto_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(psproto)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(psproto)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(psproto)
{
#if defined(COMPILE_DL_PSPROTO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(psproto)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(psproto)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "psproto support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ psproto_functions[]
 *
 * Every user visible function must have an entry in psproto_functions[].
 */
const zend_function_entry psproto_functions[] = {
	PHP_FE(confirm_psproto_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(ps_newproto,	NULL)
	PHP_FE(ps_newproto_from_file,	NULL)
	PHP_FE(ps_deleteproto,	NULL)
	PHP_FE(ps_dumpproto,	NULL)
	PHP_FE(ps_querytype,	NULL)
	PHP_FE(ps_decode,	NULL)
	PHP_FE(ps_encode,	NULL)
	PHP_FE(ps_protocol,	NULL)
	PHP_FE(ps_loadproto,	NULL)
	PHP_FE(ps_saveproto,	NULL)
	PHP_FE(ps_default,	NULL)
	PHP_FE(ps_pack,	NULL)
	PHP_FE(ps_unpack,	NULL)
	PHP_FE_END	/* Must be the last line in psproto_functions[] */
};
/* }}} */

/* {{{ psproto_module_entry
 */
zend_module_entry psproto_module_entry = {
	STANDARD_MODULE_HEADER,
	"psproto",
	psproto_functions,
	PHP_MINIT(psproto),
	PHP_MSHUTDOWN(psproto),
	PHP_RINIT(psproto),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(psproto),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(psproto),
	PHP_PSPROTO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PSPROTO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(psproto)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
