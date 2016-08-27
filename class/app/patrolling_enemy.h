#ifndef PATROLLING_ENEMY_H
#define PATROLLING_ENEMY_H

#include "enemy.h"

namespace app_game
{

class patrolling_enemy:
	public enemy
{
	public:

				patrolling_enemy(float, float, float);

	//////////////////////
	//Enemy

	void			do_turn(float);
	void			collide_with_player();
	void			get_jumped_on();
	void			get_trapped();
	bool			can_be_trapped() const {return true;}
	bool			can_be_jumped_on() const {return true;}
	void			be_friendly(player_effects&);

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 25;}
	virtual void 		transform_draw_struct(draw_control&) const;

	////////////////////
	//Motion actor

	//TODO: Are these really part of the motion actor????. Should we try again?.

	virtual float 	get_weight() const {return 0.f;}
	virtual float 	get_max_fall_speed() const {return 0.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos) {}

	private:

	float			limit_left, limit_right;

	static const int	fixed_w=20,
				fixed_h=30;
};

}

#endif
