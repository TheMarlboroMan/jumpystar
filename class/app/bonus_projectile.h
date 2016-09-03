#ifndef BONUS_PROJECTILE_H
#define BONUS_PROJECTILE_H

#include "pickup.h"

namespace app_game
{

class bonus_projectile:
	public pickup
{
	public:

				bonus_projectile();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
};

}

#endif
