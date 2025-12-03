#include "visuals.h"
#include "../aimbot/aimbot.h"

void visuals::render()
{
	if (!render_manager::draw_list)
		return;

	if (!interfaces::engine->is_in_game())
		return;

	c_base_entity* local_player = interfaces::entity_list->get_entity(interfaces::engine->get_local_player());
	if (!local_player)
		return;

	ImGuiIO& io = GetIO();

	render_manager::draw_list->_ResetForNewFrame();
	render_manager::draw_list->PushTextureID(io.Fonts->TexID);
	render_manager::draw_list->PushClipRectFullScreen();

	c_vector origin = local_player->get_abs_origin();

	for (size_t i = 0; i <= interfaces::entity_list->get_highest_entity_index(); i++)
	{
		c_base_entity* entity = interfaces::entity_list->get_entity(i);
		if (!entity)
			continue;

		if (entity->is_player())
		{
			if (!settings::visuals::esp::players::enable)
				continue;

			if (!entity->is_alive())
				continue;

			if (!settings::visuals::esp::players::dormant && entity->is_dormant())
				continue;

			if (entity == local_player)
				continue;

			box_t box;
			if (!utilities::get_entity_box(entity, box))
				continue;

			player_info_s info;
			interfaces::engine->get_player_info(i, &info);
			std::string pl_name = info.name;

			if (settings::visuals::esp::players::friend_ignore_vis && !pl_name.empty() && settings::visuals::esp::players::friend_list.contains(pl_name) && settings::visuals::esp::players::friend_list[pl_name].get<bool>())
				continue;

			// offsets
			float o_l = 0.f;
			float o_t = 0.f;
			float o_r = 0.f;
			float o_b = 0.f;


			float distance = origin.distance_to(entity->get_abs_origin());

			float alpha = std::clamp((settings::visuals::esp::players::render_distance - distance) / 100.f, 0.f, 1.f);
			if (alpha <= 0.0f)
				continue;

			float team_col[4] = { lua_utilities::get_team_col(entity).x, lua_utilities::get_team_col(entity).y, lua_utilities::get_team_col(entity).z, lua_utilities::get_team_col(entity).w };

			//PushStyleVar(ImGuiStyleVar_Alpha, alpha);

			if (settings::visuals::esp::players::box) {
				float* box_col = settings::visuals::esp::players::colors::box;
				if (settings::visuals::esp::players::box_team_col) {
					box_col = team_col;
				}
				render_manager::box(box, box_col, 1.f, settings::visuals::esp::players::box_outline);
			}

			if (settings::visuals::esp::players::health_bar) {
				float health_width = (box.right - box.left)*0.05f;
				render_manager::filled_rect(ImVec2(box.left - health_width * 0.75f * settings::visuals::esp::players::health_bar_thick, box.bottom), ImVec2(box.left - health_width * 0.25f * settings::visuals::esp::players::health_bar_thick, box.top), settings::menu::colors::window_bg,0);
				float hpMul = std::clamp((entity->get_health() / 100.f), 0.f, 1.f);
				render_manager::filled_rect(ImVec2(box.left - health_width * 0.75f * settings::visuals::esp::players::health_bar_thick, box.bottom + (box.top - box.bottom) * (1 - hpMul)), ImVec2(box.left - health_width * 0.25f * settings::visuals::esp::players::health_bar_thick, box.top), settings::visuals::esp::players::colors::health_bar, 0);
				
				box.left -= health_width * 0.75f * settings::visuals::esp::players::health_bar_thick;
			}

			if (settings::visuals::esp::players::name.enabled) {
				float* name_col = settings::visuals::esp::players::colors::name;
				if (settings::visuals::esp::players::name.team_col) {
					name_col = team_col;
				}
				render_manager::render_text(box, lua_utilities::get_name(entity), name_col, settings::visuals::esp::players::name, o_l, o_t, o_r, o_b);
			}
			if (settings::visuals::esp::players::health_text.enabled) {
				std::string hp_txt = std::to_string(entity->get_health()) + "hp";
				float* hpt_col = settings::visuals::esp::players::colors::health_text;
				if (settings::visuals::esp::players::health_text.team_col) {
					hpt_col = team_col;
				}
				render_manager::render_text(box, hp_txt.c_str(), hpt_col, settings::visuals::esp::players::health_text, o_l, o_t, o_r, o_b);
			}
			if (settings::visuals::esp::players::armor.enabled) {
				std::string hp_txt = std::to_string(lua_utilities::get_armor(entity)) + "ar";
				float* ar_col = settings::visuals::esp::players::colors::armor;
				if (settings::visuals::esp::players::armor.team_col) {
					ar_col = team_col;
				}
				render_manager::render_text(box, hp_txt.c_str(), ar_col, settings::visuals::esp::players::armor, o_l, o_t, o_r, o_b);
			}

			c_base_combat_weapon* weapon = entity->get_active_weapon();
			if (weapon)
			{
				if (settings::visuals::esp::players::weapon_name.enabled) {
					float* wep_col = settings::visuals::esp::players::colors::weapon_name;
					if (settings::visuals::esp::players::weapon_name.team_col) {
						wep_col = team_col;
					}
					render_manager::render_text(box, lua_utilities::language_get_phrase(lua_utilities::get_weapon_print_name(weapon)), wep_col, settings::visuals::esp::players::weapon_name, o_l, o_t, o_r, o_b);
				}
			}

			if (settings::visuals::esp::players::user_group.enabled) {
				float* ug_col = settings::visuals::esp::players::colors::user_group;
				if (settings::visuals::esp::players::user_group.team_col) {
					ug_col = team_col;
				}
				render_manager::render_text(box, lua_utilities::get_user_group(entity), ug_col, settings::visuals::esp::players::user_group, o_l, o_t, o_r, o_b);
			}

			if (settings::visuals::esp::players::rp_team.enabled) {
				float* team_col = settings::visuals::esp::players::colors::rp_team;
				if (settings::visuals::esp::players::rp_team.team_col) {
					team_col = team_col;
				}
				render_manager::render_text(box, lua_utilities::get_team_name(entity), team_col, settings::visuals::esp::players::rp_team, o_l, o_t, o_r, o_b);
			}

			if (settings::visuals::esp::players::distance.enabled)
			{
				std::string distance_text = std::to_string((int)distance) + "m";

				float* dist_col = settings::visuals::esp::players::colors::distance;
				if (settings::visuals::esp::players::distance.team_col) {
					dist_col = team_col;
				}

				render_manager::render_text(box, distance_text.c_str(), dist_col, settings::visuals::esp::players::distance, o_l, o_t, o_r, o_b);
			}
			
			/*if (settings::visuals::esp::players::skeleton) {
				
			}*/

			//PopStyleVar();
		}
		else if (settings::visuals::esp::entity::enable)
		{
			if (!settings::visuals::esp::entity::dormant && entity->is_dormant())
				continue;

			std::string name = entity->get_class_name();
			if (name.empty())
				continue;

			if (!settings::visuals::esp::entity::list.contains(name) || !settings::visuals::esp::entity::list[name].get<bool>())
				continue;

			box_t box;
			if (!utilities::get_entity_box(entity, box))
				continue;

			float offset = 0;
			float distance = origin.distance_to(entity->get_abs_origin());

			/*float alpha = std::clamp((settings::visuals::esp::entity::render_distance - distance) / 100.f, 0.f, 1.f);
			if (alpha <= 0.0f)
				continue;*/

			//PushStyleVar(ImGuiStyleVar_Alpha, alpha);

			if (settings::visuals::esp::entity::box)
				render_manager::box(box, settings::visuals::esp::entity::colors::box, 1.f, false);

			if (settings::visuals::esp::entity::distance)
			{
				std::string distance_text = std::to_string((int)distance) + "m";
				render_manager::render_text_simple(box, distance_text.c_str(), settings::visuals::esp::entity::colors::distance, offset);
			}

			if (settings::visuals::esp::entity::name)
				render_manager::render_text_simple(box, name.c_str(), settings::visuals::esp::entity::colors::name, offset);

			//PopStyleVar();
		}
	}

	ImVec2 center(io.DisplaySize.x / 2, io.DisplaySize.y / 2);

	if (settings::aimbot::visuals::snaplines && aimbot::target.entity)
	{
		c_vector pos;
		if (utilities::world_to_screen(aimbot::target.shoot_pos, &pos))
			render_manager::line(center, ImVec2(pos.x, pos.y), settings::aimbot::visuals::colors::snaplines, 1.f);
	}

	if (settings::aimbot::visuals::fov)
	{
		float screen_fov = tanf(math::deg2rad(0.5f * globals::fov)) * ((io.DisplaySize.x / io.DisplaySize.y) / (4.f / 3.f));
		float radius = tanf(math::deg2rad(settings::aimbot::globals::fov * 0.5) * 2) / screen_fov * (io.DisplaySize.x * 0.5);

		render_manager::circle(center, radius, settings::aimbot::visuals::colors::fov, 100, 1.f);
	}

	render_manager::draw_list->PopClipRect();
}