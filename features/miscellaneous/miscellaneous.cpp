#include "miscellaneous.h"

#define FCVAR_CHEAT 16384

void miscellaneous::run()
{
	c_base_entity* local_player = interfaces::entity_list->get_entity(interfaces::engine->get_local_player());
	if (!local_player || !local_player->is_alive())
		return;

	if (settings::aimbot::accuracy::disable_visual_recoil)
		local_player->get_punch_angle() = c_vector();

	if (settings::visuals::world::fullbright) {

	}
}

void miscellaneous::third_person(c_view_setup& view)
{
	c_base_entity* local_player = interfaces::entity_list->get_entity(interfaces::engine->get_local_player());
	if (!local_player)
		return;

	if (!settings::miscellaneous::globals::third_person::enable) {
		interfaces::input->camera_in_third_person = false;
		return;
	}

	if (!settings::miscellaneous::globals::third_person::hotkey.check()) {
		interfaces::input->camera_in_third_person = false;
		return;
	}

	//https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/game/client/in_camera.cpp#L59
	interfaces::input->camera_in_third_person = true;

	trace_t trace;
	c_trace_filter filter;
	filter.skip = local_player;

	c_vector dir;
	math::angle_to_vector(view.angles, dir);

	c_vector origin = view.origin + ((dir * -1) * settings::miscellaneous::globals::third_person::distance);

	ray_t ray(view.origin, origin);
	interfaces::engine_trace->trace_ray(ray, MASK_SOLID, &filter, &trace);

	view.origin = trace.end;
}

ImVec2 spec_size = ImVec2(200.f, 200.f);

void TextCentered(const char* text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text);
}

void miscellaneous::spectators_list()
{
	c_base_entity* local_player = interfaces::entity_list->get_entity(interfaces::engine->get_local_player());
	if (!settings::visuals::world::spectator_list || !local_player || !local_player->is_alive())
		return;

	ImGui::SetNextWindowSize(spec_size);
	ImGui::Begin(xorstr("Spectators window"), (bool*)true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX()+spec_size.x/2.5f-ImGui::CalcTextSize(xorstr("Spectators")).x/2);
		TextCentered(xorstr("Spectators"));

		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(settings::menu::colors::selection[0], settings::menu::colors::selection[1], settings::menu::colors::selection[2], settings::menu::colors::selection[3]));
		ImGui::Separator();
		ImGui::PopStyleColor();

		int spec_cnt = 0;
		std::string names = "";
		for (int i = 0; i < interfaces::entity_list->get_highest_entity_index(); i++)
		{
			c_base_entity* entity = (c_base_entity*)interfaces::entity_list->get_entity(i);
			if (entity == nullptr || !entity->is_player() || entity == local_player)
				continue;

			c_base_entity* obs_target = entity->get_observer_target();

			if (obs_target != local_player)
				continue;

			player_info_s info;
			interfaces::engine->get_player_info(i, &info);

			names += std::string(info.name) + "\n";
			spec_cnt += 1;
		}
		TextCentered(names.c_str());
		spec_size.y = (ImGui::CalcTextSize(xorstr("Spectators")).y+settings::menu::styles::margin)*(spec_cnt+1);
	}
	ImGui::End();
}


void miscellaneous::airstuck(c_user_cmd* cmd)
{
}