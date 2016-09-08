#ifndef DECORATION_H
#define DECORATION_H

#include "motion_actor.h"
#include "drawable.h"
#include "game_object.h"

//TODO: This and the base enemy could share a common ancestry... This could be a
//game_object_movable and the enemy would specialize it... Not that we would
//really be saving anything but well, we could have all decorations be game_object_movable
//and would save one class in the whole thing.

namespace app_game
{
class decoration:
	public motion_actor,
	public app_interfaces::drawable,
	public app_interfaces::game_object
{
	public:

				decoration(const t_box&);
	virtual void		do_turn(float)=0;

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const=0;
	virtual void 		transform_draw_struct(draw_control&) const=0;

	////////////////////
	//Motion actor

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;
	virtual void 	adjust_callback(float, motion_actor::adjust_pos)=0;
};
}

#endif
