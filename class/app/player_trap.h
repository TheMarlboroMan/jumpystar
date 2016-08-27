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
	void			set_max_width(int v) {max_width=v;}
	static int		get_max_width() {return max_width;}

	/////////////////
	//Drawable

	virtual int		get_draw_order() const {return 20;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	float			period;
	tools::linear_timed_function<float>	ltf;

	static int		max_width;

	static const int	max_period=3,
				w=15,
				h=15,
				max_draw_size=15,
				min_draw_size=5;
};

}

#endif
