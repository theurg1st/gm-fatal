#pragma once
#include "../hooks/hooks.h"


struct m9k_info
{
	float       damage;
	const char* ammo;
	int         max_ricochet;
	int         max_penetration;
	bool        penetration;
	bool        ricochet;
};

struct swb_info
{
	double pen_str;
	double pen_mod;
};

namespace lua_utilities
{
	const char* get_name(c_base_entity* entity);
	const char* get_user_group(c_base_entity* entity);

	const char* get_rank_table_name(c_base_entity* entity);
	const char* get_rp_jobs_list(int team);
	ImVec4 get_rp_jobs_col(int team);
	int get_job_without_disguise(c_base_entity* entity);

	const char* get_team_name(c_base_entity* entity);
	ImVec4 get_team_col(c_base_entity* entity);
	int get_armor(c_base_entity* entity);
	const char* get_weapon_print_name(c_base_combat_weapon* weapon);
	const char* get_weapon_base(c_base_combat_weapon* weapon);

	float get_m9k_spread(c_user_cmd* cmd, c_base_combat_weapon* weapon);
	float get_ptp_spread(c_base_combat_weapon* weapon);
	float get_weapon_max_spread_inc(c_base_combat_weapon* weapon);
	float get_weapon_cur_cone(c_base_combat_weapon* weapon);

	bool build_m9k_info(c_base_combat_weapon* weapon, m9k_info& info);
	bool build_swb_info(c_base_combat_weapon* weapon, swb_info& info);

	const char* language_get_phrase(const char* phrase);
	void random_seed(float seed);
	float rand(float min, float max);
}