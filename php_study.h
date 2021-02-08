/* study extension for PHP */

#ifndef PHP_STUDY_H
# define PHP_STUDY_H

extern zend_module_entry study_module_entry;
# define phpext_study_ptr &study_module_entry

# define PHP_STUDY_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_STUDY)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_STUDY_H */

