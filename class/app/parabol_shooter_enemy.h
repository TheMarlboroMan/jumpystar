#ifndef PARABOL_SHOOTER_ENEMY_H
#define PARABOL_SHOOTER_ENEMY_H

#include "enemy.h"

namespace app_game
{

class parabol_shooter_enemy:
	public enemy
{
	public:

				parabol_shooter_enemy(std::vector<projectile_def>&, const app_interfaces::spatiable&, const app_interfaces::spatiable&);

	//////////////////////
	//Enemy

	void			do_turn(float);
	void			collide_with_player();
	void			get_jumped_on();
	void			get_trapped();
	bool			can_be_trapped() const {return true;}
	bool			can_be_jumped_on() const {return true;}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order()const {return 25;}
	virtual int		get_draw_cycle()const {return 1;}
	virtual void 		transform_draw_struct(app_game::draw_struct &b)const;
	virtual bool		is_draw_delete()const {return false;}

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return 0.f;}
	virtual float 	get_max_fall_speed() const {return 0.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos) {}

	private:

	std::vector<projectile_def>&		projectiles;
	const app_interfaces::spatiable&	player_target;
	float					period;

	static const int	fixed_w=20,
				fixed_h=30,
				min_time=1500,
				max_time=3000,
				max_shooting_distance=300;
};

}

#endif