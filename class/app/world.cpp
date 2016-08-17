#include "world.h"

#include <cassert>

#include <class/number_generator.h>

#include "definitions.h"

using namespace app_game;

world::world()
	:moving(false), distance(0.f), partial(0.f), speed(20.f),
	camera_movement(0), world_threshold(0)
{

}

void world::do_turn(float delta)
{
	//Considering the camera moves in integer values, we
	//will fake it a bit.

	float dist=delta*speed;
	speed+=delta;

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

/* At the beginning, generates the world with a large platform on the floor
and upwards to a fixed position, the equivalent of a whole screen up.
*/
void world::init()
{
	const float y=480.f;
	
	platforms.push_back({0.f,y,400});
	generate_new_world_threshold();

	do
	{
		generate_new_world();
	}while(world_threshold >= -480.f);
}

void world::generate_new_world_threshold()
{
	tools::int_generator next_diff_gen(1 , 3);
	world_threshold=platforms.back().get_spatiable_y()-(next_diff_gen()*app::definitions::unit);
}

void world::generate_new_world()
{
	create_new_platform(world_threshold);
	generate_new_world_threshold();

	//TODO: Generate bonus on the platform.

	//TODO: Generate obstacles in the previous position.
}

void world::reset()
{
	distance=0.f;
	partial=0.f;
	speed=0.f;
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
	const float 	cam_height=500.f,
			bottom_limit=distance-cam_height-s.get_spatiable_h()-extra;

	//Measure from the bottom of the object since it's negated.
	return bottom_limit > -s.get_spatiable_ey();
}

void world::delete_discarded_objects()
{
	auto it=std::remove_if(std::begin(platforms), std::end(platforms), [](const app_game::platform& p) {return p.is_delete();});
	platforms.erase(it, std::end(platforms));
}

/* Evaluates if the screen is near enough the world threshold. Distance is a positive
value while the threshold is negative.*/

bool world::is_create_new() const
{
	const int val=-(distance+app::definitions::distance_threshold);
	return val < world_threshold;
}

/** Platforms can exist between the horizontal world positions 0 and 
screen_w /definitions::unit. There is a maximum fixed distance between two 
platforms that should be roughly the horizontal distance the player can jump.
*/

void world::create_new_platform(float y)
{
	//First get the width.
	tools::int_generator width_generator(app::definitions::min_w_platform, app::definitions::max_w_platform);
	int w=width_generator();

	//Basic minimum and maximum range of horizontal position.
	float 	min_x=app::definitions::min_x_platform_position, 
		max_x=app::definitions::max_x_platform_position-w;

	//If there's a platform the range is recalculated near its edges.
	if(platforms.size() > 1) //The baseline platform does not count for this!.
	{		
		auto	last_platform=platforms.back();
		float 	left=(last_platform.get_spatiable_x() / app::definitions::unit),
			right=(last_platform.get_spatiable_ex() / app::definitions::unit);

		min_x=left-app::definitions::max_w_platform_gap;
		max_x=right+app::definitions::max_w_platform_gap-w;
	}

	//Account for the platform witdh at the rightmost side.
	assert(max_x >= min_x);

	//Clip to limits...
	if(min_x < app::definitions::min_x_platform_position) min_x=app::definitions::min_x_platform_position;
	if(max_x > app::definitions::max_x_platform_position) max_x=app::definitions::max_x_platform_position;

	//Generate and place.
	tools::int_generator position_generator(min_x, max_x);
	platforms.push_back({(float)position_generator()*app::definitions::unit,y,w*app::definitions::unit});
}

std::vector<app_interfaces::spatiable const *> world::get_collidables() const
{
	std::vector<app_interfaces::spatiable const *> res;
	for(const auto &p : platforms) res.push_back(&p);
	return res;
}

std::vector<app_interfaces::drawable const *> world::get_drawables() const
{
	std::vector<app_interfaces::drawable const *> res;
	for(const auto &p : platforms) res.push_back(&p);
	return res;
}
