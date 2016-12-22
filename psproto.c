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
static int le_sproto;
static int le_sproto_type;
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
	//zval *values;
};

struct decode_ud {
	int mainindex;
	zval *arr;
	zval *mapkey;
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
		RETURN_RES(zend_register_resource(sp, le_sproto));
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
		RETURN_RES(zend_register_resource(sp, le_sproto));
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
	  struct sproto *rsp=zend_fetch_resource(Z_RES_P(sp),"sproto",le_sproto);
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
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_sproto);
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
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_sproto);
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
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_sproto);
		if (rsp)
		{
			struct sproto_type * spt = sproto_type(rsp, name);
			if (spt)
			{
				RETURN_RES(zend_register_resource(spt, le_sproto_type));
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
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp), "sproto", le_sproto);
		if (rsp)
		{
			struct sproto_type * spt=sproto_protoquery(rsp,proto,what);
			if (spt)
			{
				RETURN_RES(zend_register_resource(spt,le_sproto_type));
			}
		}
	}
	RETURN_LONG(0);

}
/* }}} */

static int decode(const struct sproto_arg *args)
{
	struct decode_ud *self = args->ud;
	const char *tagname = args->tagname;
	int tagid = args->tagid;
	int type = args->type;
	int index = args->index;
	int mainindex = args->mainindex;
	int length = args->length;
	zval *obj = self->arr;
	zval data,arr;
	if (index != 0)
	{
		zend_string *key = zend_string_init(tagname, strlen(tagname), 0);
		obj = zend_hash_find(Z_ARR_P(self->arr), key);
		if (obj == NULL)
		{
			array_init(&arr);
			obj = &arr;
			zend_hash_add(Z_ARR_P(self->arr), key,obj);
			ZVAL_DEREF(obj);
			if (index < 0)
			{
				return 0;
			}
		}
	}
	
	struct decode_ud sub = {0};
	zval narr;
	int r = 0;
	
	//char *buffer = NULL;
	switch (type)
	{
	case SPROTO_TINTEGER:
		if (length == 4)
		{
			ZVAL_LONG(&data,*(uint32_t*)args->value);
		}
		else if (length == 8)
		{
			ZVAL_LONG(&data, *(uint64_t*)args->value);
		}
		else {
			php_error(E_WARNING, "unexpected integer length");
		}
		//printf("%s %lld\n", args->tagname, Z_LVAL(data));
		break;
	case SPROTO_TBOOLEAN:
		ZVAL_BOOL(&data,*(uint32_t*)args->value);
		break;
	case SPROTO_TSTRING:
		ZVAL_STRINGL(&data, args->value,length);
		//printf("%s %s\n", args->tagname, Z_STR(data)->val);
		break;
	case SPROTO_TSTRUCT:
		array_init(&narr);
		sub.arr = &narr;
		//ZVAL_ARR(sub.arr, Z_ARR(narr));
		if (args->mainindex >= 0)
		{
			sub.mainindex = args->mainindex;
			r = sproto_decode(args->subtype, args->value, length, decode, &sub);
			if (r < 0)
			{
				return SPROTO_CB_ERROR;
			}
			//php_printf("%d \n", Z_STR_P(sub.mapkey)->val);
			zend_hash_add(Z_ARR_P(obj), Z_STR_P(sub.mapkey), sub.arr);
		}
		else {
			sub.mainindex = -1;
			//data=*sub.arr;
			ZVAL_ARR(&data, Z_ARR_P(sub.arr));
			r = sproto_decode(args->subtype, args->value, length, decode, &sub);
			if (r < 0)
			{
				return SPROTO_CB_ERROR;
			}
			if (r != length)
			{
				return r;
			}
			break;
		}
	default:
		break;
	}
	if (Z_TYPE(data)!=IS_NULL)
	{
		zend_string *key = zend_string_init(tagname, strlen(tagname), 0);
		/*if (Z_TYPE(data) == IS_ARRAY)
		{
			php_printf("%s %d\n", key->val, zend_array_count(Z_ARR(data)));
			zval *v;
			zend_string *k;
			ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARR(data), k, v);
			if (Z_TYPE_P(v) == IS_STRING)
			{
				php_printf("%s %s\n", k->val, Z_STR_P(v)->val);
			}else{
				php_printf("%s %d\n", k->val, Z_LVAL_P(v));
			}
		    ZEND_HASH_FOREACH_END();
		}*/

		//zend_hash_index_add();
		if (args->type==SPROTO_TSTRUCT)
		{
			//php_printf("%d %d %d\n",args->index, zend_array_count(Z_ARR_P(obj)),obj);
			//zend_hash_add(Z_ARR_P(self->arr), key, &data);

			zend_hash_index_add(Z_ARR_P(obj),args->index-1,&data);
		}
		else {
			zend_hash_add(Z_ARR_P(self->arr), key, &data);
		}
		if (self->mainindex == tagid)
		{
			self->mapkey = &data;
		}
	}
	else {
		if (self->mainindex == tagid)
		{
			php_error(E_WARNING, "map key type not support");
			return SPROTO_CB_ERROR;
		}
	}
	return 0;
}
struct sproto_type {
	const char * name;
	int n;
	int base;
	int maxn;
	struct field *f;
};
/* {{{ proto array sp_decode(resource tp, string name)
   decode sproto buffer */
