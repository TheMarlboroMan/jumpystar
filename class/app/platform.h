#ifndef PLATFORM_H
#define PLATFORM_H

#include "motion_actor.h"
#include "drawable.h"
#include "game_object.h"

namespace App_Game
{

class Platform:
	public Motion_actor,
	public App_Interfaces::Drawable,
	public App_Interfaces::Game_object
{

	public:

				Platform(float, float, int);

	void			turn(float);

	//////////////////////
	//Motion actor.
	virtual float 	get_weight() const {return 0.f;}
	virtual float 	get_max_fall_speed() const {return 0.f;}
	virtual void 	adjust_callback(float, adjust_pos) {}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const {return 30;}
	virtual int		get_draw_cycle()const {return 1;}
	virtual void 		transform_draw_struct(App_Game::Draw_struct &b)const;
	virtual bool		is_draw_delete()const {return false;}

	private:
};

}

#endif
