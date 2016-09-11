#ifndef ENEMY_FLYING_CHASER_H
#define ENEMY_FLYING_CHASER_H

#include "enemy.h"
#include "platform.h"
#include "projectile_def.h"

namespace app_game
{

class enemy_flying_chaser:
	public enemy
{
	public:

				enemy_flying_chaser(const app_interfaces::spatiable&, const app_interfaces::spatiable&, const std::vector<std::unique_ptr<platform>>&);

	//////////////////////
	//Enemy

	virtual void		do_turn(float);
	virtual void		collide_with_player();
	virtual void		get_jumped_on();
	virtual void		get_trapped();
	virtual void		get_hit_by_projectile();
	virtual void		be_friendly(player_effects&);
	virtual bool		can_be_trapped() const {return is_stunned();}
	virtual bool		can_be_jumped_on() const {return !is_stunned() && !is_friendly();}
	virtual bool		can_be_hit_by_projectile() const {return !is_friendly();}

	//////////////////////
	//Drawable.

	virtual int		get_draw_order() const {return 25;}
	virtual void 		transform_draw_struct(draw_control&) const;

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return is_active() ? 0.f : 600.f;}
	virtual float 	get_max_fall_speed() const {return 600.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos);

	private:

	void		fall(float);
	void		correct_vector();

	enum class				states{chasing, drifting} state;
	const app_interfaces::spatiable&	player_target;
	const std::vector<std::unique_ptr<platform>>&	platforms;
	float					period;

	static const int	fixed_w=30,
				fixed_h=30,
				speed=60;
};

}

#endif
