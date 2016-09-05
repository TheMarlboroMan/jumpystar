#ifndef PLATFORM_BOUNCY_H
#define PLATFORM_BOUNCY_H

#include "platform.h"

namespace app_game
{
class platform_bouncy:
	public platform
{
	public:
				platform_bouncy(float, float, int);

	virtual void		get_jumped_on() {}
	virtual void		do_turn(float) {}
	virtual bool		can_spawn_ground_based_enemies() const {return true;}
	virtual bool		can_spawn_bonus() const {return true;}
	virtual bool		is_collidable() const {return true;}
	virtual bool		is_bouncy() const {return true;}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 10;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	static const int			h=10;

};
}

#endif
