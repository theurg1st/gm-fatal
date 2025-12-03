#pragma once
#include "sdk/interfaces.h"

namespace fonts {
    inline ImFont* josefin_sans_regular;
    inline ImFont* josefin_sans_header;
    inline ImFont* font_icon;
    inline ImFont* esp_font;
}

namespace globals
{
    inline bool unload = false;

    inline v_panel mat_system_top_panel = false;
    inline v_panel overlay_popup_panel = false;
    inline v_panel focus_overlay_panel = false;

    inline D3DMATRIX world_to_screen;

    inline c_vector view_origin;
    inline q_angle view_angles;
    inline float fov = 0;

    inline void* menu_panel = nullptr;
    inline std::vector<std::string> waiting_to_be_executed;
}

namespace settings
{
    namespace menu
    {
        inline bool opened = false;

        namespace custom_loading_screen
        {
            inline bool enable = true;
            inline char url[256] = "https://dev.pavetr.ru/uploads/fatal_load.html";
        }

        namespace styles
        {
            inline float rounding = 4.f;
            inline float margin = 10.f;
            inline ImVec2 tab_padding = { 10.f,10.f };
            inline ImVec2 win_size = { 848.f,588.f };
            inline ImVec2 win_size_ex = { 848.f,588.f }; // Equals to win_size in code, needs for DPI scaling
            inline float scroll_width = 12.5f;
            inline float play_sfx = 1.f;
            inline float anim_speed = 0.03f;
        }

        namespace colors
        {
            inline float window_bg[4] = { 0.066f, 0.059f, 0.141f, 1.f };
            inline float child_bg[4] = { 0.098f, 0.086f, 0.208f, 1.f };
            inline float text_inactive[4] = { 1.f, 1.f, 1.f, 0.5f };
            inline float text[4] = { 1.f, 1.f, 1.f, 1.f };
            inline float selection[4] = { 0.761f, 0.09f, 0.314f, 1.f };
            inline float selection_high[4] = { 0.82f, 0.106f, 0.345f, 1.f };
            inline float transp[4] = { 0.f, 0.f, 0.f, 0.f };
            inline float border[4] = { 1.f, 1.f, 1.f, 0.1f };
            inline float button[4] = { 0.118f, 0.098f, 0.243f, 1.f };
            inline float button_hover[4] = { 0.145f, 0.122f, 0.302f, 1.f };
            inline float button_clicked[4] = { 0.169f, 0.141f, 0.349f, 1.f };

            inline float frame_bg_hover[4] = { window_bg[0] * 1.1f, window_bg[1] * 1.1f, window_bg[2] * 1.1f, 1.f };
            inline float frame_bg_active[4] = { frame_bg_hover[0] * 1.5f, frame_bg_hover[1] * 1.5f, frame_bg_hover[2] * 1.5f, 1.f };

            inline float outline[4] = { 0.f, 0.f, 0.f, 1.f };
        }
    }

    namespace rage {
        // Aimbot
        inline int hitchance = 100;
        inline int pointscale = 34;
        inline int mindamage = 100;

        inline int hitbox = 0;
        inline int multipoint = 0;
        inline int targetsel = 0;

        inline bool autostop = false;
        inline hotkey_t autostop_bind;

        inline bool autoscope = false;
        inline bool ignore_limbs = false;
        inline bool autorevolver = false;

        inline bool aimbot = false;
        inline bool silent = false;
        inline int maxfov = 0;

        inline bool autofire = false;
        inline bool delay_shot = false;
        inline bool duck_peek = false;
        inline bool force_bodyaim = false;
        inline bool force_shoot = false;
        inline bool headshot_only = false;

        inline bool knifebot = false;
        inline bool zeusbot = false;

        inline bool nospread = false;
        inline hotkey_t nospread_bind;

        inline bool doubletap = false;
        inline hotkey_t doubletap_bind;

        // Anti-aim
        inline bool aa_enable = false;
        inline int pitch = 0;
        inline int pitch_custom = 0;
        inline int yaw = 0;
        inline int yaw_custom = 0;
        inline bool hideshot = false;

        inline bool fake_enable = false;
        inline int fake_mode = 0;
        inline int fake_limit = 0;
    }

    namespace aimbot
    {
        namespace globals
        {
            inline bool enable = false;
            inline hotkey_t hotkey;
            inline bool silent = false;
            inline bool automatic_fire = false;
            inline bool penetrate_walls = false;
            inline float fov = 0;
            inline int hitbox = 0;
            inline int priority = 0;
        }

