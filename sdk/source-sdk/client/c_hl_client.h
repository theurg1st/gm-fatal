#pragma once

enum ñlient_frame_stage_t
{
	frame_undefined = -1,
	frame_start,
	frame_net_update_start,
	frame_net_update_postdataupdate_start,
	frame_net_update_postdataupdate_end,
	frame_net_update_end,
	frame_render_start,
	frame_render_end
};

class c_hl_client
{
public:
	c_client_class* get_all_classes()
	{
		return memory::call_v_function<c_client_class * (__thiscall*)(void*)>(this, 8)(this);
	}
};