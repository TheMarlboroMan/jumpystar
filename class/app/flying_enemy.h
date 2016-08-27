#ifndef FLYING_ENEMY_H
#define FLYING_ENEMY_H

#include "enemy.h"

namespace app_game
{

class flying_enemy:
	public enemy
{
	public:

				flying_enemy(float, float, float);

	//////////////////////
	//Enemy

	void			do_turn(float);
	void			collide_with_player();
	void			get_jumped_on();
	bool			can_be_trapped() const {return false;}
	bool			can_be_jumped_on() const {return false;}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const {return 25;}
	virtual int		get_draw_cycle()const {return 1;}
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const;
	virtual bool		is_draw_delete()const {return false;}

	////////////////////
	//Motion actor

	//TODO: Are these really part of the motion actor????. Should we try again?.

	virtual float 	get_weight() const {return 0.f;}
	virtual float 	get_max_fall_speed() const {return 0.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos) {}

	private:

	void			force_turnaround();

	float			limit_left, limit_right, period;

	static const int	fixed_w=30,
				fixed_h=10,
				vertical_sway=30;
	static const float	period_multiplier,
				y_vector_multiplier;
};

}

#endif
