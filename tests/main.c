#include <stdlib.h>
#include <stdio.h>
#include <check.h>


START_TEST(example_test) {
  ck_assert_msg(3 == 4, "Expected 3 to equal 3.");
} END_TEST;

START_TEST(example_test2) {
  ck_assert_msg(3 == 3, "Expected 3 to equal 3.");
} END_TEST;

Suite* testSuite(void) {
  Suite* self = suite_create("Cog Math");
  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, example_test);
  tcase_add_test(tc_core, example_test2);
  suite_add_tcase(self, tc_core);
  return self;
}

int main(int argc, char **argv)
{
  Suite* cogmath = testSuite();
  SRunner* testRunner = srunner_create(cogmath);
  srunner_run_all(testRunner, CK_VERBOSE);
  int number_failed = srunner_ntests_failed (testRunner);
  srunner_free(testRunner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
