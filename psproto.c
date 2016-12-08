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
#include "sproto.h"
#include "zend_smart_str.h"
#define MAX_GLOBALSPROTO 16
#define ENCODE_BUFFERSIZE 2050

#define ENCODE_MAXSIZE 0x1000000
#define ENCODE_DEEPLEVEL 64
/* If you declare any globals in php_psproto.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(psproto)
*/

/* True global resources - no need for thread safety here */
static int le_psproto;
static int le_psproto_type;
static zend_string *encode_buffer = NULL;
static zend_string *decode_buffer = NULL;
/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("psproto.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_psproto_globals, psproto_globals)
    STD_PHP_INI_ENTRY("psproto.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_psproto_globals, psproto_globals)
PHP_INI_END()
*/
/* }}} */


struct encode_ud {
	zval *arr;
	struct sproto_type *st;
	int tbl_index;
	const char * array_tag;
	int array_index;
	int deep;
	int iter_index;
};


/* {{{ proto resource ps_newproto(string buffer)
   create new sproto resource from string */
PHP_FUNCTION(sp_newproto)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t buffer_len;

	if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE) 
		return;
	struct sproto * sp;
	sp = sproto_create(buffer,buffer_len);
	if (sp)
	{
		RETURN_RES(zend_register_resource(sp, le_psproto));
	}
	RETURN_NULL();

}
/* }}} */

/* {{{ proto resource ps_newproto_from_file(string file)
   create new sproto resource from file */
PHP_FUNCTION(sp_newproto_from_file)
{
	char *file = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t file_len;

	if (zend_parse_parameters(argc, "s", &file, &file_len) == FAILURE) 
		return;
	char  *buffer = NULL;
	int len = 0,r=0;
	smart_str *smt_str=NULL;
	//zend_string *zstr=zend_string_init("", sizeof(""), 0);
	//smart_str_alloc(zstr, zstr->len, 0);
	FILE* f = fopen(file,"rb");
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f,0, SEEK_SET);
	buffer = emalloc(len+1);

	//smart_str_alloc(smt_str, 0, 0);
	while (r=fread(buffer+r, 1, 1024, f))
	{

		//smart_str_appends(buffer, smt_str);
	}
	//zendi_smart_strcmp
	struct sproto * sp;
	sp = sproto_create(buffer, len);
	if (sp)
	{
		RETURN_RES(zend_register_resource(sp, le_psproto));
	}
	if (buffer)
	{
		efree(buffer);
	}
	RETURN_NULL();
	
}
/* }}} */

/* {{{ proto int ps_deleteproto(resource sp)
   release sproto resource  */
PHP_FUNCTION(sp_deleteproto)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "r", &sp) == FAILURE) 
		return;

	if (sp) {
	  struct sproto *rsp=zend_fetch_resource(Z_RES_P(sp),"sproto",le_psproto);
	  if (rsp)
	  {
		  sproto_release(rsp);
	  }
	}
	RETURN_LONG(0);
}
/* }}} */

/* {{{ proto int ps_dumpproto(resource sp)
   dump  sproto   */
PHP_FUNCTION(sp_dumpproto)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "r", &sp) == FAILURE) 
		return;

	if (sp) {
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_psproto);
		if (rsp)
		{
			sproto_dump(rsp);
		}
	}
	else {
		php_error(E_WARNING, "sp_dumpproto: not a sproto resource");
	}
	RETURN_NULL();
}
/* }}} */

/* {{{ proto resource ps_querytype(resource sp, string name)
   query  sproto type */
PHP_FUNCTION(sp_prototag)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	size_t name_len;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "rs", &sp, &name, &name_len) == FAILURE) 
		return;

	if (sp) {
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_psproto);
		if (rsp)
		{
			RETURN_LONG(sproto_prototag(rsp, name));
		}
	}
	RETURN_LONG(0);
}
/* }}} */


/* {{{ proto resource ps_querytype(resource sp, string name)
query  sproto type */
PHP_FUNCTION(sp_type)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	size_t name_len;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "rs", &sp, &name, &name_len) == FAILURE)
		return;

	if (sp) {
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_psproto);
		if (rsp)
		{
			struct sproto_type * spt = sproto_type(rsp, name);
			if (spt)
			{
				RETURN_RES(zend_register_resource(&spt, le_psproto_type));
			}
		}
	}
	RETURN_LONG(0);
}
/* }}} */

/* {{{ proto resource ps_querytype(resource sp, string name)
query  sproto type */
PHP_FUNCTION(sp_querytype)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	long proto = 0, what = 0;
	//size_t name_len;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "rll",&sp,&proto,&what) == FAILURE)
		return;

	if (sp) {
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_psproto);
		if (rsp)
		{
			struct sproto_type * spt=sproto_protoquery(rsp,proto,what);
			if (spt)
			{
				RETURN_RES(zend_register_resource(&spt,le_psproto_type));
			}
		}
	}
	RETURN_LONG(0);

}
/* }}} */

