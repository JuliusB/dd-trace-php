#include "compat_zend_string.h"
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_types.h"
#include "php_version.h"

#if PHP_VERSION_ID < 70000
zval *ddtrace_string_tolower(zval *str) {
    if (!str) {
        return NULL;
    }
    zval *ret;
    ALLOC_INIT_ZVAL(ret);

    ZVAL_STRINGL(ret, zend_str_tolower_dup(Z_STRVAL_P(str), Z_STRLEN_P(str)), Z_STRLEN_P(str), 0);
    return ret;
}
#else
zval *ddtrace_string_tolower(zval *str) {
    if (!str || Z_TYPE_P(str) != IS_STRING) {
        return NULL;
    }
    zval *val = emalloc(sizeof(zval));
    *val = EG(uninitialized_zval);
    ZVAL_STR(val, zend_string_tolower(Z_STR_P(str)));
    return val;
}
#endif
