#include <stdlib.h>

#include "munit/munit.h"
#include "parse.h"

/**
 * Test parse_token(char *input);
 * Strategy:
 *     number of tokens: 1, >1
 *     number of characters in token: 1, >1
 *     position of tokens: start, ending, between spaces
 *     num of consecutive spaces: 1, >1
 */


/**
 * Tear down function for parse_tokens tests.
 */
void test_parse_token_tear_down(
        void* fixture) {
    char **command = (char **) fixture;
    clear_tokens(command);
}


/* Parse_tokens: single token single character no spaces */

void* test_parse_token_single_token_one_char_no_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_tokens("a");
}

MunitResult test_parse_token_single_token_one_char_no_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char **command = (char **) fixture;
    char *parsed_token = "a";
    munit_assert_string_equal(parsed_token, command[0]);

    return MUNIT_OK;
}

/* Parse_tokens: single token no spaces */

void* test_parse_token_single_token_no_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_tokens("cat");
}

MunitResult test_parse_token_single_token_no_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char **command = (char **) fixture;
    char *parsed_token = "cat";
    munit_assert_string_equal(parsed_token, command[0]);

    return MUNIT_OK;
}

/* Parse_tokens: single token with spaces */

void* test_parse_token_single_token_with_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_tokens("   cat  ");
}

MunitResult test_parse_token_single_token_with_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char **command = (char **) fixture;
    char *parsed_token = "cat";
    munit_assert_string_equal(parsed_token, command[0]);

    return MUNIT_OK;
}

/* Parse_tokens: two tokens with spaces */

void* test_parse_token_two_tokens_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_tokens("a b");
}

MunitResult test_parse_token_two_tokens(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char **command = (char **) fixture;
    char *first_token = "a";
    char *second_token = "b";
    munit_assert_string_equal(first_token, command[0]);
    munit_assert_string_equal(second_token, command[1]);

    return MUNIT_OK;
}

/* Parse_tokens: general case */

void* test_parse_token_general_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_tokens("grep --version linux");
}

MunitResult test_parse_token_general(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char **command = (char **) fixture;
    char *mid_token = "--version";
    munit_assert_string_equal(mid_token, command[1]);

    return MUNIT_OK;
}


/** Parse token Test Suite **/


MunitTest parse_token_tests[] = {
    {
        "/test_parse_token_single_token_one_char_no_spaces", /* name */
        test_parse_token_single_token_one_char_no_spaces,      /* test */
        test_parse_token_single_token_one_char_no_spaces_setup, /* setup */
        test_parse_token_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_token_single_token_no_spaces", /* name */
        test_parse_token_single_token_no_spaces,      /* test */
        test_parse_token_single_token_no_spaces_setup, /* setup */
        test_parse_token_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_token_single_token_with_spaces", /* name */
        test_parse_token_single_token_with_spaces,      /* test */
        test_parse_token_single_token_with_spaces_setup, /* setup */
        test_parse_token_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_token_two_tokens", /* name */
        test_parse_token_two_tokens,      /* test */
        test_parse_token_two_tokens_setup, /* setup */
        test_parse_token_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_token_general", /* name */
        test_parse_token_general,      /* test */
        test_parse_token_general_setup, /* setup */
        test_parse_token_tear_down, /* tear_down */
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

