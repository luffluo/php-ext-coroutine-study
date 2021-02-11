#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H

#include "php_study.h"

typedef struct _php_coro_args_t
{
    zend_fcall_info_cache *fci_cache;
    zval *argv;
    uint32_t argc;
} php_coro_args_t;

typedef struct _php_coro_task_t
{
    zval *vm_stack_top;
    zval *vm_stack_end;
    zend_vm_stack vm_stack; // current coroutine stack pointer
    size_t vm_stack_page_size;
    zend_execute_data *execute_data; // current coroutine stack frame
} php_coro_task_t;


namespace study {
    class PhpCoroutine {
        public:
            static long create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv);

        protected:
            static void save_task(php_coro_task_t *task);
            static void save_vm_stack(php_coro_task_t *task);
            static php_coro_task_t *get_task();
    };
}

#endif /* end STUDY_COROUTINE_H */