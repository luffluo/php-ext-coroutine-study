#include "study_server_coro.h"

zend_class_entry study_coroutine_server_coro_ce;
zend_class_entry *study_coroutine_server_coro_ce_ptr;

PHP_METHOD(study_coroutine_server_coro, __construct)
{
    int sockfd;
    zval *zhost;
    zend_long zport;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(zhost)
        Z_PARAM_LONG(zport)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    sockfd = st_socket_create(ST_SOCK_TCP);
    st_socket_bind(sockfd, ST_SOCK_TCP, Z_STRVAL_P(zhost), zport);
    st_socket_listen(sockfd);

    zend_update_property_long(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("sock"), sockfd);
    zend_update_property_string(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("host"), Z_STRVAL_P(zhost));
    zend_update_property_long(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("port"), zport);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_construct, 0, 0, 2)
    ZEND_ARG_INFO(0, host)
    ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()

PHP_METHOD(study_coroutine_server_coro, accept)
{
    zval *sockfd;
    int connfd;

    sockfd = st_zend_read_property(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("sock"), 0);
    connfd = st_socket_accept(Z_LVAL_P(sockfd));

    RETURN_LONG(connfd);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry study_coroutine_server_coro_methods[] = {
    PHP_ME(study_coroutine_server_coro, __construct, arginfo_study_coroutine_server_coro_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(study_coroutine_server_coro, accept, arginfo_study_coroutine_server_coro_void, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void study_coroutine_server_coro_init()
{
    INIT_NS_CLASS_ENTRY(study_coroutine_server_coro_ce, "Study", "Coroutine\\Server", study_coroutine_server_coro_methods);
    study_coroutine_server_coro_ce_ptr = zend_register_internal_class(&study_coroutine_server_coro_ce TSRMLS_CC);

    zend_declare_property_long(study_coroutine_server_coro_ce_ptr, ZEND_STRL("sock"), -1, ZEND_ACC_PUBLIC);
    zend_declare_property_string(study_coroutine_server_coro_ce_ptr, ZEND_STRL("host"), "", ZEND_ACC_PUBLIC);
    zend_declare_property_long(study_coroutine_server_coro_ce_ptr, ZEND_STRL("port"), -1, ZEND_ACC_PUBLIC);
}
