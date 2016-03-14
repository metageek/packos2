#include "kputs.h"
#include <qstring.h>
#include <qunit.h>
#include "asm.h"

asmlinkage void kernelMain(void)
{
  qunitLog("Samples of QUnit assertions");

  qunitLog("This EXPECT_TRUE should pass");
  EXPECT_TRUE(5==5);
  qunitLog("This EXPECT_TRUE should fail");
  EXPECT_TRUE(5==17);

  qunitLog("This EXPECT_FALSE should pass");
  EXPECT_FALSE(5==17);
  qunitLog("This EXPECT_FALSE should fail");
  EXPECT_FALSE(5==5);
  
  qunitLog("This EXPECT_EQ_INT should pass");
  EXPECT_EQ_INT(5, 5);
  qunitLog("This EXPECT_EQ_INT should fail");
  EXPECT_EQ_INT(17, 5);
  
  qunitLog("This EXPECT_LT_INT should pass");
  EXPECT_LT_INT(4, 5);
  qunitLog("This EXPECT_LT_INT should fail");
  EXPECT_LT_INT(5, 5);
  qunitLog("This EXPECT_LT_INT should fail");
  EXPECT_LT_INT(17, 5);
  
  qunitLog("This EXPECT_LE_INT should pass");
  EXPECT_LE_INT(4, 5);
  qunitLog("This EXPECT_LE_INT should pass");
  EXPECT_LE_INT(5, 5);
  qunitLog("This EXPECT_LE_INT should fail");
  EXPECT_LE_INT(17, 5);
  
  qunitLog("This EXPECT_GT_INT should pass");
  EXPECT_GT_INT(5, 4);
  qunitLog("This EXPECT_GT_INT should fail");
  EXPECT_GT_INT(5, 5);
  qunitLog("This EXPECT_GT_INT should fail");
  EXPECT_GT_INT(5, 17);
  
  qunitLog("This EXPECT_GE_INT should pass");
  EXPECT_GE_INT(5, 4);
  qunitLog("This EXPECT_GE_INT should pass");
  EXPECT_GE_INT(5, 5);
  qunitLog("This EXPECT_GE_INT should fail");
  EXPECT_GE_INT(5, 17);
  
  qunitLog("This EXPECT_EQ_STR should pass");
  EXPECT_EQ_STR("fred", "fred");
  qunitLog("This EXPECT_EQ_STR should fail");
  EXPECT_EQ_STR("freds", "fred");
  
  qunitLog("This EXPECT_LT_STR should pass");
  EXPECT_LT_STR("barney", "fred");
  qunitLog("This EXPECT_LT_STR should fail");
  EXPECT_LT_STR("fred", "fred");
  qunitLog("This EXPECT_LT_STR should fail");
  EXPECT_LT_STR("freds", "fred");
  
  qunitLog("This EXPECT_LE_STR should pass");
  EXPECT_LE_STR("barney", "fred");
  qunitLog("This EXPECT_LE_STR should pass");
  EXPECT_LE_STR("fred", "fred");
  qunitLog("This EXPECT_LE_STR should fail");
  EXPECT_LE_STR("freds", "fred");
  
  qunitLog("This EXPECT_GT_STR should pass");
  EXPECT_GT_STR("fred", "barney");
  qunitLog("This EXPECT_GT_STR should fail");
  EXPECT_GT_STR("fred", "fred");
  qunitLog("This EXPECT_GT_STR should fail");
  EXPECT_GT_STR("fred", "freds");
  
  qunitLog("This EXPECT_GE_STR should pass");
  EXPECT_GE_STR("fred", "barney");
  qunitLog("This EXPECT_GE_STR should pass");
  EXPECT_GE_STR("fred", "fred");
  qunitLog("This EXPECT_GE_STR should fail");
  EXPECT_GE_STR("fred", "freds");
  
  qunitLog("..and now quitting");
  qunitQuit();
}