        namespace accuracy
        {
            inline bool predict_spread = false;
            inline bool disable_recoil = false;
            inline bool disable_visual_recoil = false;
            inline float backtrack = 0.f;
            inline float smooth = 0.f;
        }

        namespace visuals
        {
            inline bool fov = false;
            inline bool snaplines = false;

            namespace backtrack
            {
                inline bool enable = false;
                inline int material_type = 0;
            }

            namespace colors
            {
                inline float fov[4] = { 1.f, 1.f, 1.f, 1.f };
                inline float snaplines[4] = { 1.f, 1.f, 1.f, 1.f };
                inline float backtrack[4] = { 1.f, 1.f, 1.f, 1.f };
            }
        }
    }

    namespace antihit
    {
        namespace fake_lags
        {
            inline bool enable = false;
            inline int count = 1;
            inline int method = 0;
        }

        namespace fake_angles
        {
            inline bool enable = false;
            inline hotkey_t hotkey;
            inline int yaw = 0;
            inline int pitch = 0;
            inline bool at_target = false;
            inline bool invert_yaw = false;
            inline bool fake_duck = false;
        }

        namespace visuals
        {
            namespace fake_model
            {
                inline bool enable = false;
                inline int material_type = 0;
            }

            namespace colors
            {
                inline float fake_model[4] = { 1.f, 1.f, 1.f, 1.f };
            }
        }
    }

    namespace visuals
    {
        namespace esp
        {
            namespace players
            {
                inline bool enable = false;
                inline bool dormant = false;
                inline bool box = false;
                inline bool box_outline = false;
                inline bool box_team_col = false;
                inline visual_t name;
                inline visual_t rp_team;
                inline visual_t user_group;
                inline visual_t weapon_name;
                inline visual_t distance;
                inline int render_distance = 15000;

                inline visual_t health_text;
                inline bool health_bar = false;
                inline float health_bar_thick = 1.f;
                
                inline visual_t armor;
                inline bool skeleton = false;

                inline nlohmann::json friend_list;
                inline bool friend_ignore_aim = true;
                inline bool friend_ignore_vis = false;

                namespace colors
                {
                    inline float box[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float name[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float rp_team[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float user_group[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float weapon_name[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float distance[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float health_text[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float health_bar[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float armor[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float skeleton[4] = { 1.f, 1.f, 1.f, 1.f };
                }
            }

            namespace entity
            {
                inline bool enable = false;
                inline bool dormant = false;
                inline bool box = false;
                inline bool name = false;
                inline bool distance = false;
                inline int render_distance = 15000;

                inline bool health = false;
                inline bool health_text = true;
                inline bool health_bar = false;

                inline nlohmann::json list;

                namespace colors
                {
                    inline float box[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float name[4] = { 1.f, 1.f, 1.f, 1.f };
                    inline float distance[4] = { 1.f, 1.f, 1.f, 1.f };

                    inline float health[4] = { 1.f, 1.f, 1.f, 1.f };
                }
            }
        }

        namespace chams
        {
            namespace players
            {
                inline bool enable = false;
                inline bool ignore_walls = false;
                inline int material_type = 0;

                inline bool draw_original_model = false;
            }

            namespace entity
            {
                inline bool enable = false;
                inline bool ignore_walls = false;
                inline int material_type = 0;

                inline bool draw_original_model = false;
            }

            namespace hands
            {
                inline bool enable = false;
                inline int material_type = 0;

                inline bool draw_original_model = false;
            }

            namespace colors
            {
                inline float players[4] = { 1.f, 1.f, 1.f, 1.f };
                inline float entity[4] = { 1.f, 1.f, 1.f, 1.f };
                inline float hands[4] = { 1.f, 1.f, 1.f, 1.f };
            }
        }

        namespace world
        {
            inline bool fullbright = false;
            inline bool spectator_list = false;
            inline bool keybind_list = false;

            namespace fov_changer
            {
                inline bool enable = false;
                inline float value = 80.f;
            }

            namespace model_fov_changer
            {
                inline bool enable = false;
                inline float value = 80.f;
            }
        }
    }

    namespace miscellaneous
    {
        namespace globals
        {
            namespace third_person
            {
                inline bool enable = false;
                inline hotkey_t hotkey;
                inline int distance = 100;
            }
        }

        namespace movement
        {
            inline bool bhop = false;
            inline bool air_strafe = false;

            inline bool air_stuck = false;
            inline hotkey_t air_stuck_key;
            inline int air_stuck_ticks = 2;
        }

        inline int dpi_scale = 1;
    }

    namespace lua
    {
        namespace miscellaneous
        {
            inline bool dumper = false;
            inline bool net_logger = false;
        }
    }
}