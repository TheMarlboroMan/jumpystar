#ifndef KERNEL_DRIVER_INTERFACE
#define KERNEL_DRIVER_INTERFACE

#include <vector>
#include <string>
#include <stdexcept>

/**
* Esta es la interface que tendríamos que personalizar por cada aplicación 
* para arrancar el Kernel. La idea es dar un paso adelante y que sean menos
* archivos los que haya que manejar de un lado para otro.
* Controla los aspectos configurables del Kernel. No controla la configuración,
* que es una clase que puede tener tipos propios y que se usará aparte.
*/

namespace DFramework
{

struct Info_ventana
{
	int 		w_fisica, h_fisica;
	int 		w_logica, h_logica;
	std::string 	nombre;
	bool 		mostrar_cursor;
};

struct Par_input
{
	enum class 	tipos {nada, teclado, raton, joystick};
	tipos 		tipo;
	int 		clave, sdl_clave, indice_dispositivo;
};

class Kernel_driver_interface
{
	public:

	//Carga de recursos.
	virtual std::vector<std::string> 	obtener_entradas_audio() const=0;
	virtual std::vector<std::string> 	obtener_entradas_musica() const=0;
	virtual std::vector<std::string> 	obtener_entradas_texturas() const=0;
	virtual std::vector<std::string> 	obtener_entradas_superficies() const=0;

	//Información para ventana.
	virtual Info_ventana			obtener_info_ventana() const=0;

	//Información del input.
	virtual std::vector<Par_input> 		obtener_pares_input() const=0;
};

}

#endif
