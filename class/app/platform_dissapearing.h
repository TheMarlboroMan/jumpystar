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

	virtual void		do_turn(float);
	virtual bool		can_spawn_ground_based_enemies() const {return false;}
	virtual bool		can_spawn_bonus() const {return true;}
	virtual bool		is_collidable() const {return state!=states::out;}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 10;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	enum class states {in, trans_out, out, trans_in} state;
	float					period;
	tools::linear_timed_function<float>	ltf;

	static const int			h=10;

};
}

#endif
