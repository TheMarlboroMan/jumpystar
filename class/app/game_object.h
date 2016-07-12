#ifndef OBJETO_JUEGO_H
#define OBJETO_JUEGO_H

#include <iostream>

/**
* Interface que define un objeto de juego. Su objetivo es definir una clase
* base en la que meter todos los objetos de juego que luego puedan
* separarse usando un visitante.
*/

namespace App_Interfaces
{
/**
* Interface en si.
*/

class Game_object
{
	public:
					Game_object();
	virtual 			~Game_object() {}
	virtual bool			is_delete()const {return to_delete;}
	virtual void			set_delete(bool v) {to_delete=v;}

	private:

	bool 				to_delete;
};
}

#endif
