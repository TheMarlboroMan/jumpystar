#include "spatiable.h"

using namespace App_Interfaces;

bool Spatiable::is_colliding_with(const Spatiable& e, bool roce_es_colision) const
{
	return get_box().es_en_colision_con(e.get_box(), roce_es_colision);
}

Spatiable::t_box Spatiable::get_box_displaced(float x, float y) const
{
	auto c=get_box();
	c.origen.x+=x;
	c.origen.y+=y;
	return c;
}

void Spatiable::set_position(const Spatiable& e)
{
	set_position(e.get_spatiable_x(), e.get_spatiable_y());
}

void Spatiable::set_position(float x, float y)
{
	set_box_x(x);
	set_box_y(y);
}

DLibH::Vector_2d_pantalla<float> Spatiable::screen_vector_for(const Spatiable& a, const Spatiable& b) const
{
	float ax=a.get_spatiable_cx();
	float ay=a.get_spatiable_cy();
	float bx=b.get_spatiable_cx();
	float by=b.get_spatiable_cy();

	return DLibH::obtener_para_puntos_pantalla<float>(bx, by, ax, ay);
}

float Spatiable::angle_for(const Spatiable& a, const Spatiable& b) const
{
	return screen_vector_for(a, b).angulo_grados();
}

DLibH::Vector_2d_cartesiano<float> Spatiable::cartesian_vector_for(const Spatiable& a, const Spatiable& b) const
{
	float ax=a.get_spatiable_cx();
	float ay=a.get_spatiable_cy();
	float bx=b.get_spatiable_cx();
	float by=b.get_spatiable_cy();

	return DLibH::obtener_para_puntos_cartesiano<float>(bx, by, ax, ay);
}

float Spatiable::cartesian_angle_for(const Spatiable& a, const Spatiable& b) const
{
	return cartesian_vector_for(a, b).angulo_grados();
}
