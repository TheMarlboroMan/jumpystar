#ifndef PLATFORM_CRUMBLING_H
#define PLATFORM_CRUMBLING_H

#include "platform.h"

#include <templates/linear_timed_function.h>
#include <class/number_generator.h>

namespace app_game
{
class platform_crumbling:
	public platform
{
	public:
				platform_crumbling(float, float, int);

	virtual void		get_jumped_on();
	virtual void		do_turn(float);
	virtual bool		can_spawn_ground_based_enemies() const {return false;}
	virtual bool		can_spawn_bonus() const {return true;}
	virtual bool		is_collidable() const {return true;}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 10;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	float					period;
	bool					crumbling;
	tools::linear_timed_function<float>	ltf;

	static const int			h=10;

};
}

#endif
