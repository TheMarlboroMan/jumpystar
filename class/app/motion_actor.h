#ifndef ACTOR_MOVIL_H
#define ACTOR_MOVIL_H

#include "actor.h"
#include "motionable.h"

namespace App_Game
{

class Motion_actor:
	public Actor, 
	public App_Interfaces::Motionable
{
	public:

	enum class adjust_pos {none, top, bottom, left, right};

			Motion_actor(float x, float y, unsigned int w, unsigned int h)
		:Actor(x, y, w, h)
	{}

	void 		adjust(float, adjust_pos);
	void 		adjust(const Spatiable&, adjust_pos);
	void 		move(float delta);

	/////////////////
	//Motionable

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;

	/////////////////
	//A implementar, propio.
	virtual void 	adjust_callback(float position, adjust_pos)=0;
};

}

#endif
