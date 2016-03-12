#include "kputs.h"
#include <qunit.h>
#include "asm.h"

asmlinkage void kernelMain(void)
{
  kputs("kputs!\n");
  kputs("and again\n");
  qunitQuit();
}

