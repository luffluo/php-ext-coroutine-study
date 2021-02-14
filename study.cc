/* study extension for PHP */

#include <stdio.h>
#include <iostream>
#include <uv.h>
#include "php_study.h"

using namespace std;

uint64_t repeat = 0;

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

static void callback(uv_timer_t *handle)
{
    repeat = repeat + 1;

    cout << "repeat count:" << repeat << endl;
}

PHP_FUNCTION(study_timer_test)
{
    uv_loop_t *loop = uv_default_loop();
    uv_timer_t timer_req;
    
    uv_timer_init(loop, &timer_req);
    uv_timer_start(&timer_req, callback, 1000, 1000);
    uv_run(loop, UV_RUN_DEFAULT);
}

/* {{{ void study_test1()
 */
PHP_FUNCTION(study_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "study");
}
/* }}} */

ZEND_BEGIN_ARG_INFO(arginfo_study_test1, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(study_coroutine_create);

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()



PHP_MINIT_FUNCTION(study)
{
    study_coroutine_util_init();
    study_coroutine_server_coro_init();

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(study)
{
    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(study)
{
	return SUCCESS;
}
/* }}} */

PHP_RSHUTDOWN_FUNCTION(study)
{
    return SUCCESS;
}

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(study)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "study support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ study_functions[]
 */
static const zend_function_entry study_functions[] = {
    PHP_FE(study_coroutine_create, arginfo_study_coroutine_create)
    PHP_FALIAS(go, study_coroutine_create, arginfo_study_coroutine_create)
	PHP_FE(study_test1,		arginfo_study_test1)
    PHP_FE(study_timer_test, NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ study_module_entry
 */
zend_module_entry study_module_entry = {
	STANDARD_MODULE_HEADER,
	"study",					/* Extension name */
	study_functions,			/* zend_function_entry */
	PHP_MINIT(study),							/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(study),							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(study),			/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(study),							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(study),			/* PHP_MINFO - Module info */
	PHP_STUDY_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STUDY
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(study)
#endif

