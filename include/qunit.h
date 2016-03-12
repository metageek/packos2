#ifndef _QUNIT_H_
#define _QUNIT_H_

#include <types.h>

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

#endif /*_QUNIT_H_*/
