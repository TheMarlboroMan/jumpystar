#include "world.h"

using namespace App_Game;

World::World()
	:moving(false), distance(0.f), partial(0.f), camera_movement(0)
{

}

void World::do_turn(float delta)
{
	//Considering the camera moves in integer values, we
	//will fake it a bit.

	float dist=delta*20.f;

	distance+=dist;
	partial+=dist;

	if(partial > 1.f)
	{
		float mov=0.f;
		partial=std::modf(partial, &mov);
		camera_movement=mov;
	}
	else 
	{
		camera_movement=0;
	}
}

void World::init()
{
	//TODO: This is terrible...
	int y=-500;
	while(y < 500)
	{
		platforms.push_back({20.f,(float)y,100});
		y+=50;
	}

	platforms.push_back({0.f,480.f,400});
}

void World::reset()
{
	distance=0.f;
	partial=0.f;
	platforms.clear();
	moving=false;
}

/*Consider the top of the camera begins at distance 0. As distance goes up,
the camera moves upwards the same negative position. Thus, knowing a static
camera height we can calculate if an object is out of bounds below.
*/

bool World::is_outside_bounds(const App_Interfaces::Spatiable& s) const
{
	//Measure from the top of the object.
	float y=-s.get_spatiable_y();
	const float cam_height=500.f;

std::cout<<"DIST:"<<distance<<" Y:"<<y<<std::endl;
	
	return false;
}