PHP_FUNCTION(sp_decode)
{
	char *buffer = NULL;
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	size_t buffer_len;
	zval *sp = NULL;
	zval arr,ret,len;
	array_init(&arr);
	array_init(&ret);
	if (zend_parse_parameters(argc, "rs", &sp, &buffer, &buffer_len) == FAILURE)
		return;
	struct decode_ud self = { 0 };
	if (sp) {
		struct sproto_type *spt =zend_fetch_resource(Z_RES_P(sp),"sproto_type",le_sproto_type);
		if (spt)
		{
			//php_printf("name=%s mn=%d n=%d\n",spt->name,spt->maxn,spt->n);
			self.arr = &arr;
			int r = sproto_decode(spt,buffer,buffer_len,decode,&self);
			if (r < 0)
			{
				php_error(E_WARNING, "sproto c lib error");
				ZVAL_DEREF(self.arr);
				RETURN_NULL();
			}
			ZVAL_LONG(&len,r);
		}
	}
	//zval arr;
	//ZVAL_NEW_ARR(&arr);
	zend_hash_index_add(Z_ARR(ret),0,&arr);
	zend_hash_index_add(Z_ARR(ret), 1,&len );
	RETURN_ARR(Z_ARR(ret));
}
/* }}} */
static int encode(const struct sproto_arg *args)
{
	struct encode_ud *self = args->ud;
	const char *tagname = args->tagname;
	int tagid = args->tagid;
	int type = args->type;
	int index = args->index;
	int mainindex = args->mainindex;
	int length = args->length;
	//php_printf("%s\n", args->tagname);
	zval *obj = self->arr;
	zval *data = NULL;
	zend_string *key = zend_string_init(tagname, strlen(tagname), 0);
	data = zend_hash_find(Z_ARR_P(obj), key);
	if (data == NULL)
	{
		if (index > 0)
		{
			return SPROTO_CB_NOARRAY;
		}
		return SPROTO_CB_NIL;
	}

	if (index > 0)
	{
		if (Z_TYPE_P(data) == IS_ARRAY)
		{
			int len=zend_array_count(Z_ARR_P(data));
			if (index > len)
			{
				return SPROTO_CB_NIL;
			}
			data = zend_hash_index_find(Z_ARR_P(data), index-1);
		}
	}
	struct encode_ud sub;
	int r = 0;
	uint64_t i,vh;
	
	switch (type)
	{
	case SPROTO_TINTEGER:
		if (Z_TYPE_P(data) == IS_LONG)
		{
			i = Z_LVAL_P(data);
			vh = i >> 31;
			if (vh == 0 || vh == -1)
			{
				*(uint32_t*)args->value = (uint32_t)i;
				return 4;
			}
			else {
				*(uint64_t*)args->value = (uint64_t)i;
				return 8;
			}
		}
		else {
			php_error(E_WARNING, "type mismatch, tag:%s, expected int or long", tagname);
			return SPROTO_CB_ERROR;
		}
	case SPROTO_TBOOLEAN:
		*(uint32_t*)args->value = (uint32_t)Z_LVAL_P(data);
		r = 4;
		break;
	case SPROTO_TSTRING:
		if (Z_TYPE_P(data) != IS_STRING)
		{
			php_error(E_WARNING, "type mismatch, tag:%s, expected strng", tagname);
			return SPROTO_CB_ERROR;
		}
		if (Z_STR_P(data)->len > length)
		{
			return SPROTO_CB_ERROR;
		}
		memcpy(args->value,Z_STR_P(data)->val, Z_STR_P(data)->len);
		return Z_STR_P(data)->len;
	case SPROTO_TSTRUCT:
		sub.arr = data;
		//Z_ARRVAL_P(sub.arr) = Z_ARRVAL_P(data);
		//php_printf("%s\n",args->subtype->name);
		r = sproto_encode(args->subtype, args->value, length, encode, &sub);
		if (r < 0)
			return SPROTO_CB_ERROR;
		return r;
	default:
		return 0;
	}
	return r;
}

/* {{{ proto string sp_encode(resource tp, string name,array arr)
   encode  sproto buffer */
