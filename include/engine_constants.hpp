#pragma once

#include <string>

#define DECL_STR_CONST(name) const std::string name = #name;

namespace ge
{
    /**
     * engine initialize constants
     */
    DECL_STR_CONST(video)
    DECL_STR_CONST(audio)
    DECL_STR_CONST(timer)
    DECL_STR_CONST(joystick)
    DECL_STR_CONST(gamecontroller)
    DECL_STR_CONST(everything)
    DECL_STR_CONST(events)
    DECL_STR_CONST(haptic)
}

#undef DECL_STR_CONST
