#include <core/logger.h>
#include <core/asserts.h>

int main(int argc, char const *argv[]) {
    KFATAL("Test FATAL message!");
    KERROR("Test Error message!");
    KWARN("Test Warning message!");
    KINFO("Test Info message!");
    KDEBUG("Test Debug message!");
    KTRACE("Test Trace message!");

    KASSERT(1 == 0)

    return 0;
}
