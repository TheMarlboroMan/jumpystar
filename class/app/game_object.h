#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>

/**
* Interface que define un objeto de juego. Su objetivo es definir una clase
* base en la que meter todos los objetos de juego que luego puedan
* separarse usando un visitante.
*/

namespace app_interfaces
{
/**
* Interface en si.
*/

class game_object
{
	public:
					game_object();
	virtual 			~game_object() {}
	virtual bool			is_delete()const {return to_delete;}
	virtual void			set_delete(bool v) {to_delete=v;}

	private:

	bool 				to_delete;
};
}

#endif
