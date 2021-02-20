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

    return Coroutine::create(create_func, (void *) &php_coro_args);
}

void PhpCoroutine::defer(php_study_fci_fcc *defer_fci_fcc)
{
    php_coro_task *task = (php_coro_task *) get_task();
    
    if (task->defer_tasks == nullptr) {
        task->defer_tasks = new std::stack<php_study_fci_fcc *>;
    }

    task->defer_tasks->push(defer_fci_fcc);
}

int PhpCoroutine::sleep(double seconds)
{
    return Coroutine::sleep(seconds);
}

int PhpCoroutine::scheduler()
{
    uv_loop_t *loop = uv_default_loop();

    if (!study_g.poll) {
        init_st_poll();
    }

    while (loop->stop_flag == 0) {
        int n;
        int timeout;
        epoll_event *events;

        timeout = uv__next_timeout(loop);
        events = study_g.poll->events;
        n = epoll_wait(study_g.poll->epollfd, study_g.poll->events, study_g.poll->ncap, timeout);

        for (int i = 0; i < n; i++) {
            int fd;
            int id;
            struct epoll_event *p = &events[i];
            uint64_t u64 = p->data.u64;
            Coroutine *co;

            fromuint64(u64, &fd, &id);
            co = Coroutine::get_by_cid(id);
            co->resume();
        }

        loop->time = uv__hrtime(UV_CLOCK_FAST) / 1000000;
        uv__run_timers(loop);

        if (uv__next_timeout(loop) < 0 && !study_g.poll) {
            uv_stop(loop);
        }
    }

    free_st_poll();

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

void PhpCoroutine::vm_stack_init(void)
{
    uint32_t size = DEFAULT_PHP_STACK_PAGE_SIZE;
    zend_vm_stack page = (zend_vm_stack) emalloc(size);

    page->top = ZEND_VM_STACK_ELEMENTS(page);
    page->end = (zval *) ((char *) page + size);
    page->prev = NULL;

    EG(vm_stack) = page;
    EG(vm_stack)->top++;
    EG(vm_stack_top) = EG(vm_stack)->top;
    EG(vm_stack_end) = EG(vm_stack)->end;
    EG(vm_stack_page_size) = size;
}

void PhpCoroutine::create_func(void *arg)
{
    int i;
    php_coro_args *php_arg = (php_coro_args *) arg;
    zend_fcall_info_cache fci_cache = *php_arg->fci_cache;
    zend_function *func = fci_cache.function_handler;
    zval *argv = php_arg->argv;
    int argc = php_arg->argc;
    php_coro_task *task;
    zend_execute_data *call;
    zval _retval;
    zval *retval = &_retval;

    vm_stack_init(); // get a new php stack
    call = (zend_execute_data *) (EG(vm_stack_top));
    task = (php_coro_task *) EG(vm_stack_top);
    EG(vm_stack_top) = (zval *) ((char *) call + PHP_CORO_TASK_SLOT * sizeof(zval));

    call = zend_vm_stack_push_call_frame(
        ZEND_CALL_TOP_FUNCTION | ZEND_CALL_ALLOCATED,
        func, argc, fci_cache.called_scope, fci_cache.object
    );

    for (i = 0; i < argc; ++i) {
        zval *param;
        zval *arg = &argv[i];
        param = ZEND_CALL_ARG(call, i + 1);
        ZVAL_COPY(param, arg);
    }

    call->symbol_table = NULL;

    EG(current_execute_data) = call;

    save_vm_stack(task);

    task->co = Coroutine::get_current();
    task->co->set_task((void *) task);
    task->defer_tasks = nullptr;

    if (func->type == ZEND_USER_FUNCTION) {
        ZVAL_UNDEF(retval);
        EG(current_execute_data) = NULL;
        zend_init_func_execute_data(call, &func->op_array, retval);
        zend_execute_ex(EG(current_execute_data));
    }

    task = get_task();
    std::stack<php_study_fci_fcc *> *defer_tasks = task->defer_tasks;

    if (defer_tasks) {
        php_study_fci_fcc *defer_fci_fcc;
        zval result;

        while (!defer_tasks->empty()) {
            defer_fci_fcc = defer_tasks->top();
            defer_tasks->pop();
            defer_fci_fcc->fci.retval = &result;

            if (zend_call_function(&defer_fci_fcc->fci, &defer_fci_fcc->fcc) != SUCCESS) {
                php_error_docref(NULL, E_WARNING, "defer execute error");
                return;
            }

            efree(defer_fci_fcc);
        }

        delete defer_tasks;
        task->defer_tasks = nullptr;
    }

    zval_ptr_dtor(retval);
}