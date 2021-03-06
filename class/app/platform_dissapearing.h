#ifndef PLATFORM_DISSAPEARING_H
#define PLATFORM_DISSAPEARING_H

#include "platform.h"

#include <templates/linear_timed_function.h>

namespace app_game
{
class platform_dissapearing:
	public platform
{
	public:
				platform_dissapearing(float, float, int);

	virtual void		get_jumped_on() {}
	virtual void		do_turn(float);
	virtual bool		can_spawn_ground_based_enemies() const {return false;}
	virtual bool		can_spawn_bonus() const {return true;}
	virtual bool		is_collidable() const {return state!=states::out;}
	virtual bool		is_bouncy() const {return false;}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 10;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	enum class states {in, trans_out, out, trans_in} state;

	//TODO Actually, we don't need this period variable when there's a linear timed function. Review elsewhere.
	float					period, max_period;
	tools::linear_timed_function<float>	ltf;

	static const int			h=10;

};
}

#endif
