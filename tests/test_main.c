#include <qunit.h>
#include <asm.h>

#include "test_qstring.h"

asmlinkage void kernelMain(void)
{
  qunitLog("Test suite starting");

  TestQStringStrcmp();
  TestQStringAtoi();
  TestQStringItoa();

  qunitQuit();
}
