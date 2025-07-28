#include <RCNET/RCNET_engine.h>
#include <RCNET/RCNET_logger.h>
#include "server.h"

int main(int argc, char* argv[])
{
    rcnet_logger_log(RCNET_LOG_INFO, "Server Started\n");

#ifdef NDEBUG // If we are in Release mode
    // Keep only error and critical logs
    rcnet_logger_setPriority(RCNET_LOG_ERROR);
#else // If we are in Debug mode
    // Show all log levels
    rcnet_logger_setPriority(RCNET_LOG_DEBUG);
#endif

    RCNET_Callbacks myServerCallbacks;
    myServerCallbacks.rcnet_unload = rcnet_unload;
    myServerCallbacks.rcnet_load = rcnet_load;
    myServerCallbacks.rcnet_update = rcnet_update;

    return rcnet_engine_run(&myServerCallbacks, 60);
}