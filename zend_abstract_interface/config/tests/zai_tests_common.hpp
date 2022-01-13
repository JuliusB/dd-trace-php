#ifndef HAVE_ZAI_TESTS_COMMON_HPP
#define HAVE_ZAI_TESTS_COMMON_HPP
#include "zai_sapi/testing/catch2.hpp"
#include <cstdio>
#include <cstring>

typedef enum {
    EXT_CFG_FOO_BOOL,
    EXT_CFG_FOO_DOUBLE,
    EXT_CFG_FOO_INT,
    EXT_CFG_FOO_MAP,
    EXT_CFG_FOO_STRING,
    EXT_CFG_BAR_ALIASED_INT,
    EXT_CFG_BAZ_MAP_EMPTY,
} ext_cfg_id;

static PHP_MINIT_FUNCTION(zai_config_env) {
    zai_string_view aliases_int[] = {ZAI_STRL_VIEW("BAR_ALIASED_INT_OLD"), ZAI_STRL_VIEW("BAR_ALIASED_INT_OLDER"), ZAI_STRL_VIEW("BAR_ALIASED_INT_OLDEST")};
    zai_config_entry entries[] = {
        EXT_CFG_ENTRY(FOO_BOOL, BOOL, "1"),
        EXT_CFG_ENTRY(FOO_DOUBLE, DOUBLE, "0.5"),
        EXT_CFG_ENTRY(FOO_INT, INT, "42"),
        EXT_CFG_ENTRY(FOO_MAP, MAP, "one:1,two:2"),
        EXT_CFG_ENTRY(FOO_STRING, STRING, "foo string"),
        EXT_CFG_ALIASED_ENTRY(BAR_ALIASED_INT, INT, "0", aliases_int),
        EXT_CFG_ENTRY(BAZ_MAP_EMPTY, MAP, ""),
    };
    if (!zai_config_minit(entries, (sizeof entries / sizeof entries[0]), NULL, 0)) {
        return FAILURE;
    }
    return SUCCESS;
}

#define TEST_BODY(...)         \
{                              \
    REQUIRE(zai_sapi_sinit()); \
    ext_zai_config_ctor(&zai_sapi_extension, PHP_MINIT(zai_config_env)); \
    REQUIRE(zai_sapi_minit()); \
    { __VA_ARGS__ }            \
    zai_sapi_mshutdown();      \
    zai_sapi_sshutdown();      \
}

#define TEST_DEFAULT(description, ...) ZAI_SAPI_TEST_CASE_BARE("config/default", description, TEST_BODY(__VA_ARGS__))
#define TEST_ENV(description, ...)     ZAI_SAPI_TEST_CASE_BARE("config/env", description, TEST_BODY(__VA_ARGS__))
#define TEST_ID(description, ...)      ZAI_SAPI_TEST_CASE_BARE("config/id", description, TEST_BODY(__VA_ARGS__))
#endif
