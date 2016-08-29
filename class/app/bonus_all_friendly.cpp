#include "bonus_all_friendly.h"

using namespace app_game;

bonus_all_friendly::bonus_all_friendly():
	pickup()
{

}

void bonus_all_friendly::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(245, 120, 176, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void bonus_all_friendly::get_picked_up(player_effects& pe)
{
	pe.add_effect(app_game::player_effects::all_friendly);
	set_delete(true);
}
