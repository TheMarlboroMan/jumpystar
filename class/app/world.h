#ifndef WORLD_H
#define WORLD_H

#include "platform.h"

namespace App_Game
{

class World
{
	public:

			World();

	const		std::vector<App_Game::Platform>& get_platforms() const {return platforms;}
	bool		is_moving() const {return moving;}
	void		set_moving(bool v) {moving=v;}	
	int		get_camera_movement() const {return camera_movement;}
	bool		is_outside_bounds(const App_Interfaces::Spatiable&, float=0.f) const;
	void		delete_discarded_objects();
	void		generate_new_world();

	void		do_turn(float delta);
	void 		init();
	void 		reset(); 

	private:

	void				create_new_platform(float y);

	std::vector<App_Game::Platform>	platforms;
	bool				moving;
	float				distance, partial;
	int				camera_movement, next_platform_diff;
};

}

#endif
