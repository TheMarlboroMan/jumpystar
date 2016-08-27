#include "projectile.h"

#include "definitions.h"

using namespace app_game;

projectile::projectile(t_point pos, t_vector vec)
	:motion_actor(pos.x, pos.y, w, h, vec),
	game_object()
{

}

void projectile::do_turn(float delta)
{
	move(delta);
	do_gravity(delta, app::definitions::default_gravity);
}

void projectile::collide_with_player()
{

}

void projectile::transform_draw_struct(draw_struct& b)const
{
	b.set_type(draw_struct::types::box);
	b.set_color(ldv::rgba8(255,0,0, 255));
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
}
