#include "bonus_slow_down.h"

using namespace app_game;

bonus_slow_down::bonus_slow_down():
	pickup()
{

}

void bonus_slow_down::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(51, 204, 51, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void bonus_slow_down::get_picked_up(player_effects& pe)
{
	pe.add_effect(app_game::player_effects::slow_down);
	set_delete(true);
}
