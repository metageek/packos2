#include <qunit.h>
#include <qstring.h>

void TestQStringStrcmp(void) {
  EXPECT_EQ_INT(strcmp("fred", "fred"), 0);
  EXPECT_GT_INT(strcmp("freds", "fred"), 0);
  EXPECT_LT_INT(strcmp("fred", "freds"), 0);
  EXPECT_GT_INT(strcmp("fred", "barney"), 0);
  EXPECT_LT_INT(strcmp("barney", "fred"), 0);
}

void TestQStringAtoi(void) {
  EXPECT_EQ_INT(atoi("0"), 0);
  EXPECT_EQ_INT(atoi("-0"), 0);
  EXPECT_EQ_INT(atoi("17"), 17);
  EXPECT_EQ_INT(atoi("-17"), -17);
  EXPECT_EQ_INT(atoi("-17.5"), 0);
}

void TestQStringItoa(void) {
  EXPECT_EQ_STR(itoa(17), "17")
  EXPECT_EQ_STR(itoa(0), "0")
  EXPECT_EQ_STR(itoa(-17), "-17");
}
