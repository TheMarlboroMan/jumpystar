#include "world.h"

#include <cassert>

#include "bonus_score.h"
#include "bonus_triple_jump.h"
#include "bonus_extend_trap.h"
#include "bonus_high_jump.h"
#include "bonus_score_multiplier.h"

#include "patrolling_enemy.h"
#include "parabol_enemy.h"
#include "parabol_shooter_enemy.h"
#include "flying_enemy.h"

//#include <class/number_generator.h>

//#include "definitions.h"

using namespace app_game;

world::world(const app_interfaces::spatiable& ppos)
	:player_position(ppos), 
	moving(false), slowdown(false), distance(0.f), partial(0.f), speed(20.f), game_speed_multipler(1.f),
	camera_movement(0), world_threshold(0),
	bonus_chance_calculator(app::definitions::base_bonus_chance, app::definitions::min_bonus_percentage, app::definitions::max_bonus_percentage),
	//TODO: Use other values..
	enemy_chance_calculator(app::definitions::base_bonus_chance, app::definitions::min_bonus_percentage, app::definitions::max_bonus_percentage)
{

}

void world::do_turn(float delta)
{
	if(slowdown_process)
	{
		if(slowdown)
		{
			game_speed_multipler-=delta / 2.f;
			if(game_speed_multipler <= 0.5f) 
			{
				game_speed_multipler=0.5f;
				slowdown_process=false;
			}
		}
		else
		{
			game_speed_multipler+=delta / 2.f;
			if(game_speed_multipler > 1.f) 
			{
				game_speed_multipler=1.f;
				slowdown_process=false;
			}
		}
	}

	delta *= game_speed_multipler;

	//Evaluate possible deletions... traps are not discarded this way!.

	for(auto& e : enemies) 
		if(is_outside_bounds(*e, 32.f)) 
			e->set_delete(true);

	for(auto& b : pickups)
		if(is_outside_bounds(*b, 32.f))
			b->set_delete(true);

	for(auto& p : projectiles)
		if(is_outside_bounds(p, 32.f))
			p.set_delete(true);

	
	if(player_traps.size()==max_player_traps)
	{
		trigger_player_traps();
	}

	delete_discarded_objects();

	for(auto& t : player_traps) t.do_turn(delta);
	for(auto& e: enemies) e->do_turn(delta);
	for(auto& p: projectiles) p.do_turn(delta);

	//If someone shot, turn the definitions into real projectiles.
	if(projectile_definitions.size()) create_projectiles();

	if(moving)
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
		if(is_create_new())
		{
			generate_new_world();
		}
	}

	//There may be bonuses to pick and stuff to delete even if the world
	//is still.
	delete_discarded_objects();
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
	evaluate_new_enemy();
}

void world::reset()
{
	distance=0.f;
	partial=0.f;
	speed=0.f;
	game_speed_multipler=1.f;
	platforms.clear();
	enemies.clear();
	pickups.clear();
	projectiles.clear();
	projectile_definitions.clear();
	player_traps.clear();
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
	delete_helper_ptr(pickups);
	delete_helper(projectiles);
	delete_helper(player_traps);
	delete_helper_ptr(enemies);
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
		float 	left=app::world_to_grid(last_platform.get_spatiable_x()),
			right=app::world_to_grid(last_platform.get_spatiable_ex());

		min_x=left-app::definitions::max_w_platform_gap;
		max_x=right+app::definitions::max_w_platform_gap-w;

		//Clip to limits...
		if(min_x < app::definitions::min_x_platform_position) min_x=app::definitions::min_x_platform_position;
		if(max_x+w > app::definitions::max_x_platform_position) max_x=app::definitions::max_x_platform_position-w;
	}

	//Generate and place.
	tools::int_generator position_generator(min_x, max_x);
	int pos=position_generator();
	float 	x_pos=pos*app::definitions::unit,
		w_pos=w*app::definitions::unit;

	platforms.push_back({x_pos,y,(int)w_pos});

	//TODO Fix this bug.
	assert(platforms.back().get_spatiable_ex() <= app::definitions::playground_width);
}

