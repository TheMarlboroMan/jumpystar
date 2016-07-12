#include "actor.h"

using namespace App_Game;
using namespace App_Interfaces;

Actor::Actor()
	:box{0.0, 0.0, 0, 0}
{
	
}

Actor::Actor(float x, float y, unsigned int w, unsigned int h)
	:box{x, y, w, h}
{

}

void Actor::set_box(t_box v)
{
	box=v;
}

void Actor::set_box(float x, float y, unsigned int w, unsigned int h)
{
	box.origen.x=x;
	box.origen.y=y;
	box.w=w;
	box.h=h;
}
