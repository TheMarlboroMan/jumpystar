#ifndef ACTOR_H
#define ACTOR_H

#include "spatiable.h"

/*El actor es algo que existe en el espacio de juego.
Podemos encontrar con controladores que hacen cosas y que da igual su punto
en el espacio, pero bueno, no es importante.

Básicamente es el siguiente paso en el "espaciable", concretándulo en una
caja real.
*/

namespace app_game
{

class actor:
	public app_interfaces::spatiable
{
	public:

	enum class		faces{right, left};

				actor();
				actor(float x, float y, unsigned int w, unsigned int h);

	void			set_box(t_box);
	void 			set_box(float x, float y, unsigned int w, unsigned int h);

	///////////////////////
	// Spatiable

	public:

	virtual t_box 		get_box() const {return box;}
	virtual void 		set_box_x(float v) {box.origin.x=v;}
	virtual void 		set_box_y(float v) {box.origin.y=v;}
	virtual void 		set_box_w(unsigned int v) {box.w=v;}
	virtual void 		set_box_h(unsigned int v) {box.h=v;}
	virtual void 		displace_box(float x, float y) {box.origin.x+=x; box.origin.y+=y;}

	private:

	t_box 			box;
};

}
#endif