//TODO: Filter below the line.
std::vector<app_interfaces::drawable const *> world::get_drawables() const
{
	std::vector<app_interfaces::drawable const *> res;
	for(const auto &p : platforms) res.push_back(&p);
	for(const auto &p : pickups) res.push_back(p.get());
	for(const auto &p : enemies) res.push_back(p.get());
	for(const auto &p : projectiles) res.push_back(&p);
	for(const auto &p : player_traps) res.push_back(&p);

	return res;
}

std::vector<app_interfaces::spatiable const *> world::get_collidables() const
{
	std::vector<app_interfaces::spatiable const *> res;
	for(auto& p : platforms) 
		if(p.get_spatiable_ey() >= -distance)
			res.push_back(&p);
	return res;
}

std::vector<enemy *> world::get_enemies()
{
	std::vector<enemy *> res;
	for(auto& p : enemies) 
		if(p->get_spatiable_ey() >= -distance)
			res.push_back(p.get());
	return res;
}

std::vector<projectile *> world::get_projectiles()
{
	std::vector<projectile *> res;
	for(auto& p : projectiles)
		if(p.get_spatiable_ey() >= -distance)
			res.push_back(&p);
	return res;
}

std::vector<pickup *> world::get_pickups()
{
	std::vector<pickup *> res;
	for(auto& p : pickups) 
		if(p->get_spatiable_ey() >= -distance)
			res.push_back(p.get());

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

	bonus_chance_calculator.increment(app::world_to_grid(platforms.back().get_spatiable_w()));
	if(bonus_chance_calculator.evaluate())
	{
		bonus_chance_calculator.reset();
		create_new_bonus();
	}
}

/** By default the new bonus is created on the 0,0 position and then is placed
alongside the horizontal length of the last platform, on its top.
*/

void world::create_new_bonus()
{
	enum class types{score, triple_jump, extend_trap, high_jump, score_multiplier};
	std::vector<types> t{types::score, types::triple_jump, types::extend_trap, types::high_jump, types::score_multiplier};

	std::unique_ptr<pickup> b{nullptr};

	tools::int_generator gen(0, t.size()-1);

	if(t.size()) switch(t[gen()])
//	switch(types::high_jump)
	{
		case types::score:		b.reset(new bonus_score()); break;
		case types::triple_jump:	b.reset(new bonus_triple_jump()); break;
		case types::extend_trap:	b.reset(new bonus_extend_trap()); break;
		case types::high_jump:		b.reset(new bonus_high_jump()); break;
		case types::score_multiplier:	b.reset(new bonus_score_multiplier()); break;
	}	

	auto	last_platform=platforms.back();
	float 	left=last_platform.get_spatiable_x(),
		right=last_platform.get_spatiable_ex()-b->get_spatiable_w();

	tools::int_generator x_generator(left, right);
	float x=x_generator();
	float y=last_platform.get_spatiable_y()-b->get_spatiable_h()-app::definitions::bonus_units_above_ground;

	if(b) 
	{
		b->set_position(x, y);
		pickups.push_back(std::move(b));
	}
}

/** By default the new patrolling enemy is placed on 0,0 and later is placed at the center
top of the last platform, facing the direction set on its constructor.
*/

