#ifndef WORLD_H
#define WORLD_H

#include "platform.h"

namespace app_game
{

class world
{
	public:

			world();

	std::vector<app_interfaces::spatiable const *> get_collidables() const;
	std::vector<app_interfaces::drawable const *> get_drawables() const;

	bool		is_moving() const {return moving;}
	void		set_moving(bool v) {moving=v;}	
	int		get_camera_movement() const {return camera_movement;}
	bool		is_outside_bounds(const app_interfaces::spatiable&, float=0.f) const;
	void		delete_discarded_objects();
	void		generate_new_world();

	void		do_turn(float delta);
	void 		init();
	void 		reset(); 

	private:

	void				create_new_platform(float y);

	std::vector<app_game::platform>	platforms;
	bool				moving;
	float				distance, partial;
	int				camera_movement, next_platform_diff;
};

}

#endif
