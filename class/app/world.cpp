#include "world.h"

#include <cassert>

#include "platform_regular.h"
#include "platform_dissapearing.h"
#include "platform_crumbling.h"
#include "platform_bouncy.h"

#include "bonus_score.h"
#include "bonus_triple_jump.h"
#include "bonus_extend_trap.h"
#include "bonus_high_jump.h"
#include "bonus_projectile.h"
#include "bonus_score_multiplier.h"
#include "bonus_always_trap.h"

#include "projectile_parabol.h"
#include "enemy_patrolling.h"
#include "enemy_patrolling_pause.h"
#include "enemy_flying_parabol.h"
#include "enemy_parabol_shooter.h"
#include "enemy_flying.h"

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
	//TODO: A sexier template would get a vector of pointers to these.
//	check_bounds_helper(enemies);
//	check_bounds_helper(pickups);
//	check_bounds_helper(projectiles);
//	check_bounds_helper(player_projectiles);
//	check_bounds_helper(platforms);
	
	if(player_traps.size()==max_player_traps)
	{
		trigger_player_traps();
	}

//TODO: There's another below, let's see what we need
//	delete_discarded_objects();

	for(auto& t: player_traps) t.do_turn(delta);
	for(auto& p: platforms) p->do_turn(delta);
	for(auto& e: enemies) e->do_turn(delta);
	for(auto& p: projectiles) p->do_turn(delta);
	for(auto& p: player_projectiles) 
	{
		p->do_turn(delta);
		for(auto& e : enemies)
		{
			if(e->can_be_hit_by_projectile() && p->is_colliding_with(*e))
			{
				e->get_hit_by_projectile();
				p->collide_with_target();
			}
		}
	}

	//If someone shot, turn the definitions into real projectiles.
	if(projectile_definitions.size()) create_projectiles();

	camera_movement=0;

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
			camera_movement=+mov;
		}

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

	//Base platform of the world.
	platforms.push_back(std::unique_ptr<platform>{new platform_regular(0.f,y,app::definitions::playground_width)});
//	platforms.push_back(std::unique_ptr<platform>{new platform_bouncy(0.f,y,app::definitions::playground_width)});
	generate_new_world_threshold();

	do
	{
		generate_new_world();
	}while(world_threshold >= -480.f);
}

void world::generate_new_world_threshold()
{
	tools::int_generator next_diff_gen(1 , 3);
	world_threshold=platforms.back()->get_spatiable_y()-(next_diff_gen()*app::definitions::unit);
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
	player_projectiles.clear();
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
	const float bottom_limit=-distance+app::definitions::playground_height+extra;
	return false;
//	std::cout<<s.get_spatiable_y()<<" >= "<<bottom_limit<<"     ("<<-distance<<"+"<<app::definitions::playground_height<<"+"<<extra<<")"<<std::endl;
//	return s.get_spatiable_y() >= bottom_limit;
}

void world::delete_discarded_objects()
{
	//TODO: A sexier template would get a vector of pointers to vectors of this stuff.
	delete_helper_ptr(platforms);
	delete_helper_ptr(pickups);
	delete_helper_ptr(projectiles);
	delete_helper_ptr(player_projectiles);
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
		auto&	last_platform=*(platforms.back());
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
	
	//TODO: The types should appear as the game goes on.
	//TODO: Don't add two dissapearing in a row... Or better, after
	//a dissapearing or crumbling, add a regular.
	enum class types{regular, dissapearing, crumbling, bouncy};
	std::vector<types> t{types::regular, types::dissapearing, types::crumbling, types::bouncy};
	std::unique_ptr<platform> p{nullptr};
	tools::int_generator gen(0, t.size()-1);

//	switch(t[gen()])
	switch(types::regular)
	{
		case types::regular:		p.reset(new platform_regular{x_pos,y,(int)w_pos}); break;
		case types::dissapearing:	p.reset(new platform_dissapearing{x_pos,y,(int)w_pos}); break;
		case types::crumbling:		p.reset(new platform_crumbling{x_pos,y,(int)w_pos}); break;
		case types::bouncy:		p.reset(new platform_bouncy{x_pos,y,(int)w_pos}); break;
	}
	assert(p.get());
	platforms.push_back(std::move(p));
	assert(platforms.back()->get_spatiable_ex() <= app::definitions::playground_width);
}

//TODO: Filter below the line.
std::vector<app_interfaces::drawable const *> world::get_drawables() const
{
	std::vector<app_interfaces::drawable const *> res;
	for(const auto &p : platforms) res.push_back(p.get());
	for(const auto &p : pickups) res.push_back(p.get());
	for(const auto &p : enemies) res.push_back(p.get());
	for(const auto &p : projectiles) res.push_back(p.get());
	for(const auto &p : player_projectiles) res.push_back(p.get());
	for(const auto &p : player_traps) res.push_back(&p);

	return res;
}

std::vector<platform *> world::get_platforms()
{
	std::vector<platform *> res;
	for(auto& p : platforms) 
		if(p->is_collidable() && p->get_spatiable_ey() >= -distance)
			res.push_back(p.get());
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
		if(p->get_spatiable_ey() >= -distance)
			res.push_back(p.get());
	return res;
}

