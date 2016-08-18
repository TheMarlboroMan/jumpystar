#ifndef PATROLLING_ENEMY_H
#define PATROLLING_ENEMY_H

#include "motion_actor.h"
#include "drawable.h"
#include "game_object.h"

namespace app_game
{

class patrolling_enemy:
	public motion_actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object

{
	public:

				patrolling_enemy(float, float);
	void			do_turn(float);
	void			collide_with_player();

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

	float			limit_left, limit_right;

	static const int	fixed_w=20,
				fixed_h=30;
};

}

#endif
