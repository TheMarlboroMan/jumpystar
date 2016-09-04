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

				platform(float, float, int, int);

	virtual void		do_turn(float)=0;
	virtual bool		can_spawn_ground_based_enemies() const=0;
	virtual bool		can_spawn_bonus() const=0;
	virtual bool		is_collidable() const=0;

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const=0;
	virtual void 		transform_draw_struct(draw_control&) const=0;

	private:
};

}

#endif
