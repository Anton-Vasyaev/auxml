#pragma once

#include "callisto/framework/native/platform_detect.h"

#ifdef CALLISTO_OS_FAMILY_NT
    #define CALLISTO_PYTHON_CALL __stdcall
#endif


#ifdef CALLISTO_OS_FAMILY_UNIX
    #define CALLISTO_PYTHON_CALL __cdecl
#endif