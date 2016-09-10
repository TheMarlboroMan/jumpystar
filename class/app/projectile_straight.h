#ifndef PROJECTILE_STRAIGHT_H
#define PROJECTILE_STRAIGHT_H

#include "projectile.h"

namespace app_game
{

class projectile_straight:
	public projectile
{
	public:

				projectile_straight(t_point, t_vector);
	virtual void		collide_with_target();

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 40;}
	virtual void 		transform_draw_struct(draw_control&) const;

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return 0.f;}
	virtual float 	get_max_fall_speed() const {return 0.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos){}

	private:

	static const int	w=10, 
				h=10;
};

}

#endif
