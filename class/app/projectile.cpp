#include "projectile.h"

#include "definitions.h"


using namespace app_game;

projectile::projectile(t_point pos, t_vector vec, int w, int h)
	:motion_actor(pos.x, pos.y, w, h, vec),
	game_object()
{

}

void projectile::do_turn(float delta)
{
	move(delta);
	do_gravity(delta, app::definitions::default_gravity);
}

