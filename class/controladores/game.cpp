#include "game.h"

#include "../app/framework_impl/input.h"
#include <templates/parches_compat.h>

#include "../app/player_input.h"

using namespace App;

Controller_game::Controller_game(DLibH::Log_base& log)
	:log(log),
	camera({0,0,400,600},{0,0})
{
	world.init();
}

void Controller_game::preloop(DFramework::Input& input, float delta)
{
	
}

void Controller_game::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::escape))
	{
		abandonar_aplicacion();
		return;
	}

	do_world_turn(delta);
	do_player_turn(delta, player_instance, get_user_input(input));
	do_player_collisions(player_instance);
}

void Controller_game::postloop(DFramework::Input& input, float delta)
{

}

void Controller_game::dibujar(DLibV::Pantalla& screen)
{
	screen.limpiar(DLibV::rgba8(255, 255, 255, 255));

	std::vector<App_Interfaces::Drawable const *> drawables;
	drawables.push_back(&player_instance);
	for(const auto& p : world.platforms) drawables.push_back(&p);

	App_Interfaces::Drawable_order dorder;
 	std::sort(std::begin(drawables), std::end(drawables), dorder);

	//TODO: Weirdest bug... Render just one thing and it fucks up.
	for(const auto& d : drawables)
	{
		d->transform_draw_struct(draw_struct);
		draw_struct.rep->volcar(screen, camera);
	}

	//TODO: Fix the above bug...
	DLibV::Representacion_primitiva_caja caja{DLibV::Representacion_primitiva_caja::tipo::relleno, {0,0,6,6}, DLibV::rgba8(255,0,0,128)};
	caja.volcar(screen);
}

void  Controller_game::despertar()
{

}

void  Controller_game::dormir()
{

}

bool Controller_game::es_posible_abandonar_estado() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Controller_game::do_player_turn(float delta, App_Game::Player& pl, App_Game::Player_input pi)
{
	if(!world.moving && pl.get_spatiable_y() < 450.f)
	{
		world.moving=true;
	}

	pl.get_input(pi);
	pl.turn(delta);
	pl.do_gravity(delta);
	pl.update_previos_position();
	pl.move(delta);
}

App_Game::Player_input Controller_game::get_user_input(const DFramework::Input& input)
{
	App_Game::Player_input pi{0, false};
	if(input.es_input_pulsado(Input::left)) pi.x=-1;
	else if(input.es_input_pulsado(Input::right)) pi.x=1;

	if(input.es_input_pulsado(Input::up)) pi.y=-1;
	else if(input.es_input_pulsado(Input::down)) pi.y=1;

	if(input.es_input_down(Input::jump)) pi.jump=true;

	return pi;
}

void Controller_game::do_player_collisions(App_Game::Player& pl)
{
	//Horizontal limits...
	if(pl.get_spatiable_x() < 0.f)
	{
		pl.adjust(0.f, App_Game::Motion_actor::adjust_pos::left);
	}
	else if(pl.get_spatiable_ex() > 400.f)
	{
		pl.adjust(400.f, App_Game::Motion_actor::adjust_pos::right);
	}

	//World.
	for(const auto& p : world.platforms)
	{
		if(pl.is_colliding_with(p))
		{
			if(p.is_under(pl.get_previous_position()))
			{
				pl.latch_to_platform(p);
			}
		}
	}
}

void Controller_game::do_world_turn(float delta)
{
	world.do_distance(delta);

	//TODO: How to change the vector of them all at the same time?.
	//EASY: LET THEM HAVE NO VECTOR AND HAVE THE CAMERA MOVING.
	if(world.moving)
	{
		for(auto& p : world.platforms)
		{
			p.move(delta);
			p.turn(delta);
		}

		//TODO: What happens when the platform the player is latched to dissapears?. 
		//We cannot check now because it's already deleted. I am sure you can
		//figure something out.

		auto it=std::remove_if(std::begin(world.platforms), std::end(world.platforms), [](const App_Game::Platform& p) {return p.is_delete();});
		world.platforms.erase(it, std::end(world.platforms));
	}



}
