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

			motion_actor(float x, float y, unsigned int w, unsigned int h, t_vector v={0.f, 0.f});
			motion_actor(const t_box&, t_vector v={0.f, 0.f});

	const t_box&	get_previous_position() const {return previous_position;}
	void		reset_previous_position() {previous_position=get_box();}
	void 		adjust(float, adjust_pos);
	void 		adjust(const spatiable&, adjust_pos);
	void 		move(float delta);
	t_box		get_below_position() const;

	//TODO: All motion actors could store their previous position. Yeah...

	/////////////////
	//motionable

	virtual float 	get_weight() const=0;
	virtual float 	get_max_fall_speed() const=0;

	/////////////////
	//A implementar, propio.
	virtual void 	adjust_callback(float position, adjust_pos)=0;
	
	private:
	
	t_box		previous_position;
};

}

#endif
