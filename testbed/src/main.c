#include <core/logger.h>
#include <core/asserts.h>

#include "platform/platform.h"

int main(int argc, char const *argv[]) {
    KFATAL("Test FATAL message!");
    KERROR("Test Error message!");
    KWARN("Test Warning message!");
    KINFO("Test Info message!");
    KDEBUG("Test Debug message!");
    KTRACE("Test Trace message!");

    platform_state state;
    if (platform_startup(&state, "Kohi Engine Testbed", 100, 100, 1280, 720)) {
        while (TRUE) {
            platform_pump_messages(&state);
        }
    }

    platform_shutdown(&state);

    return 0;
}
