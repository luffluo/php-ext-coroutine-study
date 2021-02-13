#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H

#include "php_study.h"
#include "coroutine.h"
#include <stack>

#define DEFAULT_PHP_STACK_PAGE_SIZE     8192
#define PHP_CORO_TASK_SLOT              ((int)((ZEND_MM_ALIGNED_SIZE(sizeof(php_coro_task)) + ZEND_MM_ALIGNED_SIZE(sizeof(zval)) - 1) / ZEND_MM_ALIGNED_SIZE(sizeof(zval))))

struct php_study_fci_fcc {
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
};

struct php_coro_args {
    zend_fcall_info_cache *fci_cache;
    zval *argv;
    uint32_t argc;
};

struct php_coro_task {
    zval *vm_stack_top;
    zval *vm_stack_end;
    zend_vm_stack vm_stack; // current coroutine stack pointer
    size_t vm_stack_page_size;
    zend_execute_data *execute_data; // current coroutine stack frame
    study::Coroutine *co;
    std::stack<php_study_fci_fcc *> *defer_tasks;
};

namespace study {
    class PhpCoroutine {
        public:
            static long create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv);
            static void defer(php_study_fci_fcc *defer_fci_fcc);

        protected:
            static php_coro_task main_task;

            static void save_task(php_coro_task *task);
            static void save_vm_stack(php_coro_task *task);
            static php_coro_task *get_task();
            static void vm_stack_init(void);
            static void create_func(void *arg);
    };
}

#endif /* end STUDY_COROUTINE_H */