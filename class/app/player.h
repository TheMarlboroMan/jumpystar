#ifndef PLAYER_H
#define PLAYER_H

#include "motion_actor.h"
#include "drawable.h"
#include "player_input.h"

namespace App_Game
{

class Player:
	public Motion_actor,
	public App_Interfaces::Drawable
{
	public:

			Player();
	void		get_input(Player_input pi) {player_input=pi;}

	const t_box&	get_previous_position() const {return previous_position;}
	void		update_previos_position() {previous_position=get_box();}
	void		turn(float delta);
	void		latch_to_platform(const Spatiable& s);

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return 600.f;}
	virtual float 	get_max_fall_speed() const {return 600.f;}
	virtual void 	adjust_callback(float, Motion_actor::adjust_pos);

	////////////////////
	//Drawable

	virtual int 	get_draw_order() const {return 50;}
	virtual int	get_draw_cycle() const {return 0;}
	virtual void 	transform_draw_struct(Draw_struct&)const;
	virtual bool	is_draw_delete() const {return false;}

	private:

	enum class states{ground, air};

	t_box			previous_position;
	states			state;
	Spatiable const *	latched_platform;
	Player_input		player_input;
	

};

}

#endif
