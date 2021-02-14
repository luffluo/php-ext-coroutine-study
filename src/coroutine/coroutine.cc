#include "coroutine.h"

using study::Coroutine;

Coroutine *Coroutine::current = nullptr;
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE;
long Coroutine::last_cid = 0;

long Coroutine::create(coroutine_func_t fn, void *args)
{
    return (new Coroutine(fn, args))->run();
}

void *Coroutine::get_task()
{
    return task;
}

void Coroutine::set_task(void *_task)
{
    task = _task;
}

void *Coroutine::get_current_task()
{
    return Coroutine::current ? Coroutine::current->get_task() : nullptr;
}

Coroutine *Coroutine::get_current()
{
    return current;
}

void Coroutine::yield()
{
    current = origin;
    ctx.swap_out();
}

void Coroutine::resume()
{
    origin = current;
    current = this;
    ctx.swap_in();

    if (ctx.is_end()) {
        cid = current->get_cid();
        printf("in resume method: co[%ld] end\n", cid);
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
}

static void sleep_timeout(uv_timer_t *timer)
{
    ((Coroutine *) timer->data)->resume();
}

int Coroutine::sleep(double seconds)
{
    Coroutine *co = Coroutine::get_current();

    uv_timer_t timer;
    timer.data = co;

    uv_timer_init(uv_default_loop(), &timer);
    uv_timer_start(&timer, sleep_timeout, seconds * 1000, 0);

    co->yield();

    return 0;
}