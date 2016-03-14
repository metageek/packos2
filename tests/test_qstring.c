#include <qunit.h>
#include <qstring.h>

void TestQStringStrcmp(void) {
  EXPECT_EQ_INT(strcmp("fred", "fred"), 0);
  EXPECT_GT_INT(strcmp("freds", "fred"), 0);
  EXPECT_LT_INT(strcmp("fred", "freds"), 0);
  EXPECT_GT_INT(strcmp("fred", "barney"), 0);
  EXPECT_LT_INT(strcmp("barney", "fred"), 0);
}

