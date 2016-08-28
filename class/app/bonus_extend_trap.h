#ifndef BONUS_EXTEND_TRAP_H
#define BONUS_EXTEND_TRAP_H

#include "pickup.h"

namespace app_game
{

class bonus_extend_trap:
	public pickup
{
	public:

				bonus_extend_trap();
	virtual void		get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 30;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	static const int	w=20,
				h=20;
};

}

#endif
