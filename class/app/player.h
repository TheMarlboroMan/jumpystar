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

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return 600.f;}
	virtual float 	get_max_fall_speed() const {return 600.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos);

	////////////////////
	//Drawable

	virtual int 	get_draw_order() const {return 50;}
	virtual int	get_draw_cycle() const {return 0;}
	virtual void 	transform_draw_struct(draw_struct&)const;
	virtual bool	is_draw_delete() const {return false;}

	private:

	enum class states{ground, air};

	t_box			previous_position;
	states			state;
	player_input		p_input;
	

};

}

#endif
