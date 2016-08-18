#ifndef WORLD_H
#define WORLD_H

#include "platform.h"
#include "bonus.h"

#include <class/number_generator.h>

#include "definitions.h"

namespace app_game
{

class world
{
	public:

			world();

	std::vector<app_game::bonus *> 			get_pickables();
	std::vector<app_interfaces::spatiable const *> 	get_collidables() const;
	std::vector<app_interfaces::drawable const *> 	get_drawables() const;

	int		get_camera_movement() const {return camera_movement;}
	bool		is_moving() const {return moving;}
	bool		is_outside_bounds(const app_interfaces::spatiable&, float=0.f) const;
	bool		is_create_new() const;

	void		set_moving(bool v) {moving=v;}

	void		generate_new_world();
	void		delete_discarded_objects();

	void		do_turn(float delta);
	void 		init();
	void 		reset(); 

	private:

	void				create_new_platform(float y);
	void				create_new_bonus();
	void				generate_new_world_threshold();
	void				evaluate_new_bonus();

	std::vector<app_game::platform>	platforms;
	std::vector<app_game::bonus>	bonus;

	bool				moving;
	float				distance, partial, speed;
	int				camera_movement, world_threshold;

	struct s_bonus_data{
		int			chance;
		tools::int_generator 	generator;
		void			reset_chance() {chance=app::definitions::base_bonus_chance;}

		s_bonus_data()
			:chance{app::definitions::base_bonus_chance},
			generator{app::definitions::min_bonus_percentage, app::definitions::max_bonus_percentage}
		{}
	}				bonus_data;
};

template<typename T>
void delete_helper(std::vector<T>& container)
{
	auto it=std::remove_if(std::begin(container), std::end(container), [](const T& p) {return p.is_delete();});
	container.erase(it, std::end(container));
}

}

#endif
