#include "../include/engine.hpp"
#include "../include/SDL.h"
#include "../include/engine_constants.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace ge
{
    struct bind_event
    {
        bind_event(Uint32 _sdl_type, events_t _type, std::string _event_str)
            : sdl_type(_sdl_type), type(_type), event_str(_event_str)
        {
        }

        Uint32 sdl_type;
        events_t type;
        std::string event_str;
    };

    struct bind_key
    {
        bind_key(SDL_Keycode sdl_k, keys _key, std::string key_s)
            : sdl_key(sdl_k), key(_key), key_str(key_s)
        {
        }
        SDL_Keycode sdl_key;
        keys key;
        std::string key_str;
    };

    class Engine : public IEngine
    {
        SDL_Window* window = nullptr;

        const std::map<std::string, uint> defined_options{
            { ge::timer, SDL_INIT_TIMER },
            { ge::audio, SDL_INIT_AUDIO },
            { ge::video, SDL_INIT_VIDEO },
            { ge::events, SDL_INIT_EVENTS },
            { ge::joystick, SDL_INIT_JOYSTICK },
            { ge::gamecontroller, SDL_INIT_GAMECONTROLLER },
            { ge::haptic, SDL_INIT_HAPTIC },
            { ge::everything, SDL_INIT_EVERYTHING }
        };

        const std::vector<bind_event> defined_events{
            bind_event{ SDL_QUIT, events_t::shutdown, "shutdown" },
            bind_event{ SDL_KEYUP, events_t::released, "released" },
            bind_event{ SDL_KEYDOWN, events_t::pressed, "pressed" }
        };

        const std::vector<bind_key> defined_keys{
            bind_key{ SDLK_UP, keys::up, "up" },
            bind_key{ SDLK_DOWN, keys::down, "down" },
            bind_key{ SDLK_LEFT, keys::left, "left" },
            bind_key{ SDLK_RIGHT, keys::right, "right" },
            bind_key{ SDLK_SPACE, keys::pause, "pause" },
            bind_key{ SDLK_ESCAPE, keys::select, "select" },
            bind_key{ SDLK_a, keys::button1, "button1" },
            bind_key{ SDLK_d, keys::button2, "button2" }
        };

    public:
        Engine();
        std::string init_engine(std::string init_options) override;
        bool read_event(event& event) override;
        void uninit_engine() override;

    private:
        uint parseWndOptions(std::string init_options);
        bind_key* check_input(SDL_Keycode check_code);
        bind_event* check_event(Uint32 check_event);
    };

    Engine::Engine()
    {
    }

    bind_event* Engine::check_event(Uint32 check_event)
    {
        const auto it = std::find_if(
            defined_events.begin(),
            defined_events.end(),
            [&](const bind_event& b_e) { return b_e.sdl_type == check_event; });

        if (it != defined_events.end())
        {
            return new bind_event{ it->sdl_type, it->type, it->event_str };
        }

        return nullptr;
    }

    bind_key* Engine::check_input(SDL_Keycode check_code)
    {
        const auto it = std::find_if(
            defined_keys.begin(), defined_keys.end(), [&](const bind_key& b_k) {
                return b_k.sdl_key == check_code;
            });

        if (it != defined_keys.end())
        {
            return new bind_key{ it->sdl_key, it->key, it->key_str };
        }

        return nullptr;
    }

    uint Engine::parseWndOptions(std::string init_options)
    {
        uint flags = 0;
        try
        {

            char delim = ' ';
            std::istringstream strStream(init_options);
            std::string option;
            while (std::getline(strStream, option, delim))
            {
                std::transform(
                    option.begin(), option.end(), option.begin(), ::tolower);
                flags |= defined_options.at(option);
            }

            return flags;
        }
        catch (const std::out_of_range& ex)
        {
            std::cerr << "Some err is occurred: " << ex.what() << std::endl;
            return flags;
        }
    }

    std::string Engine::init_engine(std::string init_options)
    {
        SDL_version linked_lib = { 0, 0, 0 };
        SDL_GetVersion(&linked_lib);
        if (SDL_COMPILEDVERSION != SDL_VERSIONNUM(linked_lib.major,
                                                  linked_lib.minor,
                                                  linked_lib.patch))
        {
            std::cerr << "SDL compiled and linked libs are mismatch"
                      << std::endl;
        }

        std::stringstream errMsg;
        uint wndFlags      = parseWndOptions(init_options);
        const int init_res = SDL_Init(wndFlags);

        if (init_res != 0)
        {
            errMsg << "SDL_INIT failed " << SDL_GetError() << std::endl;
            return errMsg.str();
        }

        const char* title_wnd = "SDL window";

        window = SDL_CreateWindow(title_wnd,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  640,
                                  480,
                                  ::SDL_WINDOW_OPENGL);

        if (window == nullptr)
        {
            errMsg << "Window creating failed " << SDL_GetError() << std::endl;
            return errMsg.str();
        }
        return errMsg.str();
    }

    bool Engine::read_event(event& e)
    {
        e             = event();
        bool hasEvent = false;
        std::string event_msg;
        SDL_Event sdl_event;

        if (SDL_PollEvent(&sdl_event))
        {
            hasEvent           = true;
            bind_event* bind_e = check_event(sdl_event.type);
            if (bind_e == nullptr)
            {
                return hasEvent;
            }

            bind_key* bind_k = nullptr;
            std::stringstream sstr;
            switch (sdl_event.type)
            {
                case SDL_QUIT:
                    sstr << bind_e->event_str;
                    e.msg  = sstr.str();
                    e.type = bind_e->type;
                    break;
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                    bind_k = check_input(sdl_event.key.keysym.sym);
                    if (bind_k == nullptr)
                    {
                        return hasEvent;
                    }
                    sstr << bind_k->key_str << "_" << bind_e->event_str;
                    e.msg  = sstr.str();
                    e.type = bind_e->type;
                    e.key  = bind_k->key;
                    break;
                default:
                    break;
            }
        }

        return hasEvent;
    }

    void Engine::uninit_engine()
    {
        if (window != nullptr)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        SDL_Quit();
    }

    IEngine* getInstance()
    {
        static Engine engine_inst;
        return &engine_inst;
    }

    IEngine::~IEngine()
    {
    }
}
