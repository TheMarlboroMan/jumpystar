#include "bonus_score_multiplier.h"

using namespace app_game;

bonus_score_multiplier::bonus_score_multiplier():
	pickup()
{

}

void bonus_score_multiplier::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(32, 32, 32, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void bonus_score_multiplier::get_picked_up(player_effects& pe)
{
	pe.add_effect(app_game::player_effects::specials::score_multiplier);
	set_delete(true);
}