/* {{{ proto array ps_decode(resource tp, string name)
   decode  sproto buffer */
PHP_FUNCTION(sp_decode)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	size_t name_len;
	zval *tp = NULL;

	if (zend_parse_parameters(argc, "rs", &tp, &name, &name_len) == FAILURE) 
		return;

	if (tp) {
		//ZEND_FETCH_RESOURCE(???, ???, tp, tp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_decode: not yet implemented");
}
/* }}} */



int encode(const struct sproto_arg *args)
{
	struct encode_ud *self = args->ud;
	if (self->deep > ENCODE_DEEPLEVEL)
	{
		php_error(E_WARNING, "The array is too deep");
		return 0;
	}
	zval *value = NULL;
	if (args->index > 0)
	{
		if (args->tagname != self->array_tag) {
			zend_string *key=zend_string_init(args->tagname,strlen(args->tagname),0);
			
			
			zval *value=zend_hash_find(Z_ARR_P(self->arr), key);
			if (!value)
			{
				self->array_index = 0;
				return SPROTO_CB_NOARRAY;
			}
		 }
	 }
	
	zend_string *zs=NULL;
	struct encode_ud sub;
	int r = 0;
	switch (args->type)
	{
	case SPROTO_TINTEGER:
		*(uint64_t*)args->value = (uint64_t)Z_LVAL(*value);
		return 8;
	case SPROTO_TBOOLEAN:
		*(uint32_t*)args->value = (uint32_t)Z_LVAL(*value);
		return 4;
	case SPROTO_TSTRING:
		zs=Z_STR(*value);
		if (zs->len > args->length)
		{
			return SPROTO_CB_ERROR;
		}
		memcpy(args->value,zs->val,zs->len);
		return zs->len;
	case SPROTO_TSTRUCT:
		sub.arr = value;
		sub.st = args->subtype;
		sub.tbl_index = 0;
		sub.array_tag = NULL;
		sub.array_index = 0;
		sub.deep = self->deep + 1;
		sub.iter_index = sub.tbl_index + 1;
		r = sproto_encode(args->subtype,args->value,args->length,encode,&sub);
		if (r < 0)
			return SPROTO_CB_ERROR;
		return r;
	default:
		return 0;
		break;
	}

}



struct sproto_type {
	const char * name;
	int n;
	int base;
	int maxn;
	struct field *f;
};
/* {{{ proto string ps_encode(resource tp, string name)
   encode  sproto buffer */
