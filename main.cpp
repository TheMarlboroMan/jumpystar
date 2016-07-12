#ifdef WINCOMPIL
//Terrible parche para SDL2_Gfx...
#include <SDL2_gfxPrimitives.c>
#include <SDL2_rotozoom.c>
#include <SDL2_imageFilter.c>
#endif

#include <libDan2.h>
#include <defDanSDL.h>
#include "class/framework/kernel.h"
#include "class/app/framework_impl/kernel_config.h"
#include "class/app/framework_impl/app_config.h"
#include "class/controladores/director_estados.h"

//Declaración del log de aplicación en espacio global.
DLibH::Log_base LOG;

int main(int argc, char ** argv)
{
	using namespace App;

	//Inicializar control de logs.
	LOG.inicializar("logs/global.log");
	LOG.activar();
	DLibH::Log_motor::arrancar("logs/libdansdl2.log");

	//Inicializar control de argumentos.
	Herramientas_proyecto::Controlador_argumentos CARG(argc, argv);

	DLibH::Log_base log_app("logs/app.log");
	log_app<<"Init main process..."<<std::endl;

	//Intentar inicializar sin cargar aún la SDL...
	try
	{
		log_app<<"Init application config..."<<std::endl;
		App_config config;

		//Si la inicialización ha tenido éxito podemos arrancar la SDL y el Kernel.
		log_app<<"Init kernel config..."<<std::endl;
		Kernel_config kernel_config(config);

		log_app<<"Init SDL2..."<<std::endl;	
		if(!DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
		{
			throw std::runtime_error("Unable to init SD2L");
		}
		
		log_app<<"Creating kernel..."<<std::endl;
		DFramework::Kernel kernel(CARG);

		log_app<<"Init kernel..."<<std::endl;
		kernel.inicializar(kernel_config, config);

		log_app<<"Init state director..."<<std::endl;
		App::Director_estados APP(kernel, config, log_app);
		APP.iniciar(kernel);

		log_app<<"Main process ends..."<<std::endl;
	}
	catch(std::exception& e)
	{
		log_app<<"ERROR "<<e.what()<<std::endl;
		std::cout<<"Exiting due to error : "<<e.what()<<std::endl;

		log_app<<"Closing SDL2..."<<std::endl;
		DLibH::Herramientas_SDL::apagar_SDL();
		DLibH::Log_motor::finalizar();

		return 1;
	}

	log_app<<"Finalizando SDL2..."<<std::endl;
	DLibH::Herramientas_SDL::apagar_SDL();
	DLibH::Log_motor::finalizar();
	return 0;
}
