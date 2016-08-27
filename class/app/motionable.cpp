#include "motionable.h"

using namespace app_interfaces;

motionable::motionable(t_vector v)
	:vector(v)
{

}

motionable::~motionable()
{

}

float motionable::integrate_vector(float delta, float &vec, float factor)
{
	float copy=vec;
	vec+=factor * delta;
	float val=copy + vec;
	return val * 0.5 * delta;
}

void motionable::add_vector(float c, axis t)
{
	switch(t)
	{
		case axis::x: vector.x+=c; break;
		case axis::y: vector.y+=c; break;
	}
}

void motionable::set_vector(float c, axis t)
{
	switch(t)
	{
		case axis::x: vector.x=c; break;
		case axis::y: vector.y=c; break;
	}
}

void motionable::do_gravity(float delta, float gravity_value)
{
	integrate_vector(delta, vector.y, get_weight()*gravity_value);
	float max_v_c=get_max_fall_speed();
	if(vector.y > max_v_c) vector.y=max_v_c;
}

