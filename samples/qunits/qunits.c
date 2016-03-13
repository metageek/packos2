#include "kputs.h"
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
  
  qunitLog("..and now quitting");
  qunitQuit();
}