PHP_FUNCTION(sp_encode)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	size_t name_len;
	zval *tp=NULL,*arr=NULL;
	int tbl_index = 2;
	struct encode_ud self;
	if (zend_parse_parameters(argc, "rsa",&tp, &name, &name_len,&arr) == FAILURE)
		return;
	if (tp) {
		struct sproto_type *spt = zend_fetch_resource(Z_RES_P(tp), "sproto_type", le_psproto_type);
	
		if (spt)
		{
			self.arr = arr;
			self.st = spt;
			self.tbl_index = tbl_index;
			for (;;)
			{
				
				int r;
				self.array_tag = NULL;
				self.array_index = 0;
				self.deep = 0;
				self.iter_index = tbl_index + 1;
				php_printf("%d",spt->maxn);
				return;
				r=sproto_encode(spt, encode_buffer, encode_buffer->len, encode, &self);

				if (r < 0)
				{
					size_t nsz = encode_buffer->len * 2;
					encode_buffer = zend_string_extend(encode_buffer, nsz,0);
				}
				else {
					encode_buffer->val[r + 1] = '\0';
					encode_buffer->len = r;
					RETURN_STR(encode_buffer);
				}
			}
			
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ proto array ps_protocol(resource sp, mixed arg)
   decode  sproto buffer */
PHP_FUNCTION(sp_protocol)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zval *sp = NULL;
	zval *arg = NULL;

	if (zend_parse_parameters(argc, "rz", &sp, &arg) == FAILURE) 
		return;

	if (sp) {

		//ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_protocol: not yet implemented");
}
/* }}} */

/* {{{ proto resource ps_loadproto(int index)
   load proto */
PHP_FUNCTION(sp_loadproto)
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
PHP_FUNCTION(sp_saveproto)
{
	int argc = ZEND_NUM_ARGS();
	int sp_id = -1;
	zend_long index;
	zval *sp = NULL;

	if (zend_parse_parameters(argc, "rl", &sp, &index) == FAILURE) 
		return;

	if (sp) {
		//ZEND_FETCH_RESOURCE(???, ???, sp, sp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_saveproto: not yet implemented");
}
/* }}} */

/* {{{ proto array ps_default(resource tp)
   get default array */
PHP_FUNCTION(sp_default)
{
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	zval *tp = NULL;

	if (zend_parse_parameters(argc, "r", &tp) == FAILURE) 
		return;

	if (tp) {
		//ZEND_FETCH_RESOURCE(???, ???, tp, tp_id, "???", ???_rsrc_id);
	}

	php_error(E_WARNING, "ps_default: not yet implemented");
}
/* }}} */
static size_t expand_sz(int osz,int nsz)
{
	do {
		osz *= 2;
	} while (osz < nsz);
	if (osz > ENCODE_MAXSIZE) {
		php_error(E_WARNING, "object is too large (>%d)", ENCODE_MAXSIZE);
		return 0;
	}
	return osz;
}
/* {{{ proto string ps_pack(string buffer)
   0'pack string  */
PHP_FUNCTION(sp_pack)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t buffer_len = 0;
	
	int bytes=0;
	if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE)
		return;
	size_t maxsz = (buffer_len + 2047) / 2048 * 2 + buffer_len + 2;
	if (encode_buffer->len < maxsz)
	{
		size_t nsz=expand_sz(encode_buffer->len, maxsz);
		encode_buffer=zend_string_extend(encode_buffer,nsz,0);
	}
	bytes=sproto_pack(buffer, buffer_len, encode_buffer->val, encode_buffer->len);
	if(bytes>maxsz)
	{
		php_error(E_WARNING, "packing error, return size = %d", bytes);
	}
	encode_buffer->val[bytes+1] = '\0';
	encode_buffer->len = bytes;
	RETURN_STR(encode_buffer);
	
}
/* }}} */

/* {{{ proto string ps_unpack(string buffer)
   0'unpack string  */
PHP_FUNCTION(sp_unpack)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t buffer_len;
	int bytes = 0;
	if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE) 
		return;
	int r = sproto_unpack(buffer, buffer_len, decode_buffer->val, decode_buffer->len);
	if (r < 0)
	{
		  php_error(E_WARNING, "Invalid unpack stream");;
		  RETURN_NULL();
	}
		
	if (r>decode_buffer->len)
	{
		size_t nsz = expand_sz(decode_buffer->len, r);
		decode_buffer = zend_string_extend(decode_buffer, nsz, 0);
		r = sproto_unpack(buffer, buffer_len, decode_buffer->val, decode_buffer->len);
		if (r < 0)
		{
			php_error(E_WARNING, "Invalid unpack stream");;
			RETURN_NULL();
		}
	}

	decode_buffer->val[r + 1] = '\0';
	decode_buffer->len = r;
	RETURN_STR(decode_buffer);
	
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

void sproto_dtor(zend_resource *res TSRMLS_DC) {
	if (res->ptr)
	{
		sproto_release(res->ptr);
	}
}
void sproto_type_dtor(zend_resource *res TSRMLS_DC) {
	if (res->ptr)
	{
		efree(res->ptr);
	}
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(sproto)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	le_psproto=zend_register_list_destructors_ex(sproto_dtor, sproto_dtor, "sproto", module_number);
	le_psproto_type= zend_register_list_destructors_ex(sproto_type_dtor, sproto_type_dtor, "sproto_type", module_number);
	encode_buffer = zend_string_alloc(ENCODE_BUFFERSIZE,0);
	decode_buffer = zend_string_alloc(ENCODE_BUFFERSIZE,0);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(sproto)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	zend_string_free(encode_buffer);
	zend_string_free(decode_buffer);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(sproto)
{
#if defined(COMPILE_DL_SPROTO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(sproto)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(sproto)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "sproto support", "enabled");
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
const zend_function_entry sproto_functions[] = {
	PHP_FE(sp_newproto,	NULL)
	PHP_FE(sp_newproto_from_file,	NULL)
	PHP_FE(sp_deleteproto,	NULL)
	PHP_FE(sp_dumpproto,	NULL)
	PHP_FE(sp_prototag,	NULL)
	PHP_FE(sp_querytype,NULL)
	PHP_FE(sp_type,NULL)
	PHP_FE(sp_decode,	NULL)
	PHP_FE(sp_encode,	NULL)
	PHP_FE(sp_protocol,	NULL)
	PHP_FE(sp_loadproto,	NULL)
	PHP_FE(sp_saveproto,	NULL)
	PHP_FE(sp_default,	NULL)
	PHP_FE(sp_pack,	NULL)
	PHP_FE(sp_unpack,	NULL)
	PHP_FE_END	/* Must be the last line in psproto_functions[] */
};
/* }}} */

/* {{{ psproto_module_entry
 */
zend_module_entry sproto_module_entry = {
	STANDARD_MODULE_HEADER,
	"sproto",
	sproto_functions,
	PHP_MINIT(sproto),
	PHP_MSHUTDOWN(sproto),
	PHP_RINIT(sproto),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(sproto),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(sproto),
	PHP_PSPROTO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SPROTO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sproto)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
