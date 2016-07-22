#include "world.h"
#include <class/number_generator.h>

using namespace app_game;

world::world()
	:moving(false), distance(0.f), partial(0.f), 
	camera_movement(0), next_platform_diff(50)
{

}

void world::do_turn(float delta)
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

	//Now, let's see what platforms are out of bounds... There's a 
	//slight margin that allows for the world to persist outside the screen
	//for a bit.
	for(auto& p : platforms) p.set_delete(is_outside_bounds(p, 32.f));

	//And maybe create some other platforms...
	
}

void world::init()
{
	float y=480.f;
	platforms.push_back({0.f,y,400});

	do
	{
		create_new_platform(y);
		y-=50.f;
	}while(y >= 0.f);
}

void world::reset()
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

bool world::is_outside_bounds(const app_interfaces::spatiable& s, float extra) const
{
	const float 	cam_height=500.f;
	const float bottom_limit=distance-cam_height-s.get_spatiable_h()-extra;

	//Measure from the bottom of the object since it's negated.
	return bottom_limit > -s.get_spatiable_ey();
}

void world::delete_discarded_objects()
{
	auto it=std::remove_if(std::begin(platforms), std::end(platforms), [](const app_game::platform& p) {return p.is_delete();});
	platforms.erase(it, std::end(platforms));
}

void world::generate_new_world()
{
	//Measure the distance from the top of the last platform to climbed 
	//distance. This assumes that platforms are created bottom-top.

	const auto& last_platform=platforms.back();
	const int difference=distance-(-last_platform.get_spatiable_y());

	if(difference >= next_platform_diff)
	{
		tools::int_generator next_diff_gen(1, 3);
		next_platform_diff=next_diff_gen()*30;
		create_new_platform(-(std::round(distance)+10.f));

		//TODO: Take into account the previous position.

		//TODO: Generate bonus on the platform.
	}
}

void world::create_new_platform(float y)
{
	//First get the width.
	tools::int_generator width_generator(3, 8);
	int w=width_generator()*10;

	//Now the position...
	tools::int_generator position_generator(20, 400-20-w);

	//Now we place it.
	platforms.push_back({(float)position_generator(),y,w});
}
