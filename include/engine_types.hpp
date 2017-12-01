#pragma once

#include <string>

namespace ge
{
    enum class keys
    {
        nobutton,
        up,
        down,
        left,
        right,
        pause,
        select,
        button1,
        button2
    };

    enum class events_t
    {
        noevent,
        pressed,
        released,
        shutdown
    };

    struct event
    {
        std::string msg;
        events_t type = events_t::noevent;
        keys key      = keys::nobutton;
    };
}
