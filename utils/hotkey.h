#pragma once
#include "../dependencies/imgui/imgui.h"

struct hotkey_t
{
    ImGuiKey key;

    enum type
    {
        always_on,
        hold,
        toggle,
        force_disable
    } type = hold;

    bool check();

private:
    bool toggle_state = false;
};