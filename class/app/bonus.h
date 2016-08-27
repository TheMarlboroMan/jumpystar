#ifndef BONUS_H
#define BONUS_H

#include "actor.h"
#include "drawable.h"
#include "game_object.h"
#include "player_effects.h"

namespace app_game
{

class bonus:
	public actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				bonus();
	void			get_picked_up(player_effects&);

	private:

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 30;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:

	static const int	fixed_w=20,
				fixed_h=20;
};

}

#endif
