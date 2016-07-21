#include "game.h"

#include "../app/framework_impl/input.h"
#include <templates/parches_compat.h>

#include "../app/player_input.h"

using namespace App;

Controller_game::Controller_game(DLibH::Log_base& log)
	:log(log),
	camera({0,0,400,500},{0,0}),
	world()
{
	reset();
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
	for(const auto& p : world.get_platforms()) drawables.push_back(&p);

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
	if(!world.is_moving() && pl.get_spatiable_y() < 350.f)
	{
		world.set_moving(true);
	}

	//TODO: Check if player is in air to change its state.
	
	pl.get_input(pi);
	pl.turn(delta);
	pl.do_gravity(delta);
	pl.update_previos_position();
	pl.move(delta);

	if(world.is_outside_bounds(pl))
	{
		reset();
	}
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
	//TODO: Add constants.
	if(pl.get_spatiable_x() < 0.f)
	{
		pl.adjust(0.f, App_Game::Motion_actor::adjust_pos::left);
	}
	else if(pl.get_spatiable_ex() > 400.f)
	{
		pl.adjust(400.f, App_Game::Motion_actor::adjust_pos::right);
	}

	//World.
	for(const auto& p : world.get_platforms())
	{
		if(pl.is_colliding_with(p))
		{
			if(p.is_under(pl.get_previous_position()))
			{
				pl.adjust(p, App_Game::Motion_actor::adjust_pos::bottom);
			}
		}
	}
}

void Controller_game::do_world_turn(float delta)
{
	if(world.is_moving())
	{
		world.do_turn(delta);
		world.generate_new_world();
		world.delete_discarded_objects();
		camera.movimiento_relativo(0, -world.get_camera_movement());

//		std::cout<<"THERE ARE "<<world.get_platforms().size()<<" PLATFORMS REMAINING"<<std::endl;
	}
}

void Controller_game::reset()
{
	world.reset();
	world.init();
	player_instance.set_position(20.f, 400.f);
	camera.enfocar_a({0,0});
}