std::vector<projectile *> world::get_player_projectiles()
{
	std::vector<projectile *> res;
	for(auto& p : player_projectiles)
		if(p->get_spatiable_ey() >= -distance)
			res.push_back(p.get());
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

	//TODO: Do we want to enforce that condition????
- No bonus can exist on the world's lowest platform. 
- The wider the last platform, the more possibilities a bonus appears.
- Bonus change grows as more platforms are created and resets when a bonus is created.
*/

void world::evaluate_new_bonus()
{
	bonus_chance_calculator.increment(app::world_to_grid(platforms.back()->get_spatiable_w()));
	if(bonus_chance_calculator.evaluate() && platforms.back()->can_spawn_bonus())
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
	enum class types{score, triple_jump, extend_trap, high_jump, score_multiplier, projectile, always_trap};
	std::vector<types> t{types::score, types::triple_jump, types::extend_trap, types::high_jump, types::score_multiplier, types::projectile, types::always_trap};

	if(!t.size()) return;

	std::unique_ptr<pickup> b{nullptr};
	tools::int_generator gen(0, t.size()-1);

	switch(t[gen()])
//	switch(types::score)
	{
		case types::score:		b.reset(new bonus_score()); break;
		case types::triple_jump:	b.reset(new bonus_triple_jump()); break;
		case types::extend_trap:	b.reset(new bonus_extend_trap()); break;
		case types::high_jump:		b.reset(new bonus_high_jump()); break;
		case types::score_multiplier:	b.reset(new bonus_score_multiplier()); break;
		case types::projectile:		b.reset(new bonus_projectile()); break;
		case types::always_trap:	b.reset(new bonus_always_trap()); break;
	}	

	auto&	last_platform=*(platforms.back());
	float 	left=last_platform.get_spatiable_x(),
		right=last_platform.get_spatiable_ex()-b->get_spatiable_w();

	tools::int_generator x_generator(left, right);
	float x=x_generator();
	float y=last_platform.get_spatiable_y()-b->get_spatiable_h()-app::definitions::bonus_units_above_ground;

	assert(b.get());
	b->set_position(x, y);
	pickups.push_back(std::move(b));
}

/** By default the new patrolling enemy is placed on 0,0 and later is placed at the center
top of the last platform, facing the direction set on its constructor.
*/

bool world::create_new_enemy()
{
	const auto&	last_platform=*(platforms.back());

	enum class types {patrolling, patrolling_pause, parabol, flying, parabol_shooter};
	
	std::vector<types> t;
	//TODO: Rework this values.
	if(last_platform.can_spawn_ground_based_enemies() && distance > 20.f) t.push_back(types::patrolling);
	if(last_platform.can_spawn_ground_based_enemies() && distance > 20.f) t.push_back(types::patrolling_pause);
	if(distance > 100.f) t.push_back(types::flying);
	if(last_platform.can_spawn_ground_based_enemies() && distance > 300.f) t.push_back(types::parabol_shooter);
	if(distance > 300.f) t.push_back(types::parabol);

	tools::int_generator gen(0, t.size()-1);
	std::unique_ptr<enemy> e{nullptr};

	float 	left=last_platform.get_spatiable_x(),
		right=last_platform.get_spatiable_ex();

	if(!t.size()) return false;
	switch(t[gen()])
//	switch(types::patrolling_pause)
	{
		case types::patrolling:
			e.reset(new enemy_patrolling{{left, right}, last_platform.get_spatiable_y()});
		break;
		case types::patrolling_pause:
			e.reset(new enemy_patrolling_pause{{left, right}, last_platform.get_spatiable_y()});
		break;
		case types::parabol:
			e.reset(new enemy_flying_parabol(player_position.get_spatiable_position(), app::definitions::playground_height-distance));
		break;
		case types::flying:
			e.reset(new enemy_flying{{0.f, app::definitions::playground_width}, last_platform.get_spatiable_y()});
		break;
		case types::parabol_shooter:
			e.reset(new enemy_parabol_shooter(projectile_definitions, player_position, last_platform));
		break;
	}

	assert(e.get());
	enemies.push_back(std::move(e));
	return true; 
}

/** Evaluates if an enemy is to be created on top of last platform according to
* these rules...
* - No enemies should appear before a certain height is reached. The 
	"create_new_enemy" function shall enforce that.
* - As with bonus generators, there's a chance that increases as platforms are
* created.
*/

void world::evaluate_new_enemy()
{
	auto& last_platform=*(platforms.back());
	enemy_chance_calculator.increment(app::world_to_grid(last_platform.get_spatiable_w()));
	if(enemy_chance_calculator.evaluate())
	{
		if(create_new_enemy()) enemy_chance_calculator.reset();
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
			auto container=pd.side==projectile_def::sides::enemy ? &projectiles : &player_projectiles;
			switch(pd.type)
			{
				case projectile_def::types::parabol:
					container->push_back(std::unique_ptr<projectile>(new projectile_parabol(pd.origin, pd.direction)));
				break;
			}
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

/** All enemies inside the "limit" box (basically the whole screen) are made
friendly. There's little more to it.
*/

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

void world::add_player_projectile(const motion_actor& pl, actor::faces f)
{
	projectile_definitions.push_back(
		{{pl.get_spatiable_cx(), pl.get_spatiable_y()}, 
		{f==actor::faces::left ? -300.f : 300.f, -50.f}, 
		projectile_def::types::parabol, 
		projectile_def::sides::player});
}

/** Gets how far is the y position from the edge of the camera.
*/

float world::get_relative_y(float y) const
{
	if(y > 0.f) return abs(y-distance);
	else return distance-abs(y);
}

void world::adjust_high_jump_distance(int y)
{
	//TODO: There's something rotten about this????... Once we trigger this everything fucks up.

	int d=get_relative_y(y);
	if(d < 250)
	{
		int mv=250-d;
		std::cout<<"D WAS "<<d<<" WILL MOVE "<<mv<<" ORIGINAL "<<distance<<" NEW: "<<distance+mv<<std::endl;
		distance+=mv;
		camera_movement+=mv;
	}
}
