#ifndef BONUS_SCORE_MULTIPLIER_H
#define BONUS_SCORE_MULTIPLIER_H

#include "pickup.h"

namespace app_game
{

class bonus_score_multiplier:
	public pickup
{
	public:

				bonus_score_multiplier();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
};

}

#endif
