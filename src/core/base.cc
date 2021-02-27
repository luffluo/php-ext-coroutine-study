#include "log.h"
#include "timer.h"
#include "study.h"
#include "coroutine.h"

using namespace std;

using study::Timer;
using study::Coroutine;
using study::TimerManager;
using study::timer_manager;

st_global_t study_g;

int init_st_poll()
{
    try {
        study_g.poll = new st_poll_t();
    } catch (const std::exception& e) {
        st_error("%s", e.what());
    }

    study_g.poll = (st_poll_t *) malloc(sizeof(st_poll_t));

    if (study_g.poll == NULL) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }

    study_g.poll->epollfd = epoll_create(256);
    if (study_g.poll->epollfd < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        delete study_g.poll;
        study_g.poll = nullptr;
        return -1;
    }

    study_g.poll->ncap = 16;
    try {
        study_g.poll->events = new epoll_event[study_g.poll->ncap]();
    } catch(const std::bad_alloc& e) {
        st_error("%s", e.what());
    }
    
    study_g.poll->event_num = 0;

    return 0;
}

int free_st_poll()
{
    if (close(study_g.poll->epollfd) < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }

    delete[] study_g.poll->events;
    study_g.poll->events = nullptr;
    delete study_g.poll;
    study_g.poll = nullptr;

    return 0;
}

int st_event_init()
{
    if (!study_g.poll) {
        init_st_poll();
    }

    study_g.running = 1;

    return 0;
}

int st_event_wait()
{
    st_event_init();

    while (study_g.running > 0) {
        int n;
        int timeout;
        epoll_event *events;

        timeout = timer_manager.get_next_timeout();
        events = study_g.poll->events;
        n = epoll_wait(study_g.poll->epollfd, events, study_g.poll->ncap, timeout);

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

        timer_manager.run_timers();

        if (timer_manager.get_next_timeout() < 0 && study_g.poll->event_num == 0) {
            study_g.running = 0;
        }
    }

    st_event_free();

    return 0;
}

int st_event_free()
{
    study_g.running = 0;
    free_st_poll();

    return 0;
}