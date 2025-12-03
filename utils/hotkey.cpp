#include "hotkey.h"
#include "utilities.h"
#include <winuser.h>

bool hotkey_t::check()
{
    switch (type)
    {
    case always_on:
        return true;

    case hold:
        return (key != 0 && ImGui::IsKeyDown(key));

    case toggle:
    {
        if (key != 0 && utilities::is_key_pressed(key))
            toggle_state = !toggle_state;

        return toggle_state;
    }

    case force_disable:
        return (key != 0 && !ImGui::IsKeyDown(key));
    }

    return false;
}