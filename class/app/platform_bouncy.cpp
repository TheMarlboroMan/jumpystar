#include "platform_bouncy.h"

#include <vector>

using namespace app_game;

platform_bouncy::platform_bouncy(float x, float y , int w)
	:platform(x, y, w, h)
{

}

void platform_bouncy::transform_draw_struct(draw_control& dc)const
{
	dc.set(1);
	auto& b=dc[0];
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(0,130,160, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h() / 2});
}
