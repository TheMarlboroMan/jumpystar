#ifndef DRAWABLE_INTERFACE_H
#define DRAWABLE_INTERFACE_H

#include <libDan2.h>
#include "draw_struct.h"

namespace app_interfaces
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

class drawable
{
	public:

	virtual 		~drawable() {}
	virtual int		get_draw_order()const=0;
	virtual int		get_draw_cycle()const=0;
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const=0;
	virtual bool		is_draw_delete()const=0;


};

class drawable_order
{
	public: 

	bool operator()(const drawable* a, const drawable* b) const
	{
		return a->get_draw_order() < b->get_draw_order();
	}
};

}
#endif
