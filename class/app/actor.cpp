#include "actor.h"

using namespace app_game;
using namespace app_interfaces;

actor::actor()
	:box{0.0, 0.0, 0, 0}
{
	
}

actor::actor(float x, float y, unsigned int w, unsigned int h)
	:box{x, y, w, h}
{

}

void actor::set_box(t_box v)
{
	box=v;
}

void actor::set_box(float x, float y, unsigned int w, unsigned int h)
{
	box.origin.x=x;
	box.origin.y=y;
	box.w=w;
	box.h=h;
}
