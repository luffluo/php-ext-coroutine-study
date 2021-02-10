#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H

#include "php_study.h"

namespace study {
    class PhpCoroutine {
        static long create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv);
    };
}

#endif /* end STUDY_COROUTINE_H */