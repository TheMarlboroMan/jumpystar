#ifndef PICKUP_H
#define PICKUP_H

#include "actor.h"
#include "drawable.h"
#include "game_object.h"
#include "player_effects.h"

namespace app_game
{

class pickup:
	public actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				pickup();
	virtual void		get_picked_up(player_effects&)=0;

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 30;}
	virtual void 		transform_draw_struct(draw_control&) const=0;

	private:

	static const int	w=20,
				h=20;
};

}

#endif
