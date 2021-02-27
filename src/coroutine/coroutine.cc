#include "timer.h"
#include "coroutine.h"

using study::Coroutine;

Coroutine *Coroutine::current = nullptr;
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE;
long Coroutine::last_cid = 0;

st_coro_on_swap_t Coroutine::on_yield = nullptr;
st_coro_on_swap_t Coroutine::on_resume = nullptr;

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
        // printf("in resume method: co[%ld] end\n", cid);
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
}

static void sleep_timeout(void *param)
{
    ((Coroutine *) param)->resume();
}

int Coroutine::sleep(double seconds)
{
    Coroutine *co = Coroutine::get_current();

    timer_manager.add_timer(seconds * Timer::SECOND, sleep_timeout, (void *) co);

    co->yield();

    return 0;
}

void Coroutine::set_on_yield(st_coro_on_swap_t func)
{
    on_yield = func;
}

void Coroutine::set_on_resume(st_coro_on_swap_t func)
{
    on_resume = func;
}