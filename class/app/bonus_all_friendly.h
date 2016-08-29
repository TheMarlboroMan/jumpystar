#ifndef BONUS_ALL_FRIENDLY_H
#define BONUS_ALL_FRIENDLY_H

#include "pickup.h"

namespace app_game
{

class bonus_all_friendly:
	public pickup
{
	public:

				bonus_all_friendly();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;

	private:
};

}

#endif
