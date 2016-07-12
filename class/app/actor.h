#ifndef ACTOR_H
#define ACTOR_H

#include "spatiable.h"

/*El actor es algo que existe en el espacio de juego.
Podemos encontrar con controladores que hacen cosas y que da igual su punto
en el espacio, pero bueno, no es importante.

Básicamente es el siguiente paso en el "espaciable", concretándulo en una
caja real.
*/

namespace App_Game
{

class Actor:
	public App_Interfaces::Spatiable
{
	public:

				Actor();
				Actor(float x, float y, unsigned int w, unsigned int h);

	void			set_box(t_box);
	void 			set_box(float x, float y, unsigned int w, unsigned int h);

	///////////////////////
	// Spatiable

	public:

	virtual t_box 		get_box() const {return box;}
	virtual void 		set_box_x(float v) {box.origen.x=v;}
	virtual void 		set_box_y(float v) {box.origen.y=v;}
	virtual void 		set_box_w(unsigned int v) {box.w=v;}
	virtual void 		set_box_h(unsigned int v) {box.h=v;}
	virtual void 		displace_box(float x, float y) {box.origen.x+=x; box.origen.y+=y;}

	private:

	t_box 			box;
};

}
#endif
