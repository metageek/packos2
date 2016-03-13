#include <qstring.h>
#include <types.h>

int atoi(const char* s) {
  if (!(s && s[0])) {
    return 0;
  }

  int sign = 1;
  if (s[0] == '-') {
    sign = -1;
    s++;
    if (!(*s)) {
      return 0;
    }
  }

  int res = 0;

  while (*s) {
    res *= 10;
    if ((*s < '0') || (*s > '9')) {
      return 0;
    }
    res += (*s)-'0';
    s++;
  }

  return res*sign;
}

static void itoa_fillbuff(int i, char* buff, int reverse, int* len) {
  int digit = i%10;
  int rest = i/10;
  if (rest) {
    itoa_fillbuff(rest, buff, reverse+1, len);
  } else {
    (*len) = reverse+1;
  }
  buff[(*len)-reverse-1] = '0'+digit;
}

const char* itoa(int i) {
  static char buff[40];
  char* s = buff;
  int len = 0;
  if (i < 0) {
    buff[0] = '-';
    i=-i;
    s++;
  }
  itoa_fillbuff(i, s, 0, &len);
  return buff;
}
