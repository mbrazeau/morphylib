#include <stdio.h>

#define pfail printf("[  FAIL  ] %s, line: %i in: %s\n", __FUNCTION__, __LINE__, __FILE__)
#define ppass printf("[  PASS  ] %s\n", __FUNCTION__)
#define psumf(...) printf("[  ** FAILED ** ] %i times. Review output to see details.\n", fails)
#define psump printf("[  ** PASSED ** ] All tests passed.\n")

