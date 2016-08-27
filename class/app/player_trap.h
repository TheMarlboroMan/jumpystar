#ifndef PLATER_TRAP_H
#define PLAYER_TRAP_H

#include <templates/linear_timed_function.h>

#include "actor.h"
#include "drawable.h"
#include "game_object.h"

namespace app_game
{

class player_trap:
	public actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				player_trap(t_point);

	void			do_turn(float);
	void			reset(t_point);

	/////////////////
	//Drawable

	virtual int		get_draw_order() const {return 20;}
	virtual int		get_draw_cycle() const {return 1;}
	virtual void 		transform_draw_struct(app_game::draw_struct &b) const;
	virtual bool		is_draw_delete() const {return false;}

	private:

	float			period;
	tools::linear_timed_function<float>	ltf;

	static const int	max_period=3,
				w=15,
				h=15,
				max_draw_size=15,
				min_draw_size=5;
};

}

#endif
