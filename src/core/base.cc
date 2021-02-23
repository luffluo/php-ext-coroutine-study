#include "uv.h"
#include "log.h"
#include "study.h"
#include "coroutine.h"

using study::Coroutine;

st_global_t study_g;

int init_st_poll()
{
    size_t size;

    study_g.poll = (st_poll_t *) malloc(sizeof(st_poll_t));

    if (study_g.poll == NULL) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }

    study_g.poll->epollfd = epoll_create(256);
    if (study_g.poll->epollfd < 0) {
        st_warn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }

    study_g.poll->ncap = 16;
    size = sizeof(struct epoll_event) * study_g.poll->ncap;
    study_g.poll->events = (struct epoll_event *) malloc(size);
    memset(study_g.poll->events, 0, size);

    return 0;
}

int free_st_poll()
{
    free(study_g.poll->events);
    free(study_g.poll);

    return 0;
}

int st_event_init()
{
    if (!study_g.poll) {
        init_st_poll();
    }

    return 0;
}

typedef enum {
    UV_CLOCK_PRECISE = 0,  /* Use the highest resolution clock available. */
    UV_CLOCK_FAST = 1      /* Use the fastest clock with <= 1ms granularity. */
} uv_clocktype_t;

extern "C" void uv__run_timers(uv_loop_t* loop);
extern "C" uint64_t uv__hrtime(uv_clocktype_t type);
extern "C" int uv__next_timeout(const uv_loop_t* loop);

int st_event_wait()
{
    uv_loop_t *loop = uv_default_loop();

    if (!study_g.poll) {
        st_error("Need to call st_event_init first.");
    }

    while (loop->stop_flag == 0) {
        int n;
        int timeout;
        epoll_event *events;

        timeout = uv__next_timeout(loop);
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

        loop->time = uv__hrtime(UV_CLOCK_FAST) / 1000000;
        uv__run_timers(loop);

        if (uv__next_timeout(loop) < 0 && !study_g.poll) {
            uv_stop(loop);
        }
    }

    st_event_free();

    return 0;
}

int st_event_free()
{
    free_st_poll();

    return 0;
}