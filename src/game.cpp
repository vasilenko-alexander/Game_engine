#include "../include/engine.hpp"
#include "../include/engine_constants.hpp"
#include "../include/engine_types.hpp"
#include <iostream>

int main(int /*argn*/, char* /*args*/ [])
{
    ge::IEngine* gameEngine = ge::getInstance();
    std::string errMsg      = gameEngine->init_engine(ge::everything);

    if (!errMsg.empty())
    {
        std::cerr << errMsg << std::endl;
        return EXIT_FAILURE;
    }

    bool run_loop = true;
    ge::event event;
    while (run_loop)
    {
        while (gameEngine->read_event(event))
        {
            if (!event.msg.empty())
            {
                std::cout << event.msg << std::endl;
            }
            if (event.type == ge::events_t::shutdown)
            {
                run_loop = false;
                break;
            }
        }
    }
    gameEngine->uninit_engine();
    return EXIT_SUCCESS;
}
