#include "platform.h"

#include "definitions.h"

using namespace app_game;

platform::platform(float x, float y , int w):
	actor(x, y, w, app::definitions::unit),
	game_object()
{

}

void platform::transform_draw_struct(app_game::draw_struct &b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(130,0,160, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h() / 2});
}
