#include <system/error.h>

void error(const char* reason)
{
    (void)printf("Fatal error: %s\n", reason);
}
