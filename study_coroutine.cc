#include "coroutine.h"
#include "study_coroutine.h"

using study::Coroutine;
using study::PhpCoroutine;

php_coro_task PhpCoroutine::main_task = {0};

long PhpCoroutine::create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv)
{
    php_coro_args php_coro_args;
    php_coro_args.fci_cache = fci_cache;
    php_coro_args.argc = argc;
    php_coro_args.argv = argv;

    save_task(get_task());

    return 0;
}

void PhpCoroutine::save_task(php_coro_task *task)
{
    save_vm_stack(task);
}

void PhpCoroutine::save_vm_stack(php_coro_task *task)
{
    task->vm_stack_top = EG(vm_stack_top);
    task->vm_stack_end = EG(vm_stack_end);
    task->vm_stack = EG(vm_stack);
    task->vm_stack_page_size = EG(vm_stack_page_size);
    task->execute_data = EG(current_execute_data);
}

php_coro_task *PhpCoroutine::get_task()
{
    php_coro_task *task = (php_coro_task *) Coroutine::get_current_task();

    return task ? task : &main_task;
}