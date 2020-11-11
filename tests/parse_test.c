#include <stdlib.h>

#include "munit/munit.h"
#include "parse.h"
#include "io.h"


/**
 * Test get_token(char **input);
 * Strategy:
 *     number of tokens: 0, 1, >1
 *     number of characters in token: 1, >1
 *     position of tokens: start, ending, between spaces
 *     num of consecutive spaces: 1, >1
 */


/* Get_tokens: single token single character no spaces */

void* test_get_token_single_token_one_char_no_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) "a";
}

MunitResult test_get_token_single_token_one_char_no_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char *input = (char *) fixture;
    munit_assert_string_equal("a", get_token(&input));

    return MUNIT_OK;
}

/* Get_tokens: single token multiple character no spaces */

void* test_get_token_single_token_no_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) "cat";
}

MunitResult test_get_token_single_token_no_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char *input = (char *) fixture;
    munit_assert_string_equal("cat", get_token(&input));

    return MUNIT_OK;
}

/* Get_tokens: no token */

void* test_get_token_no_token_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) " ";
}

MunitResult test_get_token_no_token(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char *input = (char *) fixture;
    munit_assert_null(get_token(&input));

    return MUNIT_OK;
}

/* Get_tokens: single token with spaces */

void* test_get_token_single_token_with_spaces_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) "  cat   ";
}

MunitResult test_get_token_single_token_with_spaces(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char *input = (char *) fixture;
    munit_assert_string_equal("cat", get_token(&input));
    munit_assert_char(' ', ==, input[0]); /* input ptr left after token */
    munit_assert_null(get_token(&input));

    return MUNIT_OK;
}

/* Get_tokens: multiple tokens */

void* test_get_token_multiple_tokens_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) "a b";
}

MunitResult test_get_token_multiple_tokens(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    char *input = (char *) fixture;
    munit_assert_string_equal("a", get_token(&input));
    munit_assert_string_equal("b", get_token(&input));

    return MUNIT_OK;
}



/**
 * Test parse_command(char *input);
 * Strategy:
 *     number of tokens: 1, >1
 *     io_redirection: no, in, out, both, syntax_error
 *     position of io_redirection: interleaved, at end
 */


/**
 * Tear down function for parse_tokens tests.
 */
void test_parse_command_tear_down(
        void* fixture) {
    command_t *command = (command_t *) fixture;
    clear_tokens(command->tokens);
}


/* Parse_command: single token */

void* test_parse_command_single_token_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_command("cat");
}

MunitResult test_parse_command_single_token(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    command_t *command = (command_t *) fixture;
    munit_assert_string_equal("cat", command->tokens[0]);

    return MUNIT_OK;
}

/* Parse_command: multiple tokens */

void* test_parse_command_multiple_tokens_no_io_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_command("echo hello");
}

MunitResult test_parse_command_multiple_tokens_no_io(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    command_t *command = (command_t *) fixture;
    munit_assert_string_equal("echo", command->tokens[0]);
    munit_assert_string_equal("hello", command->tokens[1]);

    return MUNIT_OK;
}

/* Parse_command: multiple tokens with io */

void* test_parse_command_multiple_tokens_with_io_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_command("echo hello < filename");
}

MunitResult test_parse_command_multiple_tokens_with_io(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    command_t *command = (command_t *) fixture;
    munit_assert_string_equal("echo", command->tokens[0]);
    munit_assert_string_equal("hello", command->tokens[1]);
    munit_assert_null(command->tokens[2]);

    return MUNIT_OK;
}

/* Parse_command: io redirection syntax error */

void* test_parse_command_io_redirection_syntax_error_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_command("echo hello < ");
}

MunitResult test_parse_command_io_redirection_syntax_error(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    command_t *command = (command_t *) fixture;
    munit_assert_null(command);

    return MUNIT_OK;
}

/* Parse_command: no pipeline, empty next_command */

void* test_parse_command_no_pipeline_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_command("cat");
}

MunitResult test_parse_command_no_pipeline(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    command_t *command = (command_t *) fixture;
    munit_assert_null(command->next_command);

    return MUNIT_OK;
}

/* Parse_command: single pipeline two commands*/

void* test_parse_command_single_pipeline_setup(
        const MunitParameter params[] MUNIT_UNUSED,
        void* user_data MUNIT_UNUSED) {
    return (void *) parse_command("echo 3 | grep 3");
}

MunitResult test_parse_command_single_pipeline(
        const MunitParameter params[] MUNIT_UNUSED, void* fixture) {
    command_t *first_command = (command_t *) fixture;
    command_t *second_command = first_command->next_command;

    munit_assert_string_equal("echo", first_command->tokens[0]);
    munit_assert_string_equal("grep", second_command->tokens[0]);
    munit_assert_null(second_command->next_command);

    return MUNIT_OK;
}


/** Parse token Test Suite **/


MunitTest parse_tests[] = {
    {
        "/test_get_token_single_token_one_char_no_spaces", /* name */
        test_get_token_single_token_one_char_no_spaces,      /* test */
        test_get_token_single_token_one_char_no_spaces_setup, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_get_token_single_token_no_spaces", /* name */
        test_get_token_single_token_no_spaces,      /* test */
        test_get_token_single_token_no_spaces_setup, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_get_token_no_token", /* name */
        test_get_token_no_token,      /* test */
        test_get_token_no_token_setup, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_get_token_single_token_with_spaces", /* name */
        test_get_token_single_token_with_spaces,      /* test */
        test_get_token_single_token_with_spaces_setup, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_get_token_multiple_tokens", /* name */
        test_get_token_multiple_tokens,      /* test */
        test_get_token_multiple_tokens_setup, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_command_single_token", /* name */
        test_parse_command_single_token,      /* test */
        test_parse_command_single_token_setup, /* setup */
        test_parse_command_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_command_multiple_tokens_no_io", /* name */
        test_parse_command_multiple_tokens_no_io,      /* test */
        test_parse_command_multiple_tokens_no_io_setup, /* setup */
        test_parse_command_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_command_multiple_tokens_with_io", /* name */
        test_parse_command_multiple_tokens_with_io,      /* test */
        test_parse_command_multiple_tokens_with_io_setup, /* setup */
        test_parse_command_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_command_io_redirection_syntax_error", /* name */
        test_parse_command_io_redirection_syntax_error,      /* test */
        test_parse_command_io_redirection_syntax_error_setup, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_command_no_pipeline", /* name */
        test_parse_command_no_pipeline,      /* test */
        test_parse_command_no_pipeline_setup, /* setup */
        test_parse_command_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/test_parse_command_single_pipeline", /* name */
        test_parse_command_single_pipeline,      /* test */
        test_parse_command_single_pipeline_setup, /* setup */
        test_parse_command_tear_down, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    /* Mark end of array with null test function */
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite parse_suite MUNIT_UNUSED = {
    "/parse_test_suite", /* name */
    parse_tests, /* tests */
    NULL, /* suites */
    1, /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

