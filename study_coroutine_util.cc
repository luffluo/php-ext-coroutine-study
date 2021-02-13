#include "study_coroutine.h"
#include <unordered_map>

using study::Coroutine;
using study::PhpCoroutine;

zend_class_entry study_coroutine_ce;
zend_class_entry *study_coroutine_ce_ptr;

static std::unordered_map<long, Coroutine *> user_yield_coros;

PHP_METHOD(study_coroutine_util, create)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fcc)
        Z_PARAM_VARIADIC('*', fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    long cid = PhpCoroutine::create(&fcc, fci.param_count, fci.params);

    RETURN_LONG(cid);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(study_coroutine_util, yield)
{
    Coroutine *co = Coroutine::get_current();
    user_yield_coros[co->get_cid()] = co;
    co->yield();

    RETURN_TRUE;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(study_coroutine_util, resume)
{
    zend_long cid = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(cid)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    auto coroutine_iterator = user_yield_coros.find(cid);
    
    Coroutine *co = coroutine_iterator->second;
    user_yield_coros.erase(cid);
    co->resume();

    RETURN_TRUE;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_resume, 0, 0, 1)
    ZEND_ARG_INFO(0, cid)
ZEND_END_ARG_INFO()

PHP_METHOD(study_coroutine_util, getCid)
{
    Coroutine *co = Coroutine::get_current();
    
    RETURN_LONG(co->get_cid());
}

const zend_function_entry study_coroutine_util_methods[] = {
    PHP_ME(study_coroutine_util, create, arginfo_study_coroutine_create, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, yield, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, resume, arginfo_study_coroutine_resume, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, getCid, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void study_coroutine_util_init()
{
    INIT_NS_CLASS_ENTRY(study_coroutine_ce, "Study", "Coroutine", study_coroutine_util_methods);
    study_coroutine_ce_ptr = zend_register_internal_class(&study_coroutine_ce TSRMLS_CC);
}