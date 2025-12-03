#include "config_manager.h"

void config_manager::load(std::string name)
{
    try
    {
        std::filesystem::path dir(xorstr("C:/fatalgm/configs"));

        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
            std::filesystem::create_directories(dir);

        std::ifstream file(dir / (name + xorstr(".ftg")));
        if (!file.is_open())
            return;

        nlohmann::json json = nlohmann::json::parse(file);

        file.close();

        nlohmann::json& menu = json[xorstr("settings")][xorstr("menu")];
        menu[xorstr("custom_loading_screen")][xorstr("enable")].get_to(settings::menu::custom_loading_screen::enable);
        strncpy(settings::menu::custom_loading_screen::url, menu[xorstr("custom_loading_screen")][xorstr("url")].get<std::string>().c_str(), sizeof(settings::menu::custom_loading_screen::url));

        menu[xorstr("styles")][xorstr("rounding")].get_to(settings::menu::styles::rounding);
        menu[xorstr("styles")][xorstr("margin")].get_to(settings::menu::styles::margin);
        menu[xorstr("styles")][xorstr("tab_padding")][xorstr("x")].get_to(settings::menu::styles::tab_padding.x);
        menu[xorstr("styles")][xorstr("tab_padding")][xorstr("y")].get_to(settings::menu::styles::tab_padding.y);
        menu[xorstr("styles")][xorstr("win_size")][xorstr("x")].get_to(settings::menu::styles::win_size.x);
        menu[xorstr("styles")][xorstr("win_size")][xorstr("y")].get_to(settings::menu::styles::win_size.y);
        menu[xorstr("styles")][xorstr("scroll_width")].get_to(settings::menu::styles::scroll_width);
        menu[xorstr("styles")][xorstr("play_sfx")].get_to(settings::menu::styles::play_sfx);
        menu[xorstr("styles")][xorstr("anim_speed")].get_to(settings::menu::styles::anim_speed);


        menu[xorstr("colors")][xorstr("window_bg")].get_to(settings::menu::colors::window_bg);
        menu[xorstr("colors")][xorstr("child_bg")].get_to(settings::menu::colors::child_bg);
        menu[xorstr("colors")][xorstr("text_inactive")].get_to(settings::menu::colors::text_inactive);
        menu[xorstr("colors")][xorstr("text")].get_to(settings::menu::colors::text);
        menu[xorstr("colors")][xorstr("selection")].get_to(settings::menu::colors::selection);
        menu[xorstr("colors")][xorstr("selection_high")].get_to(settings::menu::colors::selection_high);
        menu[xorstr("colors")][xorstr("border")].get_to(settings::menu::colors::border);
        menu[xorstr("colors")][xorstr("button")].get_to(settings::menu::colors::button);
        menu[xorstr("colors")][xorstr("button_hover")].get_to(settings::menu::colors::button_hover);
        menu[xorstr("colors")][xorstr("button_clicked")].get_to(settings::menu::colors::button_clicked);

        nlohmann::json& aimbot = json[xorstr("settings")][xorstr("aimbot")];
        aimbot[xorstr("globals")][xorstr("enable")].get_to(settings::aimbot::globals::enable);
        aimbot[xorstr("globals")][xorstr("hotkey")][xorstr("key")].get_to(settings::aimbot::globals::hotkey.key);
        aimbot[xorstr("globals")][xorstr("hotkey")][xorstr("type")].get_to(settings::aimbot::globals::hotkey.type);
        aimbot[xorstr("globals")][xorstr("silent")].get_to(settings::aimbot::globals::silent);
        aimbot[xorstr("globals")][xorstr("automatic_fire")].get_to(settings::aimbot::globals::automatic_fire);
        aimbot[xorstr("globals")][xorstr("penetrate_walls")].get_to(settings::aimbot::globals::penetrate_walls);
        aimbot[xorstr("globals")][xorstr("fov")].get_to(settings::aimbot::globals::fov);
        aimbot[xorstr("globals")][xorstr("hitbox")].get_to(settings::aimbot::globals::hitbox);
        aimbot[xorstr("globals")][xorstr("priority")].get_to(settings::aimbot::globals::priority);

        aimbot[xorstr("accuracy")][xorstr("predict_spread")].get_to(settings::aimbot::accuracy::predict_spread);
        aimbot[xorstr("accuracy")][xorstr("disable_recoil")].get_to(settings::aimbot::accuracy::disable_recoil);
        aimbot[xorstr("accuracy")][xorstr("disable_visual_recoil")].get_to(settings::aimbot::accuracy::disable_visual_recoil);
        aimbot[xorstr("accuracy")][xorstr("backtrack")].get_to(settings::aimbot::accuracy::backtrack);
        aimbot[xorstr("accuracy")][xorstr("smooth")].get_to(settings::aimbot::accuracy::smooth);

        aimbot[xorstr("visuals")][xorstr("fov")].get_to(settings::aimbot::visuals::fov);
        aimbot[xorstr("visuals")][xorstr("snaplines")].get_to(settings::aimbot::visuals::snaplines);

        aimbot[xorstr("visuals")][xorstr("backtrack")][xorstr("enable")].get_to(settings::aimbot::visuals::backtrack::enable);
        aimbot[xorstr("visuals")][xorstr("backtrack")][xorstr("material_type")].get_to(settings::aimbot::visuals::backtrack::material_type);

        aimbot[xorstr("visuals")][xorstr("colors")][xorstr("fov")].get_to(settings::aimbot::visuals::colors::fov);
        aimbot[xorstr("visuals")][xorstr("colors")][xorstr("snaplines")].get_to(settings::aimbot::visuals::colors::snaplines);
        aimbot[xorstr("visuals")][xorstr("colors")][xorstr("backtrack")].get_to(settings::aimbot::visuals::colors::backtrack);

        nlohmann::json& antihit = json[xorstr("settings")][xorstr("antihit")];
        antihit[xorstr("fake_lags")][xorstr("enable")].get_to(settings::antihit::fake_lags::enable);
        antihit[xorstr("fake_lags")][xorstr("count")].get_to(settings::antihit::fake_lags::count);
        antihit[xorstr("fake_lags")][xorstr("method")].get_to(settings::antihit::fake_lags::method);

        antihit[xorstr("fake_angles")][xorstr("enable")].get_to(settings::antihit::fake_angles::enable);
        antihit[xorstr("fake_angles")][xorstr("hotkey")][xorstr("key")].get_to(settings::antihit::fake_angles::hotkey.key);
        antihit[xorstr("fake_angles")][xorstr("hotkey")][xorstr("type")].get_to(settings::antihit::fake_angles::hotkey.type);
        antihit[xorstr("fake_angles")][xorstr("yaw")].get_to(settings::antihit::fake_angles::yaw);
        antihit[xorstr("fake_angles")][xorstr("pitch")].get_to(settings::antihit::fake_angles::pitch);
        antihit[xorstr("fake_angles")][xorstr("at_target")].get_to(settings::antihit::fake_angles::at_target);
        antihit[xorstr("fake_angles")][xorstr("invert_yaw")].get_to(settings::antihit::fake_angles::invert_yaw);
        antihit[xorstr("fake_angles")][xorstr("fake_duck")].get_to(settings::antihit::fake_angles::fake_duck);

        antihit[xorstr("visuals")][xorstr("fake_model")][xorstr("enable")].get_to(settings::antihit::visuals::fake_model::enable);
        antihit[xorstr("visuals")][xorstr("fake_model")][xorstr("material_type")].get_to(settings::antihit::visuals::fake_model::material_type);

        antihit[xorstr("visuals")][xorstr("colors")][xorstr("fake_model")].get_to(settings::antihit::visuals::colors::fake_model);

        nlohmann::json& visuals = json[xorstr("settings")][xorstr("visuals")];
        visuals[xorstr("esp")][xorstr("players")][xorstr("enable")].get_to(settings::visuals::esp::players::enable);
        visuals[xorstr("esp")][xorstr("players")][xorstr("dormant")].get_to(settings::visuals::esp::players::dormant);
        visuals[xorstr("esp")][xorstr("players")][xorstr("box")].get_to(settings::visuals::esp::players::box);
        visuals[xorstr("esp")][xorstr("players")][xorstr("box_outline")].get_to(settings::visuals::esp::players::box_outline);
        visuals[xorstr("esp")][xorstr("players")][xorstr("box_team_col")].get_to(settings::visuals::esp::players::box_team_col);

        visuals[xorstr("esp")][xorstr("players")][xorstr("name")][xorstr("enabled")].get_to(settings::visuals::esp::players::name.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("name")][xorstr("pos")].get_to(settings::visuals::esp::players::name.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("name")][xorstr("team_col")].get_to(settings::visuals::esp::players::name.team_col);

        visuals[xorstr("esp")][xorstr("players")][xorstr("rp_team")][xorstr("enabled")].get_to(settings::visuals::esp::players::rp_team.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("rp_team")][xorstr("pos")].get_to(settings::visuals::esp::players::rp_team.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("rp_team")][xorstr("team_col")].get_to(settings::visuals::esp::players::rp_team.team_col);

        visuals[xorstr("esp")][xorstr("players")][xorstr("user_group")][xorstr("enabled")].get_to(settings::visuals::esp::players::user_group.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("user_group")][xorstr("pos")].get_to(settings::visuals::esp::players::user_group.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("user_group")][xorstr("team_col")].get_to(settings::visuals::esp::players::user_group.team_col);

        visuals[xorstr("esp")][xorstr("players")][xorstr("weapon_name")][xorstr("enabled")].get_to(settings::visuals::esp::players::weapon_name.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("weapon_name")][xorstr("pos")].get_to(settings::visuals::esp::players::weapon_name.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("weapon_name")][xorstr("team_col")].get_to(settings::visuals::esp::players::weapon_name.team_col);

        visuals[xorstr("esp")][xorstr("players")][xorstr("distance")][xorstr("enabled")].get_to(settings::visuals::esp::players::distance.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("distance")][xorstr("pos")].get_to(settings::visuals::esp::players::distance.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("distance")][xorstr("team_col")].get_to(settings::visuals::esp::players::distance.team_col);


        visuals[xorstr("esp")][xorstr("players")][xorstr("render_distance")].get_to(settings::visuals::esp::players::render_distance);

        visuals[xorstr("esp")][xorstr("players")][xorstr("health_text")][xorstr("enabled")].get_to(settings::visuals::esp::players::health_text.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_text")][xorstr("pos")].get_to(settings::visuals::esp::players::health_text.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_text")][xorstr("team_col")].get_to(settings::visuals::esp::players::health_text.team_col);
        
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_bar")].get_to(settings::visuals::esp::players::health_bar);
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_bar_thick")].get_to(settings::visuals::esp::players::health_bar_thick);
        visuals[xorstr("esp")][xorstr("players")][xorstr("armor")][xorstr("enabled")].get_to(settings::visuals::esp::players::armor.enabled);
        visuals[xorstr("esp")][xorstr("players")][xorstr("armor")][xorstr("pos")].get_to(settings::visuals::esp::players::armor.pos);
        visuals[xorstr("esp")][xorstr("players")][xorstr("armor")][xorstr("team_col")].get_to(settings::visuals::esp::players::armor.team_col);

        visuals[xorstr("esp")][xorstr("players")][xorstr("friend_list")].get_to(settings::visuals::esp::players::friend_list);
        visuals[xorstr("esp")][xorstr("players")][xorstr("friend_ignore_aim")].get_to(settings::visuals::esp::players::friend_ignore_aim);
        visuals[xorstr("esp")][xorstr("players")][xorstr("friend_ignore_vis")].get_to(settings::visuals::esp::players::friend_ignore_vis);

        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("box")].get_to(settings::visuals::esp::players::colors::box);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("name")].get_to(settings::visuals::esp::players::colors::name);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("rp_team")].get_to(settings::visuals::esp::players::colors::rp_team);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("user_group")].get_to(settings::visuals::esp::players::colors::user_group);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("weapon_name")].get_to(settings::visuals::esp::players::colors::weapon_name);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("distance")].get_to(settings::visuals::esp::players::colors::distance);

        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("health_text")].get_to(settings::visuals::esp::players::colors::health_text);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("health_bar")].get_to(settings::visuals::esp::players::colors::health_bar);
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("armor")].get_to(settings::visuals::esp::players::colors::armor);

        visuals[xorstr("esp")][xorstr("entity")][xorstr("enable")].get_to(settings::visuals::esp::entity::enable);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("dormant")].get_to(settings::visuals::esp::entity::dormant);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("box")].get_to(settings::visuals::esp::entity::box);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("name")].get_to(settings::visuals::esp::entity::name);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("distance")].get_to(settings::visuals::esp::entity::distance);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("render_distance")].get_to(settings::visuals::esp::entity::render_distance);

        visuals[xorstr("esp")][xorstr("entity")][xorstr("list")].get_to(settings::visuals::esp::entity::list);

        visuals[xorstr("esp")][xorstr("entity")][xorstr("colors")][xorstr("box")].get_to(settings::visuals::esp::entity::colors::box);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("colors")][xorstr("name")].get_to(settings::visuals::esp::entity::colors::name);
        visuals[xorstr("esp")][xorstr("entity")][xorstr("colors")][xorstr("distance")].get_to(settings::visuals::esp::entity::colors::distance);

        visuals[xorstr("chams")][xorstr("players")][xorstr("enable")].get_to(settings::visuals::chams::players::enable);
        visuals[xorstr("chams")][xorstr("players")][xorstr("ignore_walls")].get_to(settings::visuals::chams::players::ignore_walls);
        visuals[xorstr("chams")][xorstr("players")][xorstr("material_type")].get_to(settings::visuals::chams::players::material_type);
        visuals[xorstr("chams")][xorstr("players")][xorstr("draw_original_model")].get_to(settings::visuals::chams::players::draw_original_model);

        visuals[xorstr("chams")][xorstr("entity")][xorstr("enable")].get_to(settings::visuals::chams::entity::enable);
        visuals[xorstr("chams")][xorstr("entity")][xorstr("ignore_walls")].get_to(settings::visuals::chams::entity::ignore_walls);
        visuals[xorstr("chams")][xorstr("entity")][xorstr("material_type")].get_to(settings::visuals::chams::entity::material_type);
        visuals[xorstr("chams")][xorstr("entity")][xorstr("draw_original_model")].get_to(settings::visuals::chams::entity::draw_original_model);

        visuals[xorstr("chams")][xorstr("hands")][xorstr("enable")].get_to(settings::visuals::chams::hands::enable);
        visuals[xorstr("chams")][xorstr("hands")][xorstr("material_type")].get_to(settings::visuals::chams::hands::material_type);
        visuals[xorstr("chams")][xorstr("hands")][xorstr("draw_original_model")].get_to(settings::visuals::chams::hands::draw_original_model);

        visuals[xorstr("chams")][xorstr("colors")][xorstr("players")].get_to(settings::visuals::chams::colors::players);
        visuals[xorstr("chams")][xorstr("colors")][xorstr("entity")].get_to(settings::visuals::chams::colors::entity);
        visuals[xorstr("chams")][xorstr("colors")][xorstr("hands")].get_to(settings::visuals::chams::colors::hands);

        visuals[xorstr("world")][xorstr("fov_changer")][xorstr("enable")].get_to(settings::visuals::world::fov_changer::enable);
        visuals[xorstr("world")][xorstr("fov_changer")][xorstr("value")].get_to(settings::visuals::world::fov_changer::value);
        visuals[xorstr("world")][xorstr("model_fov_changer")][xorstr("enable")].get_to(settings::visuals::world::model_fov_changer::enable);
        visuals[xorstr("world")][xorstr("model_fov_changer")][xorstr("value")].get_to(settings::visuals::world::model_fov_changer::value);
        visuals[xorstr("world")][xorstr("spectator_list")].get_to(settings::visuals::world::spectator_list);

        nlohmann::json& miscellaneous = json[xorstr("settings")][xorstr("miscellaneous")];
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("enable")].get_to(settings::miscellaneous::globals::third_person::enable);
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("hotkey")][xorstr("key")].get_to(settings::miscellaneous::globals::third_person::hotkey.key);
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("hotkey")][xorstr("type")].get_to(settings::miscellaneous::globals::third_person::hotkey.type);
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("distance")].get_to(settings::miscellaneous::globals::third_person::distance);

        miscellaneous[xorstr("movement")][xorstr("bhop")].get_to(settings::miscellaneous::movement::bhop);
        miscellaneous[xorstr("movement")][xorstr("air_strafe")].get_to(settings::miscellaneous::movement::air_strafe);

        nlohmann::json& lua = json[xorstr("settings")][xorstr("lua")];
        lua[xorstr("miscellaneous")][xorstr("dumper")].get_to(settings::lua::miscellaneous::dumper);
        lua[xorstr("miscellaneous")][xorstr("net_logger")].get_to(settings::lua::miscellaneous::net_logger);
    }
    catch (const std::exception& e)
    {

    }
}

void config_manager::save(std::string name)
{
    try
    {
        if (name.empty())
            return;

        nlohmann::json json;

        nlohmann::json& menu = json[xorstr("settings")][xorstr("menu")];
        menu[xorstr("custom_loading_screen")][xorstr("enable")] = settings::menu::custom_loading_screen::enable;
        menu[xorstr("custom_loading_screen")][xorstr("url")] = settings::menu::custom_loading_screen::url;

        menu[xorstr("styles")][xorstr("rounding")] = settings::menu::styles::rounding;
        menu[xorstr("styles")][xorstr("margin")] = settings::menu::styles::margin;
        menu[xorstr("styles")][xorstr("tab_padding")][xorstr("x")] = settings::menu::styles::tab_padding.x;
        menu[xorstr("styles")][xorstr("tab_padding")][xorstr("y")] = settings::menu::styles::tab_padding.y;
        menu[xorstr("styles")][xorstr("win_size")][xorstr("x")] = settings::menu::styles::win_size.x;
        menu[xorstr("styles")][xorstr("win_size")][xorstr("y")] = settings::menu::styles::win_size.y;
        menu[xorstr("styles")][xorstr("scroll_width")] = settings::menu::styles::scroll_width;
        menu[xorstr("styles")][xorstr("play_sfx")] = settings::menu::styles::play_sfx;
        menu[xorstr("styles")][xorstr("anim_speed")] = settings::menu::styles::anim_speed;

        menu[xorstr("colors")][xorstr("window_bg")] = settings::menu::colors::window_bg;
        menu[xorstr("colors")][xorstr("child_bg")] = settings::menu::colors::child_bg;
        menu[xorstr("colors")][xorstr("text_inactive")] = settings::menu::colors::text_inactive;
        menu[xorstr("colors")][xorstr("text")] = settings::menu::colors::text;
        menu[xorstr("colors")][xorstr("selection")] = settings::menu::colors::selection;
        menu[xorstr("colors")][xorstr("selection_high")] = settings::menu::colors::selection_high;
        menu[xorstr("colors")][xorstr("border")] = settings::menu::colors::border;
        menu[xorstr("colors")][xorstr("button")] = settings::menu::colors::button;
        menu[xorstr("colors")][xorstr("button_hover")] = settings::menu::colors::button_hover;
        menu[xorstr("colors")][xorstr("button_clicked")] = settings::menu::colors::button_clicked;

        nlohmann::json& aimbot = json[xorstr("settings")][xorstr("aimbot")];
        aimbot[xorstr("globals")][xorstr("enable")] = settings::aimbot::globals::enable;
        aimbot[xorstr("globals")][xorstr("hotkey")][xorstr("key")] = settings::aimbot::globals::hotkey.key;
        aimbot[xorstr("globals")][xorstr("hotkey")][xorstr("type")] = settings::aimbot::globals::hotkey.type;
        aimbot[xorstr("globals")][xorstr("enable")] = settings::aimbot::globals::enable;
        aimbot[xorstr("globals")][xorstr("silent")] = settings::aimbot::globals::silent;
        aimbot[xorstr("globals")][xorstr("automatic_fire")] = settings::aimbot::globals::automatic_fire;
        aimbot[xorstr("globals")][xorstr("penetrate_walls")] = settings::aimbot::globals::penetrate_walls;
        aimbot[xorstr("globals")][xorstr("fov")] = settings::aimbot::globals::fov;
        aimbot[xorstr("globals")][xorstr("hitbox")] = settings::aimbot::globals::hitbox;
        aimbot[xorstr("globals")][xorstr("priority")] = settings::aimbot::globals::priority;

        aimbot[xorstr("accuracy")][xorstr("predict_spread")] = settings::aimbot::accuracy::predict_spread;
        aimbot[xorstr("accuracy")][xorstr("disable_recoil")] = settings::aimbot::accuracy::disable_recoil;
        aimbot[xorstr("accuracy")][xorstr("disable_visual_recoil")] = settings::aimbot::accuracy::disable_visual_recoil;
        aimbot[xorstr("accuracy")][xorstr("backtrack")] = settings::aimbot::accuracy::backtrack;
        aimbot[xorstr("accuracy")][xorstr("smooth")] = settings::aimbot::accuracy::smooth;

        aimbot[xorstr("visuals")][xorstr("fov")] = settings::aimbot::visuals::fov;
        aimbot[xorstr("visuals")][xorstr("snaplines")] = settings::aimbot::visuals::snaplines;

        aimbot[xorstr("visuals")][xorstr("backtrack")][xorstr("enable")] = settings::aimbot::visuals::backtrack::enable;
        aimbot[xorstr("visuals")][xorstr("backtrack")][xorstr("material_type")] = settings::aimbot::visuals::backtrack::material_type;

        aimbot[xorstr("visuals")][xorstr("colors")][xorstr("fov")] = settings::aimbot::visuals::colors::fov;
        aimbot[xorstr("visuals")][xorstr("colors")][xorstr("snaplines")] = settings::aimbot::visuals::colors::snaplines;
        aimbot[xorstr("visuals")][xorstr("colors")][xorstr("backtrack")] = settings::aimbot::visuals::colors::backtrack;

        nlohmann::json& antihit = json[xorstr("settings")][xorstr("antihit")];
        antihit[xorstr("fake_lags")][xorstr("enable")] = settings::antihit::fake_lags::enable;
        antihit[xorstr("fake_lags")][xorstr("count")] = settings::antihit::fake_lags::count;
        antihit[xorstr("fake_lags")][xorstr("method")] = settings::antihit::fake_lags::method;

        antihit[xorstr("fake_angles")][xorstr("enable")] = settings::antihit::fake_angles::enable;
        antihit[xorstr("fake_angles")][xorstr("hotkey")][xorstr("key")] = settings::antihit::fake_angles::hotkey.key;
        antihit[xorstr("fake_angles")][xorstr("hotkey")][xorstr("type")] = settings::antihit::fake_angles::hotkey.type;
        antihit[xorstr("fake_angles")][xorstr("yaw")] = settings::antihit::fake_angles::yaw;
        antihit[xorstr("fake_angles")][xorstr("pitch")] = settings::antihit::fake_angles::pitch;
        antihit[xorstr("fake_angles")][xorstr("at_target")] = settings::antihit::fake_angles::at_target;
        antihit[xorstr("fake_angles")][xorstr("invert_yaw")] = settings::antihit::fake_angles::invert_yaw;
        antihit[xorstr("fake_angles")][xorstr("fake_duck")] = settings::antihit::fake_angles::fake_duck;

        antihit[xorstr("visuals")][xorstr("fake_model")][xorstr("enable")] = settings::antihit::visuals::fake_model::enable;
        antihit[xorstr("visuals")][xorstr("fake_model")][xorstr("material_type")] = settings::antihit::visuals::fake_model::material_type;

        antihit[xorstr("visuals")][xorstr("colors")][xorstr("fake_model")] = settings::antihit::visuals::colors::fake_model;

        nlohmann::json& visuals = json[xorstr("settings")][xorstr("visuals")];
        visuals[xorstr("esp")][xorstr("players")][xorstr("enable")] = settings::visuals::esp::players::enable;
        visuals[xorstr("esp")][xorstr("players")][xorstr("dormant")] = settings::visuals::esp::players::dormant;
        visuals[xorstr("esp")][xorstr("players")][xorstr("box")] = settings::visuals::esp::players::box;
        visuals[xorstr("esp")][xorstr("players")][xorstr("box_outline")] = settings::visuals::esp::players::box_outline;
        visuals[xorstr("esp")][xorstr("players")][xorstr("box_team_col")] = settings::visuals::esp::players::box_team_col;

        visuals[xorstr("esp")][xorstr("players")][xorstr("name")][xorstr("enabled")] = settings::visuals::esp::players::name.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("name")][xorstr("pos")] = settings::visuals::esp::players::name.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("name")][xorstr("team_col")] = settings::visuals::esp::players::name.team_col;

        visuals[xorstr("esp")][xorstr("players")][xorstr("rp_team")][xorstr("enabled")] = settings::visuals::esp::players::rp_team.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("rp_team")][xorstr("pos")] = settings::visuals::esp::players::rp_team.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("rp_team")][xorstr("team_col")] = settings::visuals::esp::players::rp_team.team_col;

        visuals[xorstr("esp")][xorstr("players")][xorstr("user_group")][xorstr("enabled")] = settings::visuals::esp::players::user_group.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("user_group")][xorstr("pos")] = settings::visuals::esp::players::user_group.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("user_group")][xorstr("team_col")] = settings::visuals::esp::players::user_group.team_col;

        visuals[xorstr("esp")][xorstr("players")][xorstr("weapon_name")][xorstr("enabled")] = settings::visuals::esp::players::weapon_name.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("weapon_name")][xorstr("pos")] = settings::visuals::esp::players::weapon_name.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("weapon_name")][xorstr("team_col")] = settings::visuals::esp::players::weapon_name.team_col;
        
        visuals[xorstr("esp")][xorstr("players")][xorstr("distance")][xorstr("enabled")] = settings::visuals::esp::players::distance.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("distance")][xorstr("pos")] = settings::visuals::esp::players::distance.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("distance")][xorstr("team_col")] = settings::visuals::esp::players::distance.team_col;

        visuals[xorstr("esp")][xorstr("players")][xorstr("render_distance")] = settings::visuals::esp::players::render_distance;

        visuals[xorstr("esp")][xorstr("players")][xorstr("health_text")][xorstr("enabled")] = settings::visuals::esp::players::health_text.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_text")][xorstr("pos")] = settings::visuals::esp::players::health_text.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_text")][xorstr("team_col")] = settings::visuals::esp::players::health_text.team_col;
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_bar")] = settings::visuals::esp::players::health_bar;
        visuals[xorstr("esp")][xorstr("players")][xorstr("health_bar_thick")] = settings::visuals::esp::players::health_bar_thick;
        visuals[xorstr("esp")][xorstr("players")][xorstr("armor")][xorstr("enabled")] = settings::visuals::esp::players::armor.enabled;
        visuals[xorstr("esp")][xorstr("players")][xorstr("armor")][xorstr("pos")] = settings::visuals::esp::players::armor.pos;
        visuals[xorstr("esp")][xorstr("players")][xorstr("armor")][xorstr("team_col")] = settings::visuals::esp::players::armor.team_col;

        visuals[xorstr("esp")][xorstr("players")][xorstr("friend_list")] = {};
        for (const auto& [key, value] : settings::visuals::esp::players::friend_list.items())
            if (value.get<bool>())
                visuals[xorstr("esp")][xorstr("players")][xorstr("friend_list")][key] = value;

        visuals[xorstr("esp")][xorstr("players")][xorstr("friend_ignore_aim")] = settings::visuals::esp::players::friend_ignore_aim;
        visuals[xorstr("esp")][xorstr("players")][xorstr("friend_ignore_vis")] = settings::visuals::esp::players::friend_ignore_vis;

        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("box")] = settings::visuals::esp::players::colors::box;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("name")] = settings::visuals::esp::players::colors::name;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("rp_team")] = settings::visuals::esp::players::colors::rp_team;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("user_group")] = settings::visuals::esp::players::colors::user_group;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("weapon_name")] = settings::visuals::esp::players::colors::weapon_name;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("distance")] = settings::visuals::esp::players::colors::distance;

        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("health_text")] = settings::visuals::esp::players::colors::health_text;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("health_bar")] = settings::visuals::esp::players::colors::health_bar;
        visuals[xorstr("esp")][xorstr("players")][xorstr("colors")][xorstr("armor")] = settings::visuals::esp::players::colors::armor;

        visuals[xorstr("esp")][xorstr("entity")][xorstr("enable")] = settings::visuals::esp::entity::enable;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("dormant")] = settings::visuals::esp::entity::dormant;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("box")] = settings::visuals::esp::entity::box;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("name")] = settings::visuals::esp::entity::name;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("distance")] = settings::visuals::esp::entity::distance;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("render_distance")] = settings::visuals::esp::entity::render_distance;

        visuals[xorstr("esp")][xorstr("entity")][xorstr("list")] = {};

        for (const auto& [key, value] : settings::visuals::esp::entity::list.items())
            if (value.get<bool>())
                visuals[xorstr("esp")][xorstr("entity")][xorstr("list")][key] = value;

        visuals[xorstr("esp")][xorstr("entity")][xorstr("colors")][xorstr("box")] = settings::visuals::esp::entity::colors::box;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("colors")][xorstr("name")] = settings::visuals::esp::entity::colors::name;
        visuals[xorstr("esp")][xorstr("entity")][xorstr("colors")][xorstr("distance")] = settings::visuals::esp::entity::colors::distance;

        visuals[xorstr("chams")][xorstr("players")][xorstr("enable")] = settings::visuals::chams::players::enable;
        visuals[xorstr("chams")][xorstr("players")][xorstr("ignore_walls")] = settings::visuals::chams::players::ignore_walls;
        visuals[xorstr("chams")][xorstr("players")][xorstr("material_type")] = settings::visuals::chams::players::material_type;
        visuals[xorstr("chams")][xorstr("players")][xorstr("draw_original_model")] = settings::visuals::chams::players::draw_original_model;

        visuals[xorstr("chams")][xorstr("entity")][xorstr("enable")] = settings::visuals::chams::entity::enable;
        visuals[xorstr("chams")][xorstr("entity")][xorstr("ignore_walls")] = settings::visuals::chams::entity::ignore_walls;
        visuals[xorstr("chams")][xorstr("entity")][xorstr("material_type")] = settings::visuals::chams::entity::material_type;
        visuals[xorstr("chams")][xorstr("entity")][xorstr("draw_original_model")] = settings::visuals::chams::entity::draw_original_model;
        
        visuals[xorstr("chams")][xorstr("hands")][xorstr("enable")] = settings::visuals::chams::hands::enable;
        visuals[xorstr("chams")][xorstr("hands")][xorstr("material_type")] = settings::visuals::chams::hands::material_type;
        visuals[xorstr("chams")][xorstr("hands")][xorstr("draw_original_model")] = settings::visuals::chams::hands::draw_original_model;

        visuals[xorstr("chams")][xorstr("colors")][xorstr("players")] = settings::visuals::chams::colors::players;
        visuals[xorstr("chams")][xorstr("colors")][xorstr("entity")] = settings::visuals::chams::colors::entity;
        visuals[xorstr("chams")][xorstr("colors")][xorstr("hands")] = settings::visuals::chams::colors::hands;

        visuals[xorstr("world")][xorstr("fov_changer")][xorstr("enable")] = settings::visuals::world::fov_changer::enable;
        visuals[xorstr("world")][xorstr("fov_changer")][xorstr("value")] = settings::visuals::world::fov_changer::value;
        visuals[xorstr("world")][xorstr("model_fov_changer")][xorstr("enable")] = settings::visuals::world::model_fov_changer::enable;
        visuals[xorstr("world")][xorstr("model_fov_changer")][xorstr("value")] = settings::visuals::world::model_fov_changer::value;

        visuals[xorstr("world")][xorstr("spectator_list")] = settings::visuals::world::spectator_list;

        nlohmann::json& miscellaneous = json[xorstr("settings")][xorstr("miscellaneous")];
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("enable")] = settings::miscellaneous::globals::third_person::enable;
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("hotkey")][xorstr("key")] = settings::miscellaneous::globals::third_person::hotkey.key;
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("hotkey")][xorstr("type")] = settings::miscellaneous::globals::third_person::hotkey.type;
        miscellaneous[xorstr("globals")][xorstr("third_person")][xorstr("distance")] = settings::miscellaneous::globals::third_person::distance;

        miscellaneous[xorstr("movement")][xorstr("bhop")] = settings::miscellaneous::movement::bhop;
        miscellaneous[xorstr("movement")][xorstr("air_strafe")] = settings::miscellaneous::movement::air_strafe;

        nlohmann::json& lua = json[xorstr("settings")][xorstr("lua")];
        lua[xorstr("miscellaneous")][xorstr("dumper")] = settings::lua::miscellaneous::dumper;
        lua[xorstr("miscellaneous")][xorstr("net_logger")] = settings::lua::miscellaneous::net_logger;

        std::filesystem::path dir(xorstr("C:/fatalgm/configs"));

        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
            std::filesystem::create_directories(dir);

        std::ofstream file(dir / (name + xorstr(".ftg")));
        if (!file.is_open())
            return;

        file << json.dump(4);
        file.close();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}