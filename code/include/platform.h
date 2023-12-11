#ifndef PLATFORM_H
#define PLATFORM_H

#define MAX_LOGBUFFER_STRING_LENGTH 512

struct window_dimensions {
    u32 Width  = 0;
    u32 Height = 0;
};

struct timing_management {
    s64 PerfCountFreq;
    s64 CounterElapsed;
    SYSTEMTIME LocalTime;
};

struct debug_info {
    char GlobalLogBuffer[MAX_LOGBUFFER_STRING_LENGTH];
    char ConsoleDebugBuffer[MAX_LOGBUFFER_STRING_LENGTH];
};

global_variable timing_management GlobalTime;
global_variable window_dimensions WindowDimensions;
global_variable debug_info DebugInfo;
global_variable HDC WindowDeviceContext;
global_variable HGLRC RenderingContextHandle;

#endif PLATFORM_H