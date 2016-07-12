#ifndef REPRESENTABLE_H
#define REPRESENTABLE_H

#include <libDan2.h>
#include "draw_struct.h"

extern DLibH::Log_base LOG;
/*
namespace App_Graficos
{
class Bloque_transformacion_representable;
}
*/

namespace App_Interfaces
{
/*Todos los elementos que vayan a ser representados como parte de un nivel
deben implementar la interface "representable". Uno de los métodos de la
misma consiste en recibir un Bloque_transformacion_representable y modificarlo
para luego pasarlo al representador.

El Bloque_transformacion_representable es una entidad que existe sólo una vez
para todas las cosas que se representan: es la alternativa a que cada entidad
representable tenga una copia de su representación.

El método "es_representable_borrar" está ahí para saber si el objeto debe
ser borrado de un hipotético vector de "representables". La implementación 
dependerá de la clase derivada.
*/

class Drawable
{
	public:

	virtual 		~Drawable() {}
	virtual int		get_draw_order()const=0;
	virtual int		get_draw_cycle()const=0;
	virtual void 		transform_draw_struct(App_Game::Draw_struct &b)const=0;
	virtual bool		is_draw_delete()const=0;


};

class Drawable_order
{
	public: 

	bool operator()(const Drawable* a, const Drawable* b) const
	{
		return a->get_draw_order() < b->get_draw_order();
	}
};

}
#endif
