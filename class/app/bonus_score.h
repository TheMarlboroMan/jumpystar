#ifndef BONUS_SCORE_H
#define BONUS_SCORE_H

#include "pickup.h"

namespace app_game
{

class bonus_score:
	public pickup
{
	public:

				bonus_score();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual void 		transform_draw_struct(draw_control&) const;
	
	private:

	int			score;
};

}

#endif
