#ifndef PARABOL_ENEMY_H
#define PARABOL_ENEMY_H

#include "enemy.h"

namespace app_game
{

class parabol_enemy:
	public enemy
{
	public:

				parabol_enemy(ldt::point_2d<float>, float);

	//////////////////////
	//Enemy.
	void			do_turn(float);
	void			collide_with_player();
	void			get_jumped_on();

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const {return 25;}
	virtual int		get_draw_cycle()const {return 1;}
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const;
	virtual bool		is_draw_delete()const {return false;}

	////////////////////
	//Motion actor

	//TODO: Are these really part of the motion actor????. Should we try again?.

	virtual float 	get_weight() const {return 200.f;}
	virtual float 	get_max_fall_speed() const {return 200.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos) {}

	private:

	static const int	fixed_w=20,
				fixed_h=20;
	static const float	force;
};

}

#endif
