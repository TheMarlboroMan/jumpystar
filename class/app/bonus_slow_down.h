#ifndef BONUS_SLOW_DOWN_H
#define BONUS_SLOW_DOWN_H

#include "pickup.h"

namespace app_game
{

class bonus_slow_down:
	public pickup
{
	public:

				bonus_slow_down();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
};

}

#endif
