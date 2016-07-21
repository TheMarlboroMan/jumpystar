#ifndef CONTROLLER_GAME_H
#define CONTROLLER_GAME_H

#include <cmath>

//Dependencies...
#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

//Tools.
#include <class/gestor_fuentes_ttf.h>
#include <templates/parches_compat.h>
#include <def_video.h>

//App specifics
#include "../app/player.h"
#include "../app/world.h"

namespace App
{

class Controller_game:
	public DFramework::Controlador_interface
{
	public:

						Controller_game(DLibH::Log_base&);

	virtual void 				preloop(DFramework::Input& input, float delta);
	virtual void 				loop(DFramework::Input& input, float delta);
	virtual void 				postloop(DFramework::Input& input, float delta);
	virtual void 				dibujar(DLibV::Pantalla& pantalla);
	virtual void 				despertar();
	virtual void 				dormir();
	virtual bool				es_posible_abandonar_estado() const;

	private:

	void					reset();
	void					do_world_turn(float);
	void					do_player_turn(float, App_Game::Player&, App_Game::Player_input);
	void 					do_player_collisions(App_Game::Player&);
	App_Game::Player_input			get_user_input(const DFramework::Input&);


	DLibH::Log_base&			log;

	DLibV::Camara				camera;
	App_Game::World				world;
	App_Game::Draw_struct			draw_struct;
	App_Game::Player			player_instance;
};

}

#endif
