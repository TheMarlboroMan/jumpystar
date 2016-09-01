#include "bonus_high_jump.h"

using namespace app_game;

bonus_high_jump::bonus_high_jump():
	pickup()
{

}

void bonus_high_jump::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255, 163, 102, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void bonus_high_jump::get_picked_up(player_effects& pe)
{
	pe.add_effect(app_game::player_effects::high_jump);
	set_delete(true);
}
