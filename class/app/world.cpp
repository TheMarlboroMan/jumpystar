#include "world.h"
#include <templates/generador_numeros.h>

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

	//Now, let's see what platforms are out of bounds...
	for(auto& p : platforms) p.set_delete(is_outside_bounds(p));

	//And maybe create some other platforms...
	
}

void World::init()
{
	float y=480.f;
	platforms.push_back({0.f,y,400});

	do
	{
		create_new_platform(y);
		y-=50.f;
	}while(y >= 0.f);
}

void World::reset()
{
	distance=0.f;
	partial=0.f;
	platforms.clear();
	moving=false;
}

/*Consider the bottom of the camera begins at distance 0+cam_height. As distance 
goes up, the camera moves upwards the same negative position. An object 
positioned below the camera is considered to be outside bounds. The camera is 
linked to the "distance" variable so we can safely use it. The only thing is
that the distance goes into the positive while the camera goes into the 
negative.
*/

bool World::is_outside_bounds(const App_Interfaces::Spatiable& s) const
{
	//Measure from the top of the object.
	float y=-s.get_spatiable_y();
	const float cam_height=500.f;

	const float bottom_limit=distance-cam_height;

	return bottom_limit > y;
}

void World::delete_discarded_objects()
{
	auto it=std::remove_if(std::begin(platforms), std::end(platforms), [](const App_Game::Platform& p) {return p.is_delete();});
	platforms.erase(it, std::end(platforms));
}

void World::generate_new_world()
{
	//Measure the distance from the top of the last platform to climbed 
	//distance. This assumes that platforms are created bottom-top.

	const auto& last_platform=platforms.back();
	const float difference=distance-(-last_platform.get_spatiable_y());

	if(difference >= 50.f)
	{
		create_new_platform(-(std::round(distance)+10.f));
	}
}

void World::create_new_platform(float y)
{
	//First get the width.
	Herramientas_proyecto::Generador_int width_generator(3, 8);
	int w=width_generator()*10;

	//Now the position...
	Herramientas_proyecto::Generador_int position_generator(20, 400-20-w);

	//Now we place it.
	platforms.push_back({(float)position_generator(),y,w});
}