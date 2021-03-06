#include "motion_actor.h"

using namespace app_game;

motion_actor::motion_actor(float x, float y, unsigned int w, unsigned int h, t_vector v)
	:actor(x, y, w, h), app_interfaces::motionable(v), previous_position(get_box())
{

}

motion_actor::motion_actor(const t_box& pb, t_vector v)
	:actor(pb), app_interfaces::motionable(v)
{

}

/*
* @param float pos: punto en el espacio.
* @param adjust_pos tipo: tipo de ajuste.
*
* Por tipo de ajuste se entiende el "lado que se ajusta":
* 	arriba: el motion_actor esta bajo de algo y ajustamos su parte de arriba pos.
* 	abajo: el motion_actor esta sobre de algo y ajustamos su parte de abajo pos.
* 	izquierda: el motion_actor esta a la derecha de algo y ajustamos su parte de izquierda pos.
* 	derecha: el motion_actor esta a la izquierda de algo y ajustamos su parte de derecha pos.
*/

void motion_actor::adjust(float pos, adjust_pos type)
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

void motion_actor::adjust(const spatiable& e, adjust_pos type)
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

void motion_actor::move(float delta)
{
	previous_position=get_box();
	const auto& v=get_vector();
	displace_box(v.x * delta, v.y * delta);
}

app_interfaces::spatiable::t_box motion_actor::get_below_position() const
{
	auto res=get_box();
	++res.h;
	return res;
}
