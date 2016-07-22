#ifndef MOTION_ACTOR_H
#define MOTION_ACTOR_H

#include "actor.h"
#include "motionable.h"

namespace app_game
{

class motion_actor:
	public actor, 
	public app_interfaces::motionable
{
	public:

	enum class adjust_pos {none, top, bottom, left, right};

			motion_actor(float x, float y, unsigned int w, unsigned int h)
		:actor(x, y, w, h)
	{}

	void 		adjust(float, adjust_pos);
	void 		adjust(const spatiable&, adjust_pos);
	void 		move(float delta);

	/////////////////
	//motionable

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;

	/////////////////
	//A implementar, propio.
	virtual void 	adjust_callback(float position, adjust_pos)=0;
};

}

#endif
