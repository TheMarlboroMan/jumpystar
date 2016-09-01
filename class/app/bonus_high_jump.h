#ifndef BONUS_HIGH_JUMP_H
#define BONUS_HIGH_JUMP_H

#include "pickup.h"

namespace app_game
{

class bonus_high_jump:
	public pickup
{
	public:

				bonus_high_jump();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
};

}

#endif
