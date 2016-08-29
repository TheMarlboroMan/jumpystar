#ifndef BONUS_TRIPLE_JUMP_H
#define BONUS_TRIPLE_JUMP_H

#include "pickup.h"

namespace app_game
{

class bonus_triple_jump:
	public pickup
{
	public:

				bonus_triple_jump();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
};

}

#endif