PHP_FUNCTION(sp_encode)
{
	char *name = NULL;
	int argc = ZEND_NUM_ARGS();
	int tp_id = -1;
	size_t name_len;
	zval *rsp,*arr=NULL;
	struct encode_ud self;
	if (zend_parse_parameters(argc, "ra",&rsp,&arr) == FAILURE)
		return;
	if (rsp) {
		struct sproto_type *spt = zend_fetch_resource(Z_RES_P(rsp), "sproto_type", le_sproto_type);
		if (spt)
		{
			int sz = 4096;
			char *buffer = emalloc(sz);
			struct encode_ud self;
			self.arr = arr;
			for (;;)
			{
				int r = sproto_encode(spt,buffer,sz,encode,&self);
			
				if (r < 0)
				{
					buffer = erealloc(buffer,sz*2);
					sz *= 2;
				}
				else {
					zend_string *zs = zend_string_init(buffer,r,0);
					//php_printf(" len=%d\n",r);
					efree(buffer);
					RETURN_STR(zs);
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
	zval *tagid = NULL;
	zval arr,ret,pname;
	array_init(&arr);
	if (zend_parse_parameters(argc, "rz", &sp, &tagid) == FAILURE)
		return;
	long tag_id=0;
	if (sp) {
		struct sproto *rsp = zend_fetch_resource(Z_RES_P(sp),"sproto", le_sproto);
		if (rsp)
		{
			if (Z_TYPE_P(tagid)==IS_LONG)
			{
				const char *name=sproto_protoname(rsp,Z_LVAL_P(tagid));
				//ZVAL_STRINGL(&ret,name,strlen(name));
				ZVAL_STRINGL(&pname, name, strlen(name));
				tag_id = Z_LVAL_P(tagid);
				ZVAL_LONG(&ret, tag_id);
			}
			else if(Z_TYPE_P(tagid) == IS_STRING){
				tag_id=sproto_prototag(rsp, Z_STR_P(tagid)->val);
				ZVAL_LONG(&ret,tag_id);
				ZVAL_STR(&pname,Z_STR_P(tagid));
			}
			
			struct sproto_type *request;
			struct sproto_type *response;
			zval zreq,zres;
			request = sproto_protoquery(rsp, tag_id, SPROTO_REQUEST);
			
			if (request)
			{
				//php_printf("request =%s\n",request->name);
				ZVAL_RES(&zreq,zend_register_resource(request, le_sproto_type));
			}
			else {
				ZVAL_NULL(&zreq);
			}
			response = sproto_protoquery(rsp, tag_id, SPROTO_RESPONSE);
			if (response)
			{
				//php_printf("request name %s\n", response->name);
				//php_printf("response =%s\n", response->name);
				ZVAL_RES(&zres, zend_register_resource(response, le_sproto_type));
			}
			else {
				ZVAL_NULL(&zres);
			}
			zend_hash_index_add(Z_ARR(arr),0,&ret);
			zend_hash_index_add(Z_ARR(arr), 1, &zreq);
			zend_hash_index_add(Z_ARR(arr), 2, &zres);
			zend_hash_index_add(Z_ARR(arr), 3, &pname);
		}
		RETURN_ARR(Z_ARR(arr));
	}

	
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
	int sz = 4096;
	if (sz< maxsz)
	{
		sz =expand_sz(sz, maxsz);
	}
	zend_string *out = zend_string_alloc(sz, 0);
	bytes=sproto_pack(buffer, buffer_len, out->val, out->len);
	if(bytes>maxsz)
	{
		php_error(E_WARNING, "packing error, return size = %d", bytes);
	}
	out->val[bytes+1] = '\0';
	out->len = bytes;
	RETURN_STR(out);
	
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
	int sz = 4096;
	zend_string *out = zend_string_alloc(sz, 0);
	int r = sproto_unpack(buffer, buffer_len, out->val, out->len);
	if (r < 0)
	{
		  php_error(E_WARNING, "Invalid unpack stream");;
		  RETURN_NULL();
	}
	if (r>out->len)
	{
		size_t nsz = expand_sz(out->len, r);
		out = zend_string_extend(out, nsz, 0);
		r = sproto_unpack(buffer, buffer_len, out->val, out->len);
		if (r < 0)
		{
			php_error(E_WARNING, "Invalid unpack stream");;
			RETURN_NULL();
		}
	}
	out->val[r + 1] = '\0';
	out->len = r;
	RETURN_STR(out);
	
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
		res->ptr = NULL;

	}
}
void sproto_type_dtor(zend_resource *res TSRMLS_DC) {
	if (res->ptr)
	{
		//efree(res->ptr);
		//res->ptr = NULL;
		
	}
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(sproto)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	le_sproto=zend_register_list_destructors_ex(sproto_dtor, sproto_dtor, "sproto", module_number);
	le_sproto_type= zend_register_list_destructors_ex(sproto_type_dtor, sproto_type_dtor, "sproto_type", module_number);
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
