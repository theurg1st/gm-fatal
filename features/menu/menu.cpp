#include "menu.h"
#include "../lua/lua.h"

#include "../../utils/texture.hpp"
#include <shellapi.h>

const char* tabs[6] = {
        ICON_FA_BOMB " RAGE",
        ICON_FA_CROSSHAIRS " LEGIT",
        ICON_FA_EYE " VISUALS",
        ICON_FA_COG " MISC",
        ICON_FA_CODE " LUA",
        ICON_FA_PAINT_BRUSH " LISTS"
};

float dpi_scales_num[] = { 0.85f, 1.f, 1.25f, 1.5f, 1.75f, 2.f };
float checkboxSizeEx = 16.f;


int curtab = 0;

std::unordered_map<const char*, std::pair<int, ImVec2>> child_labels;

void add_child_label(ImVec2 child_pos, const char* label, int tab) {
    ImVec2 label_size = ImGui::CalcTextSize(label);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + label_size[1] / 2);
    child_labels[label] = { tab, child_pos };
}

void draw_over_items() {
    for (const auto& item : child_labels) {
        if (item.second.first == curtab) {
            const char* text = item.first;
            ImVec2 pos = item.second.second;

            ImVec2 label_size = ImGui::CalcTextSize(text);
            if (curtab < 3) {
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos[0], pos[1] - label_size[1]), ImGui::GetColorU32(ImGuiCol_Text), text);
            }
            else {
                ImGui::GetForegroundDrawList()->AddText(ImVec2(pos[0], pos[1] - label_size[1]), ImGui::GetColorU32(ImGuiCol_Text), text);
            }
        }
    }
}

void imgui_hotkey(const char* label, hotkey_t* hk, const ImVec2& size_arg = ImVec2(0, 0))
{
    if (ImGui::BeginPopupContextItem(label))
    {
        ImGui::Text(xorstr("Bind key:"));

        static bool waitingForKey = false;
        static int delayCounter = 0;

        if (!waitingForKey) {
            auto needKeyName = ImGui::GetKeyName(hk->key);
            if (ImGui::Button(needKeyName, ImVec2(max(ImGui::CalcTextSize(needKeyName)[0], size_arg.x), size_arg.y))) {
                waitingForKey = true;
                delayCounter = 0;
            }
        }
        else {
            ImGui::Button(xorstr("..."), size_arg);

            if (++delayCounter > 3) {
                for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) {
                    ImGuiKey curKey = static_cast<ImGuiKey>(i);
                    if (ImGui::IsKeyDown(curKey)) {
                        if (curKey == ImGuiKey_Escape) {
                            hk->key = ImGuiKey_None;
                        }
                        else {
                            hk->key = curKey;
                        }
                        waitingForKey = false;
                        delayCounter = 0;
                        break;
                    }
                }
            }
        }

        ImGui::Spacing();

        ImGui::Text(xorstr("Active state:"));

        if (ImGui::Selectable(xorstr("Always on"), hk->type == hotkey_t::always_on))
            hk->type = hotkey_t::always_on;

        if (ImGui::Selectable(xorstr("Hold"), hk->type == hotkey_t::hold))
            hk->type = hotkey_t::hold;

        if (ImGui::Selectable(xorstr("Toggle"), hk->type == hotkey_t::toggle))
            hk->type = hotkey_t::toggle;

        if (ImGui::Selectable(xorstr("Force disable"), hk->type == hotkey_t::force_disable))
            hk->type = hotkey_t::force_disable;

        ImGui::End();
    }
}

void imgui_vispos(const char* label, visual_t* vis, const ImVec2& size_arg = ImVec2(0, 0)) {
    if (ImGui::BeginPopupContextItem(label))
    {
        ImGui::Text(xorstr("Position:"));

        if (ImGui::Selectable(xorstr("Left"), vis->pos == visual_t::left))
            vis->pos = visual_t::left;

        if (ImGui::Selectable(xorstr("Top"), vis->pos == visual_t::top))
            vis->pos = visual_t::top;

        if (ImGui::Selectable(xorstr("Right"), vis->pos == visual_t::right))
            vis->pos = visual_t::right;

        if (ImGui::Selectable(xorstr("Bottom"), vis->pos == visual_t::bottom))
            vis->pos = visual_t::bottom;

        ImGui::Checkbox(xorstr("Outline"), &vis->outlined);
        ImGui::Checkbox(xorstr("Team Color"), &vis->team_col);

        ImGui::End();
    }
}

int hashes = 0;

void imgui_checkbox_bind(const char* icon, const char* label, bool* v, hotkey_t* k_v, float width) {
    float check_size = checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale];
    ImGui::CheckboxSized(label, v, ImVec2(width - settings::menu::styles::margin * 3 - check_size, check_size));
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width - settings::menu::styles::margin * 1.5f - check_size, ImGui::GetCursorPosY() - check_size * 0.25f));

    std::string bind_name(label);
    bind_name.append("_keybind");

    ImGui::PushFont(fonts::font_icon);
    bool onclick = ImGui::Button(icon, ImVec2(check_size, check_size));
    ImGui::PopFont();

    if (onclick) {
        ImGui::OpenPopup(bind_name.c_str());
    }


    imgui_hotkey(bind_name.c_str(),k_v, ImVec2(check_size * 3, check_size));
}

void imgui_checkbox_col(const char* label, bool* v, float* col, float width) {
    float check_size = checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale];
    ImGui::CheckboxSized(label, v, ImVec2(width - settings::menu::styles::margin * 3 - check_size, check_size));
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width - settings::menu::styles::margin * 1.5f - check_size, ImGui::GetCursorPosY() - check_size * 0.25f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::ColorEdit4(label, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::PopStyleVar();
}

void imgui_checkbox_col_vis(const char* icon, const char* label, float* col, visual_t* vis, float width) {
    float check_size = checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale];
    ImGui::CheckboxSized(label, &vis->enabled, ImVec2(width - settings::menu::styles::margin * 2.75f - check_size*3, check_size));
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width - settings::menu::styles::margin * 2.75f - check_size*2, ImGui::GetCursorPosY() - check_size * 0.25f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::ColorEdit4(label, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::PopStyleVar();

    std::string bind_name(label);
    bind_name.append("_visual");

    ImGui::PushFont(fonts::font_icon);
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width - settings::menu::styles::margin * 1.5f - check_size, ImGui::GetCursorPosY() - check_size * 0.25f));
    bool onclick = ImGui::Button(icon, ImVec2(check_size, check_size));
    ImGui::PopFont();

    if (onclick) {
        ImGui::OpenPopup(bind_name.c_str());
    }


    imgui_vispos(bind_name.c_str(), vis, ImVec2(check_size * 3, check_size));
}

std::unordered_map<std::string, IDirect3DTexture9*> textures;

void imgui_cacheImage(IDirect3DDevice9* device, std::string textureID, const unsigned char* image_data, size_t image_size) {
    textures[textureID] = LoadTextureFromMemory(device, image_data, image_size);
}

void imgui_drawimage(std::string textureID, ImVec2 size) {
    if (!textures[textureID]) {
        ImGui::Text(xorstr("No texture"));
    }
    ImGui::Image((ImTextureID)(intptr_t)textures[textureID], size);
}

ImVec4 col_to_imvec4(float* col) {
    return ImVec4(col[0], col[1], col[2], col[3]);
}

