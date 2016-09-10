#include "projectile_straight.h"

using namespace app_game;

projectile_straight::projectile_straight(t_point pos, t_vector vec)
	:projectile(pos, vec, w, h)
{

}

void projectile_straight::collide_with_target()
{
	set_delete(true);
}

void projectile_straight::transform_draw_struct(draw_control &dc)const
{
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}
