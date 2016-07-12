#include "motion_actor.h"

using namespace App_Game;

/*
* @param float pos: punto en el espacio.
* @param adjust_pos tipo: tipo de ajuste.
*
* Por tipo de ajuste se entiende el "lado que se ajusta":
* 	arriba: el Motion_actor esta bajo de algo y ajustamos su parte de arriba pos.
* 	abajo: el Motion_actor esta sobre de algo y ajustamos su parte de abajo pos.
* 	izquierda: el Motion_actor esta a la derecha de algo y ajustamos su parte de izquierda pos.
* 	derecha: el Motion_actor esta a la izquierda de algo y ajustamos su parte de derecha pos.
*/

void Motion_actor::adjust(float pos, adjust_pos type)
{
	switch(type)
	{
		case adjust_pos::top: 		set_box_y(pos); break;
		case adjust_pos::bottom:	set_box_y(pos-get_spatiable_h()); break;
		case adjust_pos::left: 		set_box_x(pos); break;
		case adjust_pos::right: 	set_box_x(pos-get_spatiable_w()); break;
		case adjust_pos::none:		return; break;
	}

	adjust_callback(pos, type);
}

void Motion_actor::adjust(const Spatiable& e, adjust_pos type)
{
	float pos=0.0f;

	switch(type)
	{
		case adjust_pos::top: 		pos=e.get_spatiable_y()+e.get_spatiable_h(); break;
		case adjust_pos::bottom:	pos=e.get_spatiable_y(); break;
		case adjust_pos::left:	 	pos=e.get_spatiable_x()+e.get_spatiable_w(); break;
		case adjust_pos::right: 	pos=e.get_spatiable_x(); break;
		case adjust_pos::none:		return; break;
	}

	adjust(pos, type);
}

void Motion_actor::move(float delta)
{
	const auto& v=get_vector();
	displace_box(v.x * delta, v.y * delta);
}
