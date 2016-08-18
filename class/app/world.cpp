#include "world.h"

#include <cassert>

//#include <class/number_generator.h>

//#include "definitions.h"

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
	evaluate_new_bonus();
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
	delete_helper(platforms);
	delete_helper(bonus);
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

	//TODO: Perhaps we should favour the edges instead of the centered positions.
	//The inverse normal (gauss) distribution would work but that's trash on windows.

	//If there's a platform the range is recalculated near its edges.
	if(platforms.size() > 1) //The baseline platform does not count for this!.
	{		
		auto	last_platform=platforms.back();
		float 	left=app::world_to_grid(last_platform.get_spatiable_x()),
			right=app::world_to_grid(last_platform.get_spatiable_ex());

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

/** Evaluates wheter or not to create a bonus over a newly created platform. 
The rules are as follows:

- No bonus can exist on the world's lowest platform.
- The wider the last platform, the more possibilities a bonus appears.
- Bonus change grows as more platforms are created and resets when a bonus is created.
*/

void world::evaluate_new_bonus()
{
	if(platforms.size()==1)
	{
		return;
	}

	bonus_data.chance+=app::world_to_grid(platforms.back().get_spatiable_w());

	if(bonus_data.generator() <= bonus_data.chance)
	{
		bonus_data.reset_chance();
		create_new_bonus();
	}
}

/** By default the new bonus is created on the 0,0 position and then is placed
alongside the horizontal length of the last platform, on its top.
*/

void world::create_new_bonus()
{
	app_game::bonus b;

	auto	last_platform=platforms.back();
	float 	left=last_platform.get_spatiable_x(),
		right=last_platform.get_spatiable_ex()-b.get_spatiable_w();

	tools::int_generator x_generator(left, right);
	float x=x_generator();
	float y=last_platform.get_spatiable_y()-b.get_spatiable_h()-app::definitions::bonus_units_above_ground;

	b.set_position(x, y);
	bonus.push_back(b);
}

std::vector<app_interfaces::drawable const *> world::get_drawables() const
{
	std::vector<app_interfaces::drawable const *> res;
	for(const auto &p : platforms) res.push_back(&p);
	for(const auto &p : bonus) res.push_back(&p);
	return res;
}

//TODO: app_game::bonus would be replaced but some interface... In any other case
//we could just return the vector by reference and be done.

std::vector<app_game::bonus *> world::get_pickables()
{
	std::vector<app_game::bonus *> res;
	for(auto &p : bonus) res.push_back(&p);
	return res;
}
