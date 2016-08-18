#ifndef WORLD_H
#define WORLD_H

#include "platform.h"
#include "bonus.h"
#include "patrolling_enemy.h"

#include <class/number_generator.h>

#include "definitions.h"

namespace app_game
{

class world
{
	public:

			world();

	std::vector<app_game::patrolling_enemy *> 	get_enemies();
	std::vector<app_game::bonus *> 			get_pickables();
	std::vector<app_interfaces::spatiable const *> 	get_collidables() const;
	std::vector<app_interfaces::drawable const *> 	get_drawables() const;

	int		get_camera_movement() const {return camera_movement;}
	bool		is_moving() const {return moving;}
	bool		is_outside_bounds(const app_interfaces::spatiable&, float=0.f) const;

	void		set_moving(bool v) {moving=v;}

	void		do_turn(float delta);
	void 		init();
	void 		reset(); 

	private:

	bool				is_create_new() const;
	void				create_new_platform(float y);
	void				create_new_bonus();
	void				create_new_enemy();
	void				generate_new_world();
	void				generate_new_world_threshold();
	void				evaluate_new_bonus();
	void				evaluate_new_enemy();
	void				delete_discarded_objects();


	std::vector<app_game::patrolling_enemy>	enemies;
	std::vector<app_game::platform>	platforms;
	std::vector<app_game::bonus>	bonus;

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

}

#endif
