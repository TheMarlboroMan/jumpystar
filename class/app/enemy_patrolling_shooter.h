#ifndef ENEMY_PATROLLING_SHOOTER_H
#define ENEMY_PATROLLING_SHOOTER_H

#include "enemy.h"
#include "projectile_def.h"

namespace app_game
{

class enemy_patrolling_shooter:
	public enemy
{
	public:

				enemy_patrolling_shooter(const enemy_sideways_limit&, float, std::vector<projectile_def>&);

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

	//TODO: Are these really part of the motion actor????. Should we try again?.

	virtual float 		get_weight() const {return 0.f;}
	virtual float 		get_max_fall_speed() const {return 0.f;}
	virtual void 		adjust_callback(float, motion_actor::adjust_pos) {}

	private:

	std::vector<projectile_def>&		projectiles;

	enemy_sideways_limit	limits;
	enum class states	{move, shoot, pause} state;
	float			period, max_period;

	static const int	fixed_w=20,
				fixed_h=30;
};

}

#endif
