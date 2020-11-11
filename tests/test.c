#include "test.h"


/* Global symbols */
 struct shell_status STATUS;


int main(int argc, char* const argv[]) {
    int suites_failed = 0;

    /* Run parse_test */
    suites_failed += munit_suite_main(&parse_suite, NULL, argc, argv);
    return (suites_failed == 0) ? 0 : 1;
}
