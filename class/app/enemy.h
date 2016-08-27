#ifndef ENEMY_H
#define ENEMY_H

#include "motion_actor.h"
#include "drawable.h"
#include "game_object.h"

namespace app_game
{

//Definition for a projectile... Not all enemies will shoot, but whatever
//makes it easier.
class projectile_def
{
	public:
	ldt::point_2d<float>				origin;
	app_interfaces::motionable::t_vector		direction;
	enum class types {curve}			type;
};

class enemy:
	public motion_actor, //Let's just assume all are capable of moving.
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				enemy(int w, int h);

	virtual void		do_turn(float delta)=0;
	virtual void		collide_with_player()=0;
	virtual void		get_jumped_on()=0;

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const=0;
	virtual int		get_draw_cycle()const=0;
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const=0;
	virtual bool		is_draw_delete()const=0;

	////////////////////
	//Motion actor

	//TODO: Are these really part of the motion actor????. Should we try again?.

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;
	virtual void 	adjust_callback(float, motion_actor::adjust_pos)=0;
};

}

#endif
