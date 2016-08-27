#ifndef PLATFORM_H
#define PLATFORM_H

#include "actor.h"
#include "drawable.h"
#include "game_object.h"

namespace app_game
{

class platform:
	public actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{

	public:

				platform(float, float, int);

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 30;}
	virtual void 		transform_draw_struct(draw_control&) const;

	private:
};

}

#endif
