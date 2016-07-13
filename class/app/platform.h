#ifndef PLATFORM_H
#define PLATFORM_H

#include "actor.h"
#include "drawable.h"
#include "game_object.h"

namespace App_Game
{

class Platform:
	public Actor,
	public App_Interfaces::Drawable,
	public App_Interfaces::Game_object
{

	public:

				Platform(float, float, int);

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
