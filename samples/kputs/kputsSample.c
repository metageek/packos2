#include "kputs.h"
#include "asm.h"

asmlinkage void kernelMain(void)
{
  kputs("kputs!");
  kterpri();
  kputs("and again");
}

