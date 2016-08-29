#include "bonus_extend_trap.h"

using namespace app_game;

bonus_extend_trap::bonus_extend_trap():
	pickup()
{

}

void bonus_extend_trap::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(120, 176, 245, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void bonus_extend_trap::get_picked_up(player_effects& pe)
{
	pe.add_effect(app_game::player_effects::extend_trap);
	set_delete(true);
}
