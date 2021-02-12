#include "coroutine.h"

using study::Coroutine;

Coroutine *Coroutine::current = nullptr;

void *Coroutine::get_task()
{
    return task;
}

void *Coroutine::get_current_task()
{
    return Coroutine::current ? Coroutine::current->get_task() : nullptr;
}
