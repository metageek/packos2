#include <qunit.h>

#include <kputs.h>
#include <outport.h>

static const uint16_t debugPort = 0xe9;
static const char CHAR_LOG = 'L';
static const char CHAR_ASSERT = 'A';
static const char CHAR_ASSERT_EXPR = 'e';
static const char CHAR_ASSERT_EXPECTED = 'x';
static const char CHAR_ASSERT_ACTUAL = 'a';
static const char CHAR_QUIT = 'Q';

static void qunitWrite(bool ccConsole,
                       const char* s) {
  const char* cur;
  for (cur = s; *cur; cur++) {
    outb(debugPort, *cur);
  }

  if (ccConsole) {
    kputs(s);
  }
}

static void qunitWriteChar(bool ccConsole,
                           char ch) {
  outb(debugPort, ch);

  if (ccConsole) {
    kputc(ch);
  }
}

static void qunitWriteInt_internal(bool ccConsole,
                                   int i) {
  if (i>0) {
    int lowDigit = i%10;
    qunitWriteInt_internal(ccConsole, i/10);
    qunitWriteChar(ccConsole, lowDigit+'0');
  }
}

static void qunitWriteInt(bool ccConsole,
                          int i) {
  if (i<0) {
    qunitWriteChar(ccConsole, '-');
    i=-i;
  }

  if (i==0) {
    qunitWriteChar(ccConsole, '0');
    return;
  }

  qunitWriteInt_internal(ccConsole, i);
}

void qunitLog(const char* msg) {
  qunitWriteChar(true, CHAR_LOG);
  qunitWriteChar(true, ':');
  qunitWrite(true, msg);
  qunitWriteChar(true, '\n');
}

void qunitWriteAssertionResults(const char* filename,
                                int lineno,
                                const char* expr,
                                const char* expected,
                                const char* actual,
                                bool matched) {
  qunitWriteChar(false, CHAR_ASSERT);
  qunitWriteChar(false, ':');
  qunitWrite(false, filename);
  qunitWriteChar(false, ':');
  qunitWriteInt(false, lineno);
  qunitWriteChar(false, ':');
  qunitWriteChar(false, matched ? 'Y' : 'N');
  qunitWriteChar(false, '\n');

  qunitWriteChar(false, CHAR_ASSERT_EXPR);
  qunitWriteChar(false, ':');
  qunitWrite(false, expr);
  qunitWriteChar(false, '\n');

  qunitWriteChar(false, CHAR_ASSERT_EXPECTED);
  qunitWriteChar(false, ':');
  qunitWrite(false, expected);
  qunitWriteChar(false, '\n');

  qunitWriteChar(false, CHAR_ASSERT_ACTUAL);
  qunitWriteChar(false, ':');
  qunitWrite(false, actual);
  qunitWriteChar(false, '\n');
}

void qunitQuit()
{
  qunitWriteChar(false, CHAR_QUIT);
  qunitWriteChar(false, ':');
  qunitWrite(false, "Quit");
  qunitWriteChar(false, '\n');
}
