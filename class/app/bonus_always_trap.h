#ifndef BONUS_ALWAYS_TRAP_H
#define BONUS_ALWAYS_TRAP_H

#include "pickup.h"

namespace app_game
{

class bonus_always_trap:
	public pickup
{
	public:

				bonus_always_trap();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
};

}

#endif
