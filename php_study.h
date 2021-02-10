/* study extension for PHP */

#ifndef PHP_STUDY_H
# define PHP_STUDY_H

# include "php.h"
# include "php_ini.h"
# include "ext/standard/info.h"

# include "study.h"

# define PHP_STUDY_VERSION "0.1.0"

extern zend_module_entry study_module_entry;
# define phpext_study_ptr &study_module_entry

PHP_FUNCTION(study_test1);
void study_coroutine_util_init();

# if defined(ZTS) && defined(COMPILE_DL_STUDY)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

/**
 * Declare any global variables you may need between the BEGIN and END macros here
 */
ZEND_BEGIN_MODULE_GLOBALS(study)

ZEND_END_MODULE_GLOBALS(study)

#endif	/* PHP_STUDY_H */

