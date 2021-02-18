#include "log.h"
#include "error.h"

const char *st_strerror(int code)
{
    switch (code) {
        case ST_ERROR_SESSION_CLOSED_BY_SERVER:
            return "Session closed by server";

        case ST_ERROR_SESSION_CLOSED_BY_CLIENT:
            return "Session closed by client";

        default:
            snprintf(st_error, sizeof(st_error), "Unknow error: %d", code);
            return st_error;
    }
}