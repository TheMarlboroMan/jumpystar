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
	app_interfaces::spatiable::t_point		origin;
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

	bool			is_stunned() const {return state==states::stunned;}
	bool			is_trapped() const {return state==states::trapped;}

	virtual void		do_turn(float delta);
	virtual void		collide_with_player()=0;
	virtual void		get_jumped_on()=0;
	virtual void		get_trapped()=0;
	virtual bool		can_be_trapped() const=0;
	virtual bool		can_be_jumped_on() const=0;

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const=0;
	virtual int		get_draw_cycle()const=0;
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const=0;
	virtual bool		is_draw_delete()const=0;

	////////////////////
	//Motion actor

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;
	virtual void 	adjust_callback(float, motion_actor::adjust_pos)=0;

	protected:

	enum class states {regular, trapped, stunned}	state;

	states			get_state() const {return state;}
	void			stun(float sp) {state_period=sp; state=states::stunned;}
	void			trap(float sp) {state_period=sp; state=states::trapped;}
	void			force_turnaround();
	void 			limit_sideways_patrol(float, float);

	private:
	
	float			state_period;
};

}

#endif
