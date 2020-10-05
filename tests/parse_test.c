#include <stdlib.h>

#include "munit/munit.h"
#include "parse.h"

/* Parse_tokens: single tokens no spaces test functions */

void test_parse_token_single_token_no_spaces_tear_down(
        void* fixture) {
    char **command = (char **) fixture;

    clear_tokens(command);
}

void* test_parse_token_single_token_no_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_tokens("cat");
}

MunitResult test_parse_token_single_token_no_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture MUNIT_UNUSED) {
    char **command = (char **) fixture;
    char *parsed_token = "cat";
    munit_assert_string_equal(parsed_token, command[0]);

    return MUNIT_OK;
}



/** Parse token Test Suite **/


MunitTest parse_token_tests[] = {
    {
        "/test_parse_token_single_token_no_spaces", /* name */
        test_parse_token_single_token_no_spaces,      /* test */
        test_parse_token_single_token_no_spaces_setup, /* setup */
        test_parse_token_single_token_no_spaces_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    /* Mark end of array with null test function */
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite parse_suite MUNIT_UNUSED = {
    "/tests_parse_tokens", /* name */
    parse_token_tests, /* tests */
    NULL, /* suites */
    1, /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

