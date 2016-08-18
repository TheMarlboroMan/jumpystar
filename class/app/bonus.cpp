#include "bonus.h"

#include "definitions.h"

using namespace app_game;

bonus::bonus():
	actor(0, 0, fixed_w, fixed_h),
	game_object()
{

}

void bonus::transform_draw_struct(app_game::draw_struct &b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(130,160,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}
