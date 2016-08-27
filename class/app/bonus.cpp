#include "bonus.h"

#include "definitions.h"

using namespace app_game;

bonus::bonus():
	actor(0, 0, fixed_w, fixed_h),
	game_object()
{

}

void bonus::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(130,160,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}

void bonus::get_picked_up(player_effects& pe)
{
	pe.add_score(30);
	pe.add_effect(app_game::player_effects::triple_jump);
	set_delete(true);
}
