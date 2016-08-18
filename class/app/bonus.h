#ifndef BONUS_H
#define BONUS_H

#include "actor.h"
#include "drawable.h"
#include "game_object.h"

namespace app_game
{

class bonus:
	public actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				bonus();

	private:

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const {return 30;}
	virtual int		get_draw_cycle()const {return 1;}
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const;
	virtual bool		is_draw_delete()const {return false;}

	private:

	static const int	fixed_w=20,
				fixed_h=20;
};

}

#endif