void imgui_render_prev_text(box_t box, const char* text, float color[4], int pos, float& offset_l, float& offset_t, float& offset_r, float& offset_b)
{
    if (!strcmp(text, ""))
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 size = ImGui::CalcTextSize(text);
    if (pos == 0) { // L
        draw_list->AddText(ImVec2(box.left - size.x, box.bottom + offset_l), ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), text);
        offset_l += size.y + 1;
    }
    else if (pos == 1) { // T
        draw_list->AddText(ImVec2((box.left + (box.right - box.left) / 2.f) - (size.x / 2.f), box.bottom - size.y - offset_t), ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), text);
        offset_t += size.y + 1;
    }
    else if (pos == 2) { // R
        draw_list->AddText(ImVec2(box.right, box.bottom + offset_r), ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), text);
        offset_r += size.y + 1;
    }
    else if (pos == 3) { // B
        draw_list->AddText(ImVec2((box.left + (box.right - box.left) / 2.f) - (size.x / 2.f), box.top + offset_b), ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), text);
        offset_b += size.y + 1;
    }
}

void display_vis_preview(ImVec2 pos, ImVec2 size) {
    ImGui::PushFont(fonts::esp_font);
    float check_size = checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale];
    box_t box;
    box.left = pos.x;
    box.top = pos.y + size.y;
    box.right = pos.x+size.x;
    box.bottom = pos.y + check_size;
    

    if (settings::visuals::esp::players::box) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::players::colors::box));
        ImGui::GetWindowDrawList()->AddRect(ImVec2(box.left, box.top), ImVec2(box.right, box.bottom), ImGui::GetColorU32(ImGuiCol_Text), 0.f, 0.f, 1.f);
        ImGui::PopStyleColor();
    }
    float o_l = 0.f;
    float o_t = 0.f;
    float o_r = 0.f;
    float o_b = 0.f;

    if (settings::visuals::esp::players::health_bar) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::menu::colors::window_bg));
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - settings::menu::styles::margin * 0.75f * settings::visuals::esp::players::health_bar_thick, pos.y + check_size), ImVec2(pos.x - settings::menu::styles::margin * 0.25f * settings::visuals::esp::players::health_bar_thick, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::players::colors::health_bar));
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - settings::menu::styles::margin * 0.75f * settings::visuals::esp::players::health_bar_thick, pos.y + check_size + size.y * 0.41f), ImVec2(pos.x - settings::menu::styles::margin * 0.25f * settings::visuals::esp::players::health_bar_thick, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
        ImGui::PopStyleColor();
        box.left = pos.x - settings::menu::styles::margin * 0.75f * settings::visuals::esp::players::health_bar_thick;
    }

    if (settings::visuals::esp::players::name.enabled) {
        imgui_render_prev_text(box, xorstr("Exec"), settings::visuals::esp::players::colors::name, settings::visuals::esp::players::name.pos, o_l, o_t, o_r, o_b);
    }

    if (settings::visuals::esp::players::health_text.enabled) {
        imgui_render_prev_text(box, xorstr("59hp"), settings::visuals::esp::players::colors::health_text, settings::visuals::esp::players::health_text.pos, o_l, o_t, o_r, o_b);
    }

    if (settings::visuals::esp::players::armor.enabled) {
        imgui_render_prev_text(box, xorstr("40ar"), settings::visuals::esp::players::colors::armor, settings::visuals::esp::players::armor.pos, o_l, o_t, o_r, o_b);
    }

    if (settings::visuals::esp::players::weapon_name.enabled) {
        imgui_render_prev_text(box, xorstr("pickaxe"), settings::visuals::esp::players::colors::weapon_name, settings::visuals::esp::players::weapon_name.pos, o_l, o_t, o_r, o_b);
    }
    if (settings::visuals::esp::players::user_group.enabled) {
        imgui_render_prev_text(box, xorstr("user"), settings::visuals::esp::players::colors::user_group, settings::visuals::esp::players::user_group.pos, o_l, o_t, o_r, o_b);
    }
    if (settings::visuals::esp::players::rp_team.enabled) {
        imgui_render_prev_text(box, xorstr("miner"), settings::visuals::esp::players::colors::rp_team, settings::visuals::esp::players::rp_team.pos, o_l, o_t, o_r, o_b);
    }
    if (settings::visuals::esp::players::distance.enabled) {
        imgui_render_prev_text(box, xorstr("100m"), settings::visuals::esp::players::colors::distance, settings::visuals::esp::players::distance.pos, o_l, o_t, o_r, o_b);
    }

    if (settings::visuals::esp::players::skeleton) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::players::colors::skeleton));
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.3f), ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.3f, pos.y + size.y * 0.425f), ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.3f, pos.y + size.y * 0.425f), ImVec2(pos.x + size.x * 0.025f, pos.y + size.y * 0.95f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImVec2(pos.x + size.x * 0.82f, pos.y + size.y * 0.5f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.82f, pos.y + size.y * 0.5f), ImVec2(pos.x + size.x, pos.y + size.y * 0.8f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.8f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.8f), ImVec2(pos.x + size.x * 0.4f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.8f), ImVec2(pos.x + size.x * 0.7f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::PopStyleColor();
    }
    ImGui::PopFont();
}

void display_vis_preview_ent(ImVec2 pos, ImVec2 size) {
    ImGui::PushFont(fonts::esp_font);
    float check_size = checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale];
    if (settings::visuals::esp::entity::name) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::entity::colors::name));
        ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + size.x / 2 - ImGui::CalcTextSize("Printer")[0] / 2, pos.y), ImGui::GetColorU32(ImGuiCol_Text), "Printer");
        ImGui::PopStyleColor();
    }

    if (settings::visuals::esp::entity::box) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::entity::colors::box));
        ImGui::GetWindowDrawList()->AddRect(ImVec2(pos.x, pos.y + check_size), ImVec2(pos.x + size.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), 0.f, 0.f, 1.f);
        ImGui::PopStyleColor();
    }
    if (settings::visuals::esp::entity::health) {
        if (settings::visuals::esp::entity::health_bar) {
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::menu::colors::window_bg));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - settings::menu::styles::margin * 0.75f, pos.y + check_size), ImVec2(pos.x - settings::menu::styles::margin * 0.25f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::entity::colors::health));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - settings::menu::styles::margin * 0.75f, pos.y + check_size + size.y * 0.2f), ImVec2(pos.x - settings::menu::styles::margin * 0.25f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::PopStyleColor();
        }
        if (settings::visuals::esp::entity::health_text) {
            if (settings::visuals::esp::entity::health_bar) {
                ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::entity::colors::health));
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("80").x - settings::menu::styles::margin, pos.y + ImGui::CalcTextSize("80").y), ImGui::GetColorU32(ImGuiCol_Text), "80");
                ImGui::PopStyleColor();
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(settings::visuals::esp::entity::colors::health));
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("80").x - settings::menu::styles::margin * 0.25f, pos.y + ImGui::CalcTextSize("80").y), ImGui::GetColorU32(ImGuiCol_Text), "80");
                ImGui::PopStyleColor();
            }
        }
    }
    ImGui::PopFont();
}

int selected_script = -1; // From LUA tab
std::string lua_dir = xorstr("C:/fatalgm/lua/");
std::vector<std::string> scripts = utilities::get_files_from_folder(lua_dir, "", xorstr(".lua")); // LUA tab

std::string cfg_dir = xorstr("C:/fatalgm/configs/");
std::vector<std::string> configs = utilities::get_files_from_folder(cfg_dir, "", xorstr(".ftg")); // CONFIGS

const char* chams_types[] = { "Normal", "Metal", "Wireframe", "Flat"};

const char* skyboxes[] = { "Morning", "Day", "Evening", "Night", "Cloud" };

bool dpi_cached = false;
bool img_cached = false;

bool luaInited = false;
void init_lua_editor() {
    menu::lua_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
}

