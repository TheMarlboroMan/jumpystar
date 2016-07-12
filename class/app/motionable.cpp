#include "motionable.h"

using namespace App_Interfaces;

Motionable::Motionable()
	:vector(0.0, 0.0)
{

}

Motionable::~Motionable()
{

}

float Motionable::integrate_vector(float delta, float &vec, float factor)
{
	float copy=vec;
	vec+=factor * delta;
	float val=copy + vec;
	return val * 0.5 * delta;
}

void Motionable::add_vector(float c, axis t)
{
	switch(t)
	{
		case axis::x: vector.x+=c; break;
		case axis::y: vector.y+=c; break;
	}
}

void Motionable::set_vector(float c, axis t)
{
	switch(t)
	{
		case axis::x: vector.x=c; break;
		case axis::y: vector.y=c; break;
	}
}

void Motionable::do_gravity(float delta, float gravity_value)
{
	integrate_vector(delta, vector.y, get_weight()*gravity_value);
	float max_v_c=get_max_fall_speed();
	if(vector.y > max_v_c) vector.y=max_v_c;
}

