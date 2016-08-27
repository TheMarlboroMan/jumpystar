#ifndef CONTROLLER_GAME_H
#define CONTROLLER_GAME_H

//std
#include <cmath>

//libdansdl2
#include <def_video.h>

//framework
#include <class/controller_interface.h>

//Tools.
#include <class/ttf_manager.h>

//local
#include "states.h"
#include "../app/player.h"
#include "../app/world.h"

namespace app
{

class game_controller:
	public dfw::controller_interface
{
	public:

						game_controller(ldt::log&, ldv::resource_manager&, tools::ttf_manager&);

	virtual void 				preloop(dfw::input& input, float delta, int);
	virtual void 				loop(dfw::input& input, float delta);
	virtual void 				postloop(dfw::input& input, float delta, int);
	virtual void 				draw(ldv::screen& screen);
	virtual void 				awake();
	virtual void 				slumber();
	virtual bool				can_leave_state() const;

	private:

	void					reset();
	void					do_world_turn(float);
	void					do_player_turn(float, app_game::player&, app_game::player_input);
	void 					do_player_collisions(app_game::player&);
	app_game::player_input			get_user_input(const dfw::input&);


	ldt::log&				log;

	ldv::camera				camera;
	ldv::ttf_representation			fps_text,
						distance_text;
	app_game::player			player_instance;
	app_game::world				world;
	app_game::draw_struct			draw_struct;
};

}

#endif