void menu::render(IDirect3DDevice9* device)
{
    if (!img_cached) {
        imgui_cacheImage(device, "shaxter_image", visPreview, visPreviewSize);
        imgui_cacheImage(device, "printer_image", visPreviewEnt, visPreviewEntSize);

        img_cached = true;
    }

    if (!settings::menu::opened)
        return;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, settings::menu::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_TabBarBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, settings::menu::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, settings::menu::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, settings::menu::styles::rounding);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, settings::menu::styles::scroll_width);
    ImGui::PushStyleVar(ImGuiStyleVar_PlaySfx, settings::menu::styles::play_sfx);
    ImGui::PushStyleVar(ImGuiStyleVar_AnimSpeed, settings::menu::styles::anim_speed);

    ImGuiStyle* pstyle = &ImGui::GetStyle();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(settings::menu::colors::window_bg[0], settings::menu::colors::window_bg[1], settings::menu::colors::window_bg[2], settings::menu::colors::window_bg[3]));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(settings::menu::colors::window_bg[0], settings::menu::colors::window_bg[1], settings::menu::colors::window_bg[2], settings::menu::colors::window_bg[3]));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(settings::menu::colors::text_inactive[0], settings::menu::colors::text_inactive[1], settings::menu::colors::text_inactive[2], settings::menu::colors::text_inactive[3]));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(settings::menu::colors::text[0], settings::menu::colors::text[1], settings::menu::colors::text[2], settings::menu::colors::text[3]));
    ImGui::PushStyleColor(ImGuiCol_TabSelected, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_TabDimmed, ImVec4(settings::menu::colors::transp[0], settings::menu::colors::transp[1], settings::menu::colors::transp[2], settings::menu::colors::transp[3]));
    ImGui::PushStyleColor(ImGuiCol_TabDimmedSelected, ImVec4(settings::menu::colors::transp[0], settings::menu::colors::transp[1], settings::menu::colors::transp[2], settings::menu::colors::transp[3]));
    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(settings::menu::colors::transp[0], settings::menu::colors::transp[1], settings::menu::colors::transp[2], settings::menu::colors::transp[3]));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(settings::menu::colors::border[0], settings::menu::colors::border[1], settings::menu::colors::border[2], settings::menu::colors::border[3]));
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(settings::menu::colors::border[0], settings::menu::colors::border[1], settings::menu::colors::border[2], settings::menu::colors::border[3]));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(settings::menu::colors::button[0], settings::menu::colors::button[1], settings::menu::colors::button[2], settings::menu::colors::button[3]));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(settings::menu::colors::button_hover[0], settings::menu::colors::button_hover[1], settings::menu::colors::button_hover[2], settings::menu::colors::button_hover[3]));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(settings::menu::colors::button_clicked[0], settings::menu::colors::button_clicked[1], settings::menu::colors::button_clicked[2], settings::menu::colors::button_clicked[3]));

    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(settings::menu::colors::window_bg[0], settings::menu::colors::window_bg[1], settings::menu::colors::window_bg[2], settings::menu::colors::window_bg[3]));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(settings::menu::colors::frame_bg_active[0], settings::menu::colors::frame_bg_active[1], settings::menu::colors::frame_bg_active[2], settings::menu::colors::frame_bg_active[3]));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(settings::menu::colors::frame_bg_hover[0], settings::menu::colors::frame_bg_hover[1], settings::menu::colors::frame_bg_hover[2], settings::menu::colors::frame_bg_hover[3]));

    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(settings::menu::colors::selection_high[0], settings::menu::colors::selection_high[1], settings::menu::colors::selection_high[2], settings::menu::colors::selection_high[3]));

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(settings::menu::colors::selection_high[0], settings::menu::colors::selection_high[1], settings::menu::colors::selection_high[2], settings::menu::colors::selection_high[3]));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(settings::menu::colors::transp[0], settings::menu::colors::transp[1], settings::menu::colors::transp[2], settings::menu::colors::transp[3]));

    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(settings::menu::colors::window_bg[0], settings::menu::colors::window_bg[1], settings::menu::colors::window_bg[2], settings::menu::colors::window_bg[3]));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(settings::menu::colors::button[0], settings::menu::colors::button[1], settings::menu::colors::button[2], settings::menu::colors::button[3]));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(settings::menu::colors::button_hover[0], settings::menu::colors::button_hover[1], settings::menu::colors::button_hover[2], settings::menu::colors::button_hover[3]));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(settings::menu::colors::button_clicked[0], settings::menu::colors::button_clicked[1], settings::menu::colors::button_clicked[2], settings::menu::colors::button_clicked[3]));

    if (!dpi_cached) {
        pstyle->CacheScales();
        dpi_cached = true;
        float new_dpi = dpi_scales_num[settings::miscellaneous::dpi_scale];
        pstyle->SetDPIScale(new_dpi);
        settings::menu::styles::win_size_ex = settings::menu::styles::win_size;
        settings::menu::styles::win_size.x = settings::menu::styles::win_size.x * new_dpi;
        settings::menu::styles::win_size.y = settings::menu::styles::win_size.y * new_dpi;
        io.FontGlobalScale = new_dpi;
    }

    ImGui::SetNextWindowSize(settings::menu::styles::win_size); // Don't use ImGuiCond_Once due to DPI scale

    ImGui::PushFont(fonts::josefin_sans_regular);
    ImGui::Begin(xorstr("FATALMENU by Pavetr"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    child_labels = {};

    float maintab_h = ImGui::GetFontSize() * 2.75f;
    float endtab_h = ImGui::GetFontSize() * 1.5f;

    ImVec2 winpos = ImGui::GetWindowPos();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(settings::menu::colors::child_bg[0], settings::menu::colors::child_bg[1], settings::menu::colors::child_bg[2], settings::menu::colors::child_bg[3]));

    // Header
    ImGui::SetCursorPos(ImVec2(0.f, 0.f));
    ImGui::BeginChild(xorstr("fatal_shapka"), ImVec2(settings::menu::styles::win_size[0], maintab_h), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PopFont();
    ImGui::PushFont(fonts::josefin_sans_header);
    ImGui::SetCursorPos(ImVec2(settings::menu::styles::margin, maintab_h * 0.5f - ImGui::GetFontSize() * 0.4f));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
    ImGui::Text(xorstr("FATAL"));
    ImGui::PopStyleColor();
    ImGui::SetCursorPos(ImVec2(settings::menu::styles::margin * 1.2f, maintab_h * 0.5f - ImGui::GetFontSize() * 0.4f));
    ImGui::Text(xorstr("FATAL"));
    float header_size = ImGui::CalcTextSize(xorstr("FATAL"))[0];
    ImGui::PopFont();
    ImGui::PushFont(fonts::josefin_sans_regular);

    ImGui::PushFont(fonts::font_icon);
    float prev_b_w = 0.f;
    for (int i = 0; i < 6; ++i) {
        const char* txt = tabs[i];
        ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize(txt)[0] + settings::menu::styles::margin * 2, ImGui::CalcTextSize(txt)[1] + settings::menu::styles::margin);

        ImGui::SetCursorPos(ImVec2(header_size + settings::menu::styles::margin * 2 + prev_b_w, maintab_h * 0.5f - buttonSize[1] * 0.5f));
        if (ImGui::Button(txt, buttonSize, curtab == i ? ImGuiCol_Text : ImGuiCol_TextDisabled))
            curtab = i;
        prev_b_w += buttonSize[0] + settings::menu::styles::margin;
    }
    ImGui::PopFont();


    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(winpos[0], winpos[1] + maintab_h - 1.f), ImVec2(winpos[0] + settings::menu::styles::win_size[0], winpos[1] + maintab_h + 1.f), ImGui::GetColorU32(ImGuiCol_Separator));

    // Footer
    ImGui::SetCursorPos(ImVec2(0.f, settings::menu::styles::win_size[1] - endtab_h));
    ImGui::BeginChild(xorstr("fatal_footer"), ImVec2(settings::menu::styles::win_size[0], endtab_h), ImGuiChildFlags_None);
    ImGui::PushFont(fonts::font_icon);
    ImGui::SetCursorPosX(settings::menu::styles::margin);
    if (ImGui::Button(ICON_FA_SAVE)) {
        ImGui::OpenPopup("save_configs_popup");
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(settings::menu::styles::margin * 2 + ImGui::CalcTextSize(ICON_FA_SAVE)[0]);
    if (ImGui::Button(ICON_FA_FOLDER)) {
        ImGui::OpenPopup("load_configs_popup");
    }

    if (ImGui::BeginPopup("load_configs_popup"))
    {
        ImGui::SeparatorText("Load cfg");
        for (int i = 0; i < configs.size(); i++) {
            if (ImGui::Selectable(configs[i].c_str())) {
                config_manager::load(configs[i]);
            }
        }
        if (ImGui::Button(xorstr("Open folder"))) {
            std::wstring stemp = std::wstring(cfg_dir.begin(), cfg_dir.end());
            LPCWSTR sw = stemp.c_str();
            ShellExecute(NULL, L"open", sw, NULL, NULL, SW_SHOWNORMAL);
        }

        if (ImGui::Button(xorstr("Refresh"))) {
            configs = utilities::get_files_from_folder(cfg_dir, "", xorstr(".ftg"));
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("save_configs_popup"))
    {
        ImGui::SeparatorText("Save cfg");
        static char cfg_name[32] = "";
        ImGui::Text("Enter config name:");
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
        ImGui::InputText("##cfg_name", cfg_name, IM_ARRAYSIZE(cfg_name));
        ImGui::PopStyleVar();
        if (ImGui::Button("Save")) {
            config_manager::save(cfg_name);
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(settings::menu::styles::win_size[0] - settings::menu::styles::margin * 2 - ImGui::CalcTextSize(ICON_FA_INFO_CIRCLE)[0]);
    if (ImGui::Button(ICON_FA_INFO_CIRCLE)) {
        ImGui::OpenPopup("info_popup");
    }
    if (ImGui::BeginPopup("info_popup"))
    {
        ImGui::Text(xorstr("Fatal Gmod by pavetr\npavetr.ru/ds\nWant more? - ignitecheat.xyz\nThanks to github.com/pa1n-dev"));
        ImGui::Separator();
        ImGui::Text("Build date: " __DATE__ " " __TIME__);
        ImGui::EndPopup();
    }

    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(winpos[0], winpos[1] + settings::menu::styles::win_size[1] - endtab_h - 1.f), ImVec2(winpos[0] + settings::menu::styles::win_size[0], winpos[1] + settings::menu::styles::win_size[1] - endtab_h + 1.f), ImGui::GetColorU32(ImGuiCol_Separator));

    ImGui::PopStyleColor();

    // Main block
    ImGui::SetCursorPos(ImVec2(settings::menu::styles::margin, maintab_h + 1.f + settings::menu::styles::margin));
    ImGui::BeginChild(xorstr("fatal_main"), ImVec2(settings::menu::styles::win_size[0] - settings::menu::styles::margin * 2, settings::menu::styles::win_size[1] - endtab_h - maintab_h - 2.f - settings::menu::styles::margin * 2), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float row_width = (settings::menu::styles::win_size[0]) / 3 - settings::menu::styles::margin * 1.3f;
    float row_height = settings::menu::styles::win_size[1] - endtab_h - maintab_h - 2.f - settings::menu::styles::margin * 7;
    float vis_small_h = row_height / 5 - settings::menu::styles::margin * 2;

    ImVec2 checkboxSize = ImVec2(row_width - settings::menu::styles::margin * 2, checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale]);
    float check_size = checkboxSizeEx * dpi_scales_num[settings::miscellaneous::dpi_scale];

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(settings::menu::colors::child_bg[0], settings::menu::colors::child_bg[1], settings::menu::colors::child_bg[2], settings::menu::colors::child_bg[3]));
    if (curtab == 0) {
        if (ImGui::BeginTabBar(xorstr("aim_tabbar"), ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(xorstr("Aimbot")))
            {
                ImGui::SetCursorPosY(maintab_h);
                ImGui::Text(xorstr("Coming soon..."));
                /*ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(settings::menu::styles::margin, settings::menu::styles::margin));

                ImGui::BeginChild(xorstr("aim_row_1"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "WEAPON", curtab);

                ImGui::SliderInt(xorstr("Hit-chance"), &settings::rage::hitchance, 0, 100, "%d%%", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                // Also SliderFloat works
                ImGui::SliderInt(xorstr("Pointscale"), &settings::rage::pointscale, 0, 100, "%d%%", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::SliderInt(xorstr("Min-damage"), &settings::rage::mindamage, 0, 150, "%d hp", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);

                const char* hit_bones[] = { "Head","Chest","Stomach","Arms","Legs","Feet" };
                ImGui::Combo(xorstr("Hitboxes"), &settings::rage::hitbox, hit_bones, 6, -1, checkboxSize[0]);
                ImGui::Combo(xorstr("Multipoint"), &settings::rage::multipoint, hit_bones, 6, -1, checkboxSize[0]);
                const char* select_target[] = { "Crosshair","FOV","Health","Distance" };
                ImGui::Combo(xorstr("Target selection"), &settings::rage::targetsel, select_target, 4, -1, checkboxSize[0]);

                ImGui::EndChild();

                ImGui::SameLine(row_width + settings::menu::styles::margin);

                ImGui::BeginChild(xorstr("aim_row_2"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

                imgui_checkbox_bind(ICON_FA_COG, "Autostop", &settings::rage::autostop, &settings::rage::autostop_bind, row_width);

                ImGui::CheckboxSized(xorstr("Autoscope"), &settings::rage::autoscope, checkboxSize);
                ImGui::CheckboxSized(xorstr("Ignore limbs on moving"), &settings::rage::ignore_limbs, checkboxSize);
                ImGui::CheckboxSized(xorstr("Autorevolver"), &settings::rage::autorevolver, checkboxSize);

                ImGui::EndChild();

                ImGui::SameLine((row_width + settings::menu::styles::margin) * 2);

                ImGui::BeginChild(xorstr("aim_row_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);



                ImGui::CheckboxSized(xorstr("Aimbot"), &settings::rage::aimbot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Silent aim"), &settings::rage::silent, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Maximum fov"), &settings::rage::maxfov, 0, 180, "%d deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::CheckboxSized(xorstr("Autofire"), &settings::rage::autofire, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Delay shot"), &settings::rage::delay_shot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Duck peek assist"), &settings::rage::duck_peek, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Force bodyaim"), &settings::rage::force_bodyaim, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Force shoot"), &settings::rage::force_shoot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Headshot only"), &settings::rage::headshot_only, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Knife bot"), &settings::rage::knifebot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Zeus bot"), &settings::rage::zeusbot, ImVec2(checkboxSize[0], check_size));


                imgui_checkbox_bind(ICON_FA_COG "#", "Nospread", &settings::rage::nospread, &settings::rage::nospread_bind, row_width);

                imgui_checkbox_bind(ICON_FA_COG "##", "Doubletap", &settings::rage::doubletap, &settings::rage::doubletap_bind, row_width);*/


                /*ImGui::CheckboxSized(xorstr("ColorPicker"), &vars::isJopa123, ImVec2(row_width - settings::menu::styles::margin * 3 - check_size, check_size));
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(row_width - settings::menu::styles::margin * 1.5 - check_size,ImGui::GetCursorPosY()-check_size*0.4f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
                ImGui::ColorEdit4(xorstr("Background color"), settings::menu::colors::window_bg, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::PopStyleVar();

                ImGui::CheckboxSized(xorstr("Setting pop"), &vars::isJopa123, ImVec2(row_width - settings::menu::styles::margin * 3 - check_size, check_size));
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(row_width - settings::menu::styles::margin * 1.5 - check_size, ImGui::GetCursorPosY() - check_size * 0.4f));
                ImGui::PushFont(fonts::font_icon);
                if (ImGui::Button(ICON_FA_COG, ImVec2(check_size, check_size))) {
                    ImGui::OpenPopup("setting_popup");
                }
                if (ImGui::BeginPopup("setting_popup"))
                {
                    ImGui::Text("Keybind:");
                    imgui_hotkey(&vars::keybind,ImVec2(check_size*3, check_size));
                    ImGui::EndPopup();
                }
                ImGui::PopFont();
                ImGui::EndChild();

                ImGui::PopStyleVar();*/

                ImGui::EndTabItem();

            }
            if (ImGui::BeginTabItem(xorstr("Anti-Aim")))
            {
                ImGui::SetCursorPosY(maintab_h);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(settings::menu::styles::margin, settings::menu::styles::margin));

                ImGui::BeginChild(xorstr("aa_row_1"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                imgui_checkbox_bind(ICON_FA_COG, xorstr("Enable"), &settings::antihit::fake_angles::enable, &settings::antihit::fake_angles::hotkey, row_width);
                ImGui::CheckboxSized(xorstr("Fake duck"), &settings::antihit::fake_angles::fake_duck, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Turn at target"), &settings::antihit::fake_angles::at_target, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Invert yaw"), &settings::antihit::fake_angles::invert_yaw, ImVec2(checkboxSize[0], check_size));
                
                const char* yaws[] = { "LBY" };
                ImGui::Combo(xorstr("Yaw"), &settings::antihit::fake_angles::yaw, yaws, 1, -1, checkboxSize[0]);
                const char* pitches[] = { "Down", "Up" };
                ImGui::Combo(xorstr("Pitch"), &settings::antihit::fake_angles::pitch, pitches, 2, -1, checkboxSize[0]);

                ImGui::EndChild();

                ImGui::SameLine(row_width + settings::menu::styles::margin);

                ImGui::BeginChild(xorstr("aa_row_2"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "FAKELAG", curtab);
                ImGui::CheckboxSized(xorstr("Enable"), &settings::antihit::fake_lags::enable, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Limit"), &settings::antihit::fake_lags::count, 1, 24, xorstr("%d"), ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                const char* fake_states[] = { "On Ground", "In Air", "On Move", "On Stand", "Always" };
                ImGui::Combo(xorstr("States"), &settings::antihit::fake_lags::method, fake_states, 5, -1, checkboxSize[0]);
                
                ImGui::EndChild();

                ImGui::PopStyleVar();

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        draw_over_items();
    }
    else if (curtab == 1)
    {
        if (ImGui::BeginTabBar(xorstr("aim_tabbar"), ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(xorstr("Aimbot")))
            {
                ImGui::SetCursorPosY(maintab_h);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(settings::menu::styles::margin, settings::menu::styles::margin));

                ImGui::BeginChild(xorstr("aim_row_1"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                imgui_checkbox_bind(ICON_FA_COG, xorstr("Enable"), &settings::aimbot::globals::enable, &settings::aimbot::globals::hotkey, row_width);
                ImGui::CheckboxSized(xorstr("Silent"), &settings::aimbot::globals::silent, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Automatic fire"), &settings::aimbot::globals::automatic_fire, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Auto-wall"), &settings::aimbot::globals::penetrate_walls, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderFloat(xorstr("FOV"), &settings::aimbot::globals::fov, 0.f, 180.f, "%.1f", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                const char* hit_bones[] = { "Head","Chest","Stomach","Hitscan"};
                ImGui::Combo(xorstr("Hitboxes"), &settings::aimbot::globals::hitbox, hit_bones, 4, -1, checkboxSize[0]);
                const char* priority[] = { "FOV","Distance","Health"};
                ImGui::Combo(xorstr("Priority"), &settings::aimbot::globals::priority, priority, 3, -1, checkboxSize[0]);

                ImGui::EndChild();

                ImGui::SameLine(row_width + settings::menu::styles::margin);

                ImGui::BeginChild(xorstr("aim_row_2"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "ACCURACY", curtab);

                ImGui::CheckboxSized(xorstr("No spread"), &settings::aimbot::accuracy::predict_spread, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("No recoil"), &settings::aimbot::accuracy::disable_recoil, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("No lua recoil"), &settings::aimbot::accuracy::disable_visual_recoil, ImVec2(checkboxSize[0], check_size));
                
                ImGui::SliderFloat(xorstr("Backtrack"), &settings::aimbot::accuracy::backtrack, 0.f, 1.f, xorstr("%.3f ms"), ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::SliderFloat(xorstr("Smooth"), &settings::aimbot::accuracy::smooth, 0.f, 20.f, xorstr("%.1f"), ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                
                ImGui::EndChild();

                ImGui::SameLine((row_width + settings::menu::styles::margin) * 2);

                ImGui::BeginChild(xorstr("aim_row_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);
                
                imgui_checkbox_col(xorstr("FOV circle"), &settings::aimbot::visuals::fov, settings::aimbot::visuals::colors::fov, row_width);
                imgui_checkbox_col(xorstr("Snaplines"), &settings::aimbot::visuals::snaplines, settings::aimbot::visuals::colors::snaplines, row_width);
                ImGui::EndChild();

                ImGui::PopStyleVar();

                ImGui::EndTabItem();

            }
            ImGui::EndTabBar();
        }
        draw_over_items();
    }
    else if (curtab == 2)
    {
        if (ImGui::BeginTabBar(xorstr("vis_tabbar"), ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(xorstr("Local"))) {

                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "HANDS", curtab);

                imgui_checkbox_col(xorstr("Hands chams"), &settings::visuals::chams::hands::enable, settings::visuals::chams::colors::hands, row_width);
                ImGui::Combo(xorstr("Type"), &settings::visuals::chams::hands::material_type, chams_types, 4, -1, checkboxSize[0]);
                ImGui::Spacing();
                ImGui::CheckboxSized(xorstr("Draw original model"), &settings::visuals::chams::hands::draw_original_model, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(0.f, vis_small_h * 2 + maintab_h + settings::menu::styles::margin * 1.5f));

                ImGui::BeginChild(xorstr("vis_sect_2"), ImVec2(row_width, row_height - vis_small_h * 2 - settings::menu::styles::margin * 1.5f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                ImGui::CheckboxSized(xorstr("FOV Changer"), &settings::visuals::world::fov_changer::enable, ImVec2(checkboxSize[0], check_size));

                if (settings::visuals::world::fov_changer::enable) {
                    ImGui::SliderFloat(xorstr("FOV"), &settings::visuals::world::fov_changer::value, 50.f, 179.f, "%.2f deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }

                ImGui::CheckboxSized(xorstr("Model FOV"), &settings::visuals::world::model_fov_changer::enable, ImVec2(checkboxSize[0], check_size));

                if (settings::visuals::world::model_fov_changer::enable) {
                    ImGui::SliderFloat(xorstr("FOV##1"), &settings::visuals::world::model_fov_changer::value, 50.f, 179.f, "%.2f deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }

                imgui_checkbox_col(xorstr("Fake model chams"), &settings::antihit::visuals::fake_model::enable, settings::antihit::visuals::colors::fake_model, row_width);
                ImGui::Combo(xorstr("Fake model type"), &settings::antihit::visuals::fake_model::material_type, chams_types, 4, -1, checkboxSize[0]);

                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(xorstr("Player"))) {
                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                ImGui::CheckboxSized(xorstr("Enabled"), &settings::visuals::esp::players::enable, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Draw distance"), &settings::visuals::esp::players::render_distance, 0, 50000, "%d m", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::CheckboxSized(xorstr("Draw dormant"), &settings::visuals::esp::players::dormant, ImVec2(checkboxSize[0], check_size));
                ImGui::EndChild();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + settings::menu::styles::margin);

                ImGui::BeginChild(xorstr("vis_sect_2"), ImVec2(row_width, row_height - vis_small_h * 2 - settings::menu::styles::margin * 1.3f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "ESP", curtab);

                imgui_checkbox_col_vis(ICON_FA_COG, xorstr("Name"), settings::visuals::esp::players::colors::name, &settings::visuals::esp::players::name, row_width);
                imgui_checkbox_col(xorstr("Box"), &settings::visuals::esp::players::box, settings::visuals::esp::players::colors::box, row_width);
                if (settings::visuals::esp::players::box) {
                    ImGui::CheckboxSized(xorstr("Box Outline"), &settings::visuals::esp::players::box_outline, ImVec2(checkboxSize[0], check_size));
                    ImGui::CheckboxSized(xorstr("Box Team Color"), &settings::visuals::esp::players::box_team_col, ImVec2(checkboxSize[0], check_size));
                }

                imgui_checkbox_col_vis(ICON_FA_COG "##", xorstr("Health Text"), settings::visuals::esp::players::colors::health_text, &settings::visuals::esp::players::health_text, row_width);
                imgui_checkbox_col(xorstr("Health Bar"), &settings::visuals::esp::players::health_bar, settings::visuals::esp::players::colors::health_bar, row_width);
                
                if (settings::visuals::esp::players::health_bar) {
                    ImGui::SliderFloat(xorstr("Bar thickness"), &settings::visuals::esp::players::health_bar_thick, 0.f, 2.f, "%.3f", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }

                imgui_checkbox_col_vis(ICON_FA_COG "##5", xorstr("Armor"), settings::visuals::esp::players::colors::armor, &settings::visuals::esp::players::armor, row_width);

                imgui_checkbox_col_vis(ICON_FA_COG "##1", xorstr("Weapon"), settings::visuals::esp::players::colors::weapon_name, &settings::visuals::esp::players::weapon_name, row_width);
                imgui_checkbox_col_vis(ICON_FA_COG "##2", xorstr("User Group"), settings::visuals::esp::players::colors::user_group, &settings::visuals::esp::players::user_group, row_width);
                imgui_checkbox_col_vis(ICON_FA_COG "##3", xorstr("Team"), settings::visuals::esp::players::colors::rp_team, &settings::visuals::esp::players::rp_team, row_width);
                imgui_checkbox_col_vis(ICON_FA_COG "##4", xorstr("Distance"), settings::visuals::esp::players::colors::distance, &settings::visuals::esp::players::distance, row_width);
                
                //imgui_checkbox_col_vis(xorstr("Skeleton"), settings::visuals::esp::players::colors::skeleton, &settings::visuals::esp::players::skeleton, row_width);

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width + settings::menu::styles::margin, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "PREVIEW", curtab);
                ImGui::SetCursorPos(ImVec2(settings::menu::styles::margin * 4, settings::menu::styles::margin * 4 + check_size));
                ImVec2 img_pos = ImGui::GetCursorScreenPos();
                ImVec2 img_size = ImVec2(row_width - settings::menu::styles::margin * 8, row_height - settings::menu::styles::margin * 8 - check_size);
                imgui_drawimage("shaxter_image", img_size);
                display_vis_preview(ImVec2(img_pos.x, img_pos.y - check_size), ImVec2(img_size.x, img_size.y + check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + settings::menu::styles::margin * 2, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_4"), ImVec2(row_width, vis_small_h * 2.5f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "MODEL", curtab);

                imgui_checkbox_col(xorstr("Visible chams"), &settings::visuals::chams::players::enable, settings::visuals::chams::colors::players, row_width);

                ImGui::Combo(xorstr("Type"), &settings::visuals::chams::players::material_type, chams_types, 4, -1, checkboxSize[0]);

                ImGui::Spacing();

                ImGui::CheckboxSized(xorstr("Draw original model"), &settings::visuals::chams::players::draw_original_model, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();


                ImGui::SetCursorPos(ImVec2(row_width * 2 + settings::menu::styles::margin * 2, maintab_h + vis_small_h * 2.5f + settings::menu::styles::margin * 1.5f));

                ImGui::BeginChild(xorstr("vis_sect_5"), ImVec2(row_width, vis_small_h * 2.f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "BACKTRACK", curtab);

                imgui_checkbox_col(xorstr("Visible chams##"), &settings::aimbot::visuals::backtrack::enable, settings::aimbot::visuals::colors::backtrack, row_width);

                ImGui::Combo(xorstr("Type##1"), &settings::aimbot::visuals::backtrack::material_type, chams_types, 4, -1, checkboxSize[0]);

                ImGui::Spacing();

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + settings::menu::styles::margin * 2, maintab_h + vis_small_h * 4.5f + settings::menu::styles::margin * 3.f));

                ImGui::BeginChild(xorstr("vis_sect_6"), ImVec2(row_width, vis_small_h * 2.f - settings::menu::styles::margin * 3.5f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

                //imgui_checkbox_col(xorstr("Glow"), &vars::settings::visuals::glow, vars::settings::visuals::glow_col, row_width);

                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(xorstr("Entity"))) {

                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                ImGui::CheckboxSized(xorstr("Enabled"), &settings::visuals::esp::entity::enable, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Draw distance"), &settings::visuals::esp::entity::render_distance, 0, 50000, "%d m", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::CheckboxSized(xorstr("Draw dormant"), &settings::visuals::esp::entity::dormant, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + settings::menu::styles::margin);

                ImGui::BeginChild(xorstr("vis_sect_2"), ImVec2(row_width, row_height - vis_small_h * 2 - settings::menu::styles::margin * 1.3f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "ESP", curtab);

                imgui_checkbox_col(xorstr("Name"), &settings::visuals::esp::entity::name, settings::visuals::esp::entity::colors::name, row_width);
                imgui_checkbox_col(xorstr("Box"), &settings::visuals::esp::entity::box, settings::visuals::esp::entity::colors::box, row_width);
                /*imgui_checkbox_col(xorstr("Health"), &settings::visuals::esp::entity::health, settings::visuals::esp::entity::colors::health, row_width);
                if (settings::visuals::esp::entity::health) {
                    ImGui::CheckboxSized(xorstr("Health Text"), &settings::visuals::esp::entity::health_text, ImVec2(checkboxSize[0], check_size));
                    ImGui::CheckboxSized(xorstr("Health Bar"), &settings::visuals::esp::entity::health_bar, ImVec2(checkboxSize[0], check_size));
                }*/
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width + settings::menu::styles::margin, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "PREVIEW", curtab);
                ImGui::SetCursorPos(ImVec2(settings::menu::styles::margin * 4, settings::menu::styles::margin * 4 + row_height * 0.25f));
                ImVec2 img_pos = ImGui::GetCursorScreenPos();
                ImVec2 img_size = ImVec2(row_width - settings::menu::styles::margin * 8, row_height * 0.5f - settings::menu::styles::margin * 8 - check_size);
                imgui_drawimage("printer_image", img_size);
                display_vis_preview_ent(ImVec2(img_pos.x, img_pos.y - check_size), ImVec2(img_size.x, img_size.y + check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + settings::menu::styles::margin * 2, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_4"), ImVec2(row_width, vis_small_h * 4.f + settings::menu::styles::margin * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "MODEL", curtab);

                imgui_checkbox_col(xorstr("Visible chams"), &settings::visuals::chams::entity::enable, settings::visuals::chams::colors::entity, row_width);
                
                ImGui::Combo(xorstr("Type"), &settings::visuals::chams::entity::material_type, chams_types, 4, -1, checkboxSize[0]);

                ImGui::Spacing();

                /*imgui_checkbox_col(xorstr("Invisible chams"), &vars::settings::visuals::ent_chams_invis, vars::settings::visuals::ent_chams_invis_col, row_width);

                ImGui::Combo(xorstr("Type##"), &vars::settings::visuals::ent_chams_invis_type, chams_types, 4, -1, checkboxSize[0]);

                ImGui::Spacing();*/

                ImGui::CheckboxSized(xorstr("Draw original model"), &settings::visuals::chams::entity::draw_original_model, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + settings::menu::styles::margin * 2, maintab_h + vis_small_h * 4.f + settings::menu::styles::margin * 4.f));

                ImGui::BeginChild(xorstr("vis_sect_6"), ImVec2(row_width, vis_small_h * 2.f - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

                //imgui_checkbox_col(xorstr("Glow"), &vars::settings::visuals::ent_glow, vars::settings::visuals::ent_glow_col, row_width);

                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(xorstr("World"))) {

                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                //ImGui::CheckboxSized(xorstr("Fullbright"), &settings::visuals::world::fullbright, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Spectator list"), &settings::visuals::world::spectator_list, ImVec2(checkboxSize[0], check_size));
                
                //imgui_checkbox_col(xorstr("World color"), &vars::settings::visuals::enable_colormod, vars::settings::visuals::colormod_col, row_width);

                //ImGui::CheckboxSized(xorstr("Custom sky"), &vars::settings::visuals::enable_skybox, ImVec2(checkboxSize[0], check_size));

                /*if (vars::settings::visuals::enable_skybox) {
                    ImGui::Combo(xorstr("Skybox"), &vars::settings::visuals::skybox, skyboxes, 5, -1, checkboxSize[0]);
                    ImGui::Spacing();
                }*/

                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        draw_over_items();

    }
    else if (curtab == 3) {

        ImGui::SetCursorPosY(settings::menu::styles::margin);

        ImGui::BeginChild(xorstr("misc_sect_1"), ImVec2(row_width, vis_small_h * 2 + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

        imgui_checkbox_bind(ICON_FA_COG "##1", "Third person", &settings::miscellaneous::globals::third_person::enable, &settings::miscellaneous::globals::third_person::hotkey, row_width);
        if (settings::miscellaneous::globals::third_person::enable) {
            ImGui::SliderInt(xorstr("Distance"), &settings::miscellaneous::globals::third_person::distance, 0, 1000, "%d m", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        }
        //imgui_checkbox_bind(ICON_FA_COG "##2", "Auto peek", &vars::settings::misc::autopeek, &vars::settings::misc::autopeek_bind, row_width);
        /*imgui_checkbox_bind(ICON_FA_COG "##3", "Free camera", &vars::settings::misc::freecam, &vars::settings::misc::freecam_bind, row_width);
        if (vars::settings::misc::freecam) {
            ImGui::SliderInt(xorstr("Speed"), &vars::settings::misc::freecam_distance, 0, 100, "%d m/s", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        }*/
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(0.f, vis_small_h * 2 + maintab_h + settings::menu::styles::margin * 1.5f));

        ImGui::BeginChild(xorstr("misc_sect_2"), ImVec2(row_width, vis_small_h + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "HITSOUND", curtab);
        /*const char* hit_sounds[] = { "Metal", "Bell", "Phonk", "Beep" };
        ImGui::Combo(xorstr("Hit Sound"), &vars::settings::misc::hitsound, hit_sounds, 4, -1, checkboxSize[0]);
        ImGui::Combo(xorstr("Kill Sound"), &vars::settings::misc::killsound, hit_sounds, 4, -1, checkboxSize[0]);*/

        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(0.f, vis_small_h * 3 + maintab_h + settings::menu::styles::margin * 6.5f));

        ImGui::BeginChild(xorstr("misc_sect_3"), ImVec2(row_width, vis_small_h * 2 + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "MOVEMENT", curtab);

        ImGui::CheckboxSized(xorstr("Bunny hop"), &settings::miscellaneous::movement::bhop, ImVec2(checkboxSize[0], check_size));
        ImGui::CheckboxSized(xorstr("Auto strafe"), &settings::miscellaneous::movement::air_strafe, ImVec2(checkboxSize[0], check_size));
        /*imgui_checkbox_bind(ICON_FA_COG "##2", "Air Stuck", &settings::miscellaneous::movement::air_stuck, &settings::miscellaneous::movement::air_stuck_key, row_width);
        if (settings::miscellaneous::movement::air_stuck) {
            ImGui::SliderInt(xorstr("Air Stuck Ticks"), &settings::miscellaneous::movement::air_stuck_ticks, 0, 20, "%d t/s", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        }*/
        /*if (settings::miscellaneous::movement::air_strafe) {
            const char* strafe_types[] = { "Legit", "Rage", "Multi-Directional" };
            ImGui::Combo(xorstr("Strafe"), &vars::settings::misc::autostrafe_type, strafe_types, 3, -1, checkboxSize[0]);
        }*/

        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(row_width + settings::menu::styles::margin, maintab_h - settings::menu::styles::margin * 3.5f));

        ImGui::BeginChild(xorstr("misc_sect_4"), ImVec2(row_width, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

        static char name_changer[256] = "";
        
        ImGui::Text(xorstr("Name changer"));
        ImGui::InputText(xorstr("name_changer_inp"), name_changer, IM_ARRAYSIZE(name_changer));
        if (ImGui::Button(xorstr("Change name"))) {
            c_net_channel* net_channel = interfaces::engine->get_net_channel();
            if (net_channel) {
                NET_SetConVar net_name(xorstr("name"), utilities::host_cleanup_con_var_string_value(name_changer));
                net_channel->send_net_msg(net_name, true);
            }
        }

        ImGui::Spacing();

        ImGui::CheckboxSized(xorstr("Custom loadscreen"), &settings::menu::custom_loading_screen::enable, ImVec2(checkboxSize[0], check_size));
        if (settings::menu::custom_loading_screen::enable) {
            ImGui::InputText(xorstr("loading_inp"), settings::menu::custom_loading_screen::url, IM_ARRAYSIZE(settings::menu::custom_loading_screen::url));
        }
        /*static char name_changer[256] = "";

        ImGui::Text(xorstr("Name changer"));
        ImGui::InputText(xorstr("name_changer_inp"), name_changer, IM_ARRAYSIZE(name_changer));
        if (ImGui::Button(xorstr("Change name"))) {
            c_net_channel* net_channel = interfaces::engine->get_net_channel();
            if (net_channel) {
                NET_SetConVar net_name(xorstr("name"), utilities::host_cleanup_con_var_string_value(name_changer));
                net_channel->send_net_msg(net_name, true);
            }
        }*/

        ImGui::Spacing();

        if (ImGui::Button(xorstr("Unload"), ImVec2(checkboxSize[0], check_size * 2)))
            globals::unload = true;

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width * 2 + settings::menu::styles::margin * 2);

        ImGui::BeginChild(xorstr("misc_sect_5"), ImVec2(row_width, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        // TIP: This trick fixes overlapping text while user scrolls down
        add_child_label(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + ImGui::GetScrollY()), "MENU", curtab);

        const char* dpi_scales[] = { "85%", "100%", "125%", "150%", "175%", "200%" };
        if (ImGui::Combo(xorstr("DPI Scale"), &settings::miscellaneous::dpi_scale, dpi_scales, 6, -1, checkboxSize[0])) {
            float new_dpi = dpi_scales_num[settings::miscellaneous::dpi_scale];
            pstyle->SetDPIScale(new_dpi);
            settings::menu::styles::win_size.x = settings::menu::styles::win_size_ex.x * new_dpi;
            settings::menu::styles::win_size.y = settings::menu::styles::win_size_ex.y * new_dpi;
            io.FontGlobalScale = new_dpi;
        }


        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

        ImGui::ColorEdit4(xorstr("Window background"), settings::menu::colors::window_bg, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Child background"), settings::menu::colors::child_bg, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Inactive text"), settings::menu::colors::text_inactive, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Text"), settings::menu::colors::text, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Accent"), settings::menu::colors::selection, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Accent highlight"), settings::menu::colors::selection_high, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Border"), settings::menu::colors::border, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Button"), settings::menu::colors::button, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Hovered button"), settings::menu::colors::button_hover, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Clicked button"), settings::menu::colors::button_clicked, ImGuiColorEditFlags_NoInputs);

        ImGui::ColorEdit4(xorstr("Hovered background"), settings::menu::colors::frame_bg_hover, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Active background"), settings::menu::colors::frame_bg_active, ImGuiColorEditFlags_NoInputs);

        ImGui::SliderFloat(xorstr("Rounding"), &settings::menu::styles::rounding, 0.f, 20.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Margin"), &settings::menu::styles::margin, 0.f, 20.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Animation time"), &settings::menu::styles::anim_speed, 0.f, 5.f, "%.2f s", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Padding X"), &settings::menu::styles::tab_padding.x, 0.f, 15.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Padding Y"), &settings::menu::styles::tab_padding.y, 0.f, 15.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);

        static bool enable_sfx = true;
        ImGui::CheckboxSized(xorstr("Play SFX"), &enable_sfx, ImVec2(checkboxSize[0], check_size));
        settings::menu::styles::play_sfx = enable_sfx ? 1.f : 0.f;

        ImGui::SliderFloat(xorstr("Scrollbar width"), &settings::menu::styles::scroll_width, 0.f, 25.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);


        ImGui::PopStyleVar();

        ImGui::EndChild();

        draw_over_items();
    }
    else if (curtab == 4) {
        ImGui::SetCursorPosY(settings::menu::styles::margin);

        ImGui::BeginChild(xorstr("lua_sect_1"), ImVec2(row_width, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "FILES", curtab);

        ImGui::BeginListBox("##lua_list", ImVec2(-FLT_MIN, row_height + maintab_h - settings::menu::styles::margin * 4));

        if (scripts.size() > 0) {
            for (size_t i = 0; i < scripts.size(); i++)
            {
                if (ImGui::Selectable((scripts[i] + xorstr(".lua")).c_str())) {
                    selected_script = i;
                    std::ifstream file(lua_dir + scripts[i] + xorstr(".lua"));
                    if (file.is_open()) {
                        std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                        file.close();
                        lua_editor.SetText(code);
                    }
                }
            }
        }
        
        if (ImGui::Button(xorstr("New file"))) {
            selected_script = -1;
            lua_editor.SetText("");
        }

        if (ImGui::Button(xorstr("Open folder"))) {
            std::wstring stemp = std::wstring(lua_dir.begin(), lua_dir.end());
            LPCWSTR sw = stemp.c_str();
            ShellExecute(NULL, L"open", sw, NULL, NULL, SW_SHOWNORMAL);
        }

        if (ImGui::Button(xorstr("Refresh"))) {
            scripts = utilities::get_files_from_folder(lua_dir, "", xorstr(".lua"));
        }

        ImGui::EndListBox();

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width + settings::menu::styles::margin);

        ImGui::BeginChild(xorstr("lua_sect_2"), ImVec2(row_width * 2, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "EDITOR", curtab);

        if (selected_script >= 0 && selected_script < scripts.size()) {
            ImGui::Text((scripts[selected_script]+".lua").c_str());
        }
        else {
            ImGui::Text(xorstr("New file"));
        }

        // LUA EDITOR HERE
        if (!luaInited) {
            init_lua_editor();
            luaInited = true;
        }

        menu::lua_editor.Render("Lua Editor", ImVec2(row_width * 2 - settings::menu::styles::margin * 2, row_height + maintab_h - settings::menu::styles::margin * 6 - 32.f));

        if (ImGui::Button(xorstr("Run script"), ImVec2(row_width, settings::menu::styles::margin * 2 + 16.f))) {
            globals::waiting_to_be_executed.clear();
            globals::waiting_to_be_executed.push_back(menu::lua_editor.GetText().c_str());
        }

        ImGui::EndChild();

        draw_over_items();
    }
    else if (curtab == 5) {
        ImGui::SetCursorPosY(settings::menu::styles::margin);

        ImGui::BeginChild(xorstr("list_sect_1"), ImVec2(row_width, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "ENTITIES", curtab);

        static char ent_search_field[128] = "";

        ImGui::InputText(xorstr("Search"), ent_search_field, sizeof(ent_search_field));

        ImGui::BeginListBox(xorstr("##ent_list"), ImVec2(-FLT_MIN, row_height + maintab_h - settings::menu::styles::margin * 4 - check_size * 2));
        utilities::update_entity_list(settings::visuals::esp::entity::list);
        std::string ent_search_str(ent_search_field);
        std::transform(ent_search_str.begin(), ent_search_str.end(), ent_search_str.begin(), ::tolower);
        for (auto item : settings::visuals::esp::entity::list.items())
        {
            bool temp = item.value();

            std::string cur_itm(item.key().c_str());
            std::transform(cur_itm.begin(), cur_itm.end(), cur_itm.begin(), ::tolower);

            if ((ent_search_str.empty() || cur_itm.find(ent_search_str) != std::string::npos))
                ImGui::Selectable(item.key().c_str(), &temp);
            item.value() = temp;
        }

        ImGui::EndListBox();

        ImGui::EndChild();
        
        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width + settings::menu::styles::margin);

        ImGui::BeginChild(xorstr("list_sect_2"), ImVec2(row_width, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "FRIENDS", curtab);

        static char friend_search_field[128] = "";
        ImGui::InputText(xorstr("Search"), friend_search_field, sizeof(friend_search_field));

        ImGui::BeginListBox(xorstr("##friend_list"), ImVec2(-FLT_MIN, row_height + maintab_h - settings::menu::styles::margin * 6 - check_size * 4));
        utilities::update_friend_list(settings::visuals::esp::players::friend_list);
        std::string friend_search_str(friend_search_field);
        std::transform(friend_search_str.begin(), friend_search_str.end(), friend_search_str.begin(), ::tolower);
        for (auto item : settings::visuals::esp::players::friend_list.items())
        {
            bool temp = item.value();

            std::string cur_itm(item.key().c_str());
            std::transform(cur_itm.begin(), cur_itm.end(), cur_itm.begin(), ::tolower);

            if ((friend_search_str.empty() || cur_itm.find(friend_search_str) != std::string::npos))
                ImGui::Selectable(item.key().c_str(), &temp);
            item.value() = temp;
        }

        ImGui::EndListBox();

        ImGui::CheckboxSized(xorstr("Ignore Aimbot"), &settings::visuals::esp::players::friend_ignore_aim, ImVec2(checkboxSize[0], check_size));
        ImGui::CheckboxSized(xorstr("Ignore Visuals"), &settings::visuals::esp::players::friend_ignore_vis, ImVec2(checkboxSize[0], check_size));

        ImGui::EndChild();
        /*
        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width * 2 + settings::menu::styles::margin * 2);

        ImGui::BeginChild(xorstr("skin_sect_3"), ImVec2(row_width, row_height + maintab_h - settings::menu::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "QUALITY", curtab);

        const char* wear_types[] = { "StarTrek", "Good", "Well-Worn", "Battle-Hardened" };

        ImGui::InputText("Search", vars::settings::skins::wear_search, sizeof(vars::settings::skins::wear_search));

        ImGui::BeginListBox("##wear_list", ImVec2(-FLT_MIN, row_height + maintab_h - settings::menu::styles::margin * 4 - check_size * 2));

        std::string wear_search_str(vars::settings::skins::wear_search);
        std::transform(wear_search_str.begin(), wear_search_str.end(), wear_search_str.begin(), ::tolower);
        for (size_t i = 0; i < sizeof(vars::settings::skins::selected_paint); i++)
        {
            std::string cur_wear(wear_types[i]);
            std::transform(cur_wear.begin(), cur_wear.end(), cur_wear.begin(), ::tolower);

            if ((wear_search_str.empty() || cur_wear.find(wear_search_str) != std::string::npos) && ImGui::Selectable(wear_types[i]))
                vars::settings::skins::selected_paint = i;
        }

        ImGui::EndListBox();

        */

        

        draw_over_items();
    }
    ImGui::PopStyleColor();


    ImGui::EndChild();

    ImGui::End();

    ImGui::PopFont();

    ImGui::PopStyleColor(27);
    ImGui::PopStyleVar(11);
}