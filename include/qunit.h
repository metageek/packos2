#ifndef _QUNIT_H_
#define _QUNIT_H_

#include <types.h>
#include <qstring.h>

void qunitLog(const char* msg);
void qunitWriteAssertionResults(const char* filename,
                                int lineno,
                                const char* expr,
                                const char* expected,
                                const char* actual,
                                bool matched);
void qunitQuit();

#define EXPECT_TRUE(EXPR) { bool _value = (EXPR); \
    qunitWriteAssertionResults(__FILE__,                       \
                               __LINE__,                       \
                               #EXPR,                          \
                               "true",                         \
                               _value ? "true" : "false",      \
                               _value);}
#define EXPECT_FALSE(EXPR) { bool _value = (EXPR); \
    qunitWriteAssertionResults(__FILE__,                       \
                               __LINE__,                       \
                               #EXPR,                          \
                               "false",                        \
                               _value ? "true" : "false",      \
                               !_value);}

#define EXPECT_EQ_INT(ACTUAL, EXPECTED) {   \
  int _expected = (EXPECTED),               \
  _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             itoa(_actual), \
                             _actual == _expected);}

#define EXPECT_LT_INT(ACTUAL, EXPECTED) {   \
  int _expected = (EXPECTED),               \
  _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             itoa(_actual), \
                             _actual < _expected);}

#define EXPECT_GT_INT(ACTUAL, EXPECTED) {   \
  int _expected = (EXPECTED),               \
  _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             itoa(_actual), \
                             _actual > _expected);}

#define EXPECT_LE_INT(ACTUAL, EXPECTED) {   \
  int _expected = (EXPECTED),               \
  _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             itoa(_actual), \
                             _actual <= _expected);}

#define EXPECT_GE_INT(ACTUAL, EXPECTED) {   \
  int _expected = (EXPECTED),               \
  _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             itoa(_actual), \
                             _actual >= _expected);}

#define EXPECT_EQ_STR(ACTUAL, EXPECTED) {   \
    const char* _expected = (EXPECTED);     \
  const char* _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             _actual, \
                             strcmp(_actual, _expected) == 0);}

#define EXPECT_LT_STR(ACTUAL, EXPECTED) {   \
  const char* _expected = (EXPECTED);               \
  const char* _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             _actual, \
                             strcmp(_actual, _expected) < 0);}

#define EXPECT_GT_STR(ACTUAL, EXPECTED) {   \
  const char* _expected = (EXPECTED);               \
  const char* _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             _actual, \
                             strcmp(_actual, _expected) > 0);}

#define EXPECT_LE_STR(ACTUAL, EXPECTED) {   \
  const char* _expected = (EXPECTED);               \
  const char* _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             _actual, \
                             strcmp(_actual, _expected) <= 0);}

#define EXPECT_GE_STR(ACTUAL, EXPECTED) {   \
  const char* _expected = (EXPECTED);               \
  const char* _actual = (ACTUAL);                       \
  qunitWriteAssertionResults(__FILE__,      \
                             __LINE__,      \
                             #ACTUAL,       \
                             #EXPECTED,     \
                             _actual, \
                             strcmp(_actual, _expected) >= 0);}

#endif /*_QUNIT_H_*/
