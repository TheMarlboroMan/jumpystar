#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "motion_actor.h"
#include "drawable.h"
#include "game_object.h"

namespace app_game
{

class projectile:
	public motion_actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				projectile(t_point, t_vector, int, int);
	virtual void		do_turn(float delta);
	virtual void		collide_with_target()=0;

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const=0;
	virtual void 		transform_draw_struct(draw_control&) const=0;

	////////////////////
	//Motion actor

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;
	virtual void 	adjust_callback(float, motion_actor::adjust_pos)=0;
};

}

#endif
