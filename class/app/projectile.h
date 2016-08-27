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

				projectile(t_point, t_vector);

	virtual void		do_turn(float delta);
	virtual void		collide_with_player();

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const {return 40;}
	virtual int		get_draw_cycle()const {return 1;}
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const;
	virtual bool		is_draw_delete()const {return false;}

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return 200.f;}
	virtual float 	get_max_fall_speed() const {return 300.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos){}

	private:

	static const int	w=10, 
				h=10;
};

}

#endif
