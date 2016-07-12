#ifndef CONTROLLER_GAME_H
#define CONTROLLER_GAME_H

//Dependencies...
#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

//Tools.
#include <class/gestor_fuentes_ttf.h>
#include <templates/parches_compat.h>
#include <def_video.h>

//App specifics
#include "../app/player.h"
#include "../app/platform.h"

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

	void					do_world_turn(float);
	void					do_player_turn(float, App_Game::Player&, App_Game::Player_input);
	void 					do_player_collisions(App_Game::Player&);
	App_Game::Player_input			get_user_input(const DFramework::Input&);


	DLibH::Log_base&			log;

	DLibV::Camara				camera;
	App_Game::Draw_struct			draw_struct;
	App_Game::Player			player_instance;

	struct World
	{
		std::vector<App_Game::Platform>	platforms;
		bool				moving;
		float				distance;
		World():moving(false), distance(0.f) {}


		//TODO: Seems much better to me if the camera moves upwards all the time?
		//We can keep a real distance and make better position calculations.
		void	do_distance(float delta)
		{
			//TODO: Actually, this should be the vector of the platforms.
			distance+=delta*20.f;
			if(distance > 50.f)
			{
				distance=0.f;
				platforms.push_back({50.f,-8.f,200});
			}
		}

		void init()
		{
			int y=20;
			while(y < 600)
			{
				platforms.push_back({20.f,(float)y,100});
				y+=50;
			}

			platforms.push_back({0.f,580.f,400});
		}

		void reset()
		{
			distance=0;
			platforms.clear();
			moving=false;
		}
	}					world;
};

}

#endif
