#ifndef WORLD_H
#define WORLD_H

//std...

//tools
#include <class/number_generator.h>

//locals
#include "platform.h"
#include "pickup.h"
#include "projectile.h"
#include "enemy.h"
#include "player_trap.h"
#include "definitions.h"

namespace app_game
{

class world
{
	public:

			world(const app_interfaces::spatiable&);

	std::vector<std::unique_ptr<enemy>>& 		get_enemies() {return enemies;}
	std::vector<projectile>& 			get_projectiles() {return projectiles;}
	std::vector<std::unique_ptr<pickup>>&		get_pickups() {return pickups;}
	std::vector<app_interfaces::spatiable const *> 	get_collidables() const;
	std::vector<app_interfaces::drawable const *> 	get_drawables() const;

	int		get_camera_movement() const {return camera_movement;}
	int		get_distance() const {return distance;} //TODO: Remove if unused.
	bool		is_moving() const {return moving;}
	bool		is_outside_bounds(const app_interfaces::spatiable&, float=0.f) const;

	void		set_moving(bool v) {moving=v;}
	void		set_player_trap(const app_interfaces::spatiable&, const app_interfaces::spatiable&);
	void 		trigger_all_friendly_signal(const app_interfaces::spatiable::t_box&, player_effects&);

	void		do_turn(float delta);
	void 		init();
	void 		reset(); 

	void				create_new_enemy();

	private:

	bool				is_create_new() const;
	void				create_new_platform(float y);
	void				create_new_bonus();
	void				create_projectiles();

	void				generate_new_world();
	void				generate_new_world_threshold();
	void				evaluate_new_bonus();
	void				evaluate_new_enemy();
	void				delete_discarded_objects();
	void				trigger_player_traps();

	
	static const size_t		max_player_traps=2;

	std::vector<std::unique_ptr<enemy>>	enemies;
	std::vector<platform>			platforms;
	std::vector<std::unique_ptr<pickup>>	pickups;
	std::vector<projectile_def>		projectile_definitions;
	std::vector<projectile>			projectiles;
	std::vector<player_trap>		player_traps;

	const app_interfaces::spatiable&		player_position;

	bool				moving;
	float				distance, partial, speed;
	int				camera_movement, world_threshold;

	

	class s_chance_data{
		public:

		s_chance_data(int pc, int pinit, int pend)
			:chance{pc}, initial(pc), generator{pinit, pend}
		{};

		bool			evaluate() {return chance <= generator();}
		void			reset() {chance=initial;}
		void			increment(int v) {chance+=v;}

		private:

		int			chance,
					initial;
		tools::int_generator	generator;
	}				bonus_chance_calculator, enemy_chance_calculator;
};

template<typename T>
void delete_helper(std::vector<T>& container)
{
	auto it=std::remove_if(std::begin(container), std::end(container), [](const T& p) {return p.is_delete();});
	container.erase(it, std::end(container));
}

template<typename T>
void delete_helper_ptr(std::vector<T>& container)
{
	auto it=std::remove_if(std::begin(container), std::end(container), [](const T& p) {return p->is_delete();});
	container.erase(it, std::end(container));
}

}

#endif
