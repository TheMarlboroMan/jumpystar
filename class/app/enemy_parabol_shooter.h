#ifndef ENEMY_PARABOL_SHOOTER_H
#define ENEMY_PARABOL_SHOOTER_H

#include "enemy.h"
#include "projectile_def.h"

namespace app_game
{

class enemy_parabol_shooter:
	public enemy
{
	public:

				enemy_parabol_shooter(std::vector<projectile_def>&, const app_interfaces::spatiable&, const app_interfaces::spatiable&);

	//////////////////////
	//Enemy

	virtual void		do_turn(float);
	virtual void		collide_with_player();
	virtual void		get_jumped_on();
	virtual void		get_trapped();
	virtual void		get_hit_by_projectile();
	virtual void		be_friendly(player_effects&);
	virtual bool		can_be_trapped() const {return true;}
	virtual bool		can_be_jumped_on() const {return !is_stunned() && !is_friendly();}
	virtual bool		can_be_hit_by_projectile() const {return !is_friendly();}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 25;}
	virtual void 		transform_draw_struct(draw_control&) const;

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
				max_time=3000;
};

}

#endif
