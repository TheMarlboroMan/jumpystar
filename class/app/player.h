#ifndef PLAYER_H
#define PLAYER_H

#include "motion_actor.h"
#include "drawable.h"
#include "player_input.h"

namespace app_game
{

class player:
	public motion_actor,
	public app_interfaces::drawable
{
	public:

			player();
	void		get_input(player_input pi) {p_input=pi;}

	const t_box&	get_previous_position() const {return previous_position;}
	void		update_previos_position() {previous_position=get_box();}
	void		turn(float delta);
	void		set_falling();
	bool		can_fall() const {return state==states::ground;}
	bool		is_vulnerable() const {return state!=states::stunned;}
	void 		collide_with_harm_actor(const motion_actor&);
	void		reset();

	////////////////////
	//Motion actor

	//TODO: Check this, see if it works ok.
	virtual float 	get_weight() const {return cancel_jump && get_vector().y < 0.f ? 1200.f : 600.f;}
	virtual float 	get_max_fall_speed() const {return 600.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos);

	////////////////////
	//Drawable

	virtual int 	get_draw_order() const {return 50;}
	virtual int	get_draw_cycle() const {return 0;}
	virtual void 	transform_draw_struct(draw_struct&)const;
	virtual bool	is_draw_delete() const {return false;}

	private:

	enum class states{ground, air, stunned};

	t_box			previous_position;
	states			state, wakestate;
	player_input		p_input;
	int			remaining_jumps;
	bool			cancel_jump;
	float			stunned_time;
	
	//If the vector x is less than this and the player is hit, a larger vector is recieved.
	static const int	min_vector_hit_guard=30;

};

}

#endif