void world::create_new_enemy()
{
	enum class types {patrolling, parabol, flying, parabol_shooter};
	
	std::vector<types> t;
	//TODO: Rework this values.
	if(true || distance > 20.f) t.push_back(types::patrolling);
	if(true || distance > 100.f) t.push_back(types::flying);
	if(true || distance > 300.f) t.push_back(types::parabol_shooter);
	if(true || distance > 300.f) t.push_back(types::parabol);

	tools::int_generator gen(0, t.size()-1);

	std::unique_ptr<enemy> e{nullptr};
	const auto&	last_platform=platforms.back();

	if(t.size()) switch(t[gen()])
//	switch(types::parabol_shooter)
	{
		case types::patrolling:
		{
			float 	left=last_platform.get_spatiable_x(),
				right=last_platform.get_spatiable_ex();
			e.reset(new patrolling_enemy{left, right, last_platform.get_spatiable_y()});
		}
		break;
		case types::parabol:
			e.reset(new parabol_enemy(player_position.get_spatiable_position(), app::definitions::playground_height-distance));
		break;
		case types::flying:
			e.reset(new flying_enemy{0.f, app::definitions::playground_width, last_platform.get_spatiable_y()});
		break;
		case types::parabol_shooter:
			e.reset(new parabol_shooter_enemy(projectile_definitions, player_position, last_platform));
		break;
	}

	if(e.get()) enemies.push_back(std::move(e));
}

/** Evaluates if an enemy is to be created on top of last platform according to
* these rules...
* - No enemies should appear before a certain height is reached.
* - As with bonus generators, there's a chance that increases as platforms are
* created.
*/

void world::evaluate_new_enemy()
{
	auto last_platform=platforms.back();

	//TODO: Change this...
	if(platforms.size()==1 || app::world_to_grid(last_platform.get_spatiable_w()) <= 2)
	{
		return;
	}

	enemy_chance_calculator.increment(app::world_to_grid(last_platform.get_spatiable_w()));
	if(enemy_chance_calculator.evaluate())
	{
		enemy_chance_calculator.reset();
		create_new_enemy();
	}	
}

/** Projectiles will only be created below the "distance" line.
*/

void world::create_projectiles()
{
	for(const auto& pd : projectile_definitions) 
	{
		if(pd.origin.y > -distance)
		{
			projectiles.push_back({pd.origin, pd.direction});
		}
	}
	
	projectile_definitions.clear();
}

/** Only two traps can be set at once. When there's more than two, the second
replaces the first and a new one is set on its place.
*/

void world::set_player_trap(const app_interfaces::spatiable& player, const app_interfaces::spatiable& platform)
{
	if(player_traps.size() < max_player_traps)
	{
		//Y position will be transformed by the heart itself.
		player_traps.push_back({{player.get_spatiable_cx(), platform.get_spatiable_y()}});
	}
	else
	{
		std::swap(player_traps[0], player_traps[1]);
		player_traps.back().reset({player.get_spatiable_cx(), platform.get_spatiable_y()});
	}
}

/** In order to trigger traps, they both have to be at the same Y level and
form a box with a width smaller than N value.
The first enemy inside the box is affected.
*/

void world::trigger_player_traps()
{
	if(player_traps.front().get_spatiable_y()==player_traps.back().get_spatiable_y())
	{
		//Sort the traps, leftmost first.
		auto cmp=[](const app_interfaces::spatiable& a, const app_interfaces::spatiable& b) -> bool
		{
			return a.get_spatiable_x() < b.get_spatiable_x();
		};
		std::sort(std::begin(player_traps), std::end(player_traps), cmp);

		auto 	left=player_traps.front(),
			right=player_traps.back();
		float 	x=left.get_spatiable_x(),
			y=left.get_spatiable_y();
		int 	w=right.get_spatiable_ex()-x,
			h=left.get_spatiable_h();

		if(w <= player_trap::get_max_width())
		{
			app_interfaces::spatiable::t_box box{x, y, (unsigned int)w, (unsigned int)h};

			for(auto& i : enemies)
			{
				auto& e=*i;
				if(e.can_be_trapped() && !e.is_friendly() && e.is_colliding_with(box) )
				{
					e.get_trapped();
					break;
				}
			}
	
			for(auto &t : player_traps) t.set_delete(true);
		}
	}
}

void world::trigger_all_friendly_signal(const app_interfaces::spatiable::t_box& limit, player_effects& pe)
{
	for(auto& e : enemies) 
	{
		if(!e->is_friendly() && e->is_colliding_with(limit))
		{
			e->be_friendly(pe);
		}
	}
}
