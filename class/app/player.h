#ifndef PLAYER_H
#define PLAYER_H

#include "motion_actor.h"
#include "drawable.h"
#include "platform.h"
#include "player_input.h"
#include "player_effects.h"

namespace app_game
{

class player:
	public motion_actor,
	public app_interfaces::drawable
{
	public:

	enum				signals{s_all_friendly=1, s_extend_trap=2, s_reset_trap=4, s_slowdown=8, s_reset_slowdown=16, s_projectile=32};

					player();
	faces				get_facing() const {return facing;}
	void				get_input(player_input pi) {p_input=pi;}
	void				turn(float delta);
	void				set_falling();
	bool				can_fall() const {return state==states::ground;}
	bool				is_high_jumping() const {return state==states::high_jump;}
	const std::vector<player_effects::specials>& get_specials() const {return specials;}
	void				recieve_effects(player_effects);
	int				get_score() const {return score;}
	int				get_signals() const {return signals;}
	void				reset_signals() {signals=0;}
	player_effects::specials	get_next_special() const {return next_special;}
	void				land_on_platform(const platform&);

	bool				can_land_on_enemies() const {return state==states::air && get_vector().y > 0.f;}
	//Player can't set traps after double jump or when falling from an edge.
	bool				can_set_trap() const {return state==states::air && (timers_period.at(timers::always_trap) || remaining_jumps);}
	bool				is_vulnerable() const {return !timers_period.at(timers::invulnerability) && !(state==states::stunned || state==states::high_jump);}
	void 				collide_with_harm_actor(const motion_actor&);
	void				reset();
	void				bounce_on_enemy();

	////////////////////
	//Motion actor

	virtual float 	get_weight() const {return 600.f;}
	virtual float 	get_max_fall_speed() const {return 600.f;}
	virtual void 	adjust_callback(float, motion_actor::adjust_pos);

	////////////////////
	//Drawable

	virtual int 			get_draw_order() const {return 50;}
	virtual void 			transform_draw_struct(draw_control&) const;

	private:

	enum class 			states{ground, air, high_jump, stunned};
	enum class			jump_types{regular, high_jump, bounce_platform, bounce_enemy};
	enum class			timers{stunned, triple_jump, extend_trap, slow_down, invulnerability, score_multiplier, always_trap};

	void				shuffle_next_special();
	void				add_special(player_effects::specials);
	void				remove_special();
	void				activate_special();
	void				trade_special();
	void				set_timer_period(timers, float);
	void				do_jump(jump_types);

	states				state, wakestate;
	player_input			p_input;
	int				remaining_jumps, max_jumps, score, score_multiplier, signals;
	bool				can_cancel_jump, cancel_jump;
	player_effects::specials	next_special;
	faces				facing;
	std::vector<player_effects::specials>	specials;
	std::map<timers, float> 	timers_period;
	
	//If the vector x is less than this and the player is hit, a larger vector is recieved.
	static const int	min_vector_hit_guard=30,
				max_specials=3,
				extended_jump_quantity=3,
				default_jump_quantity=2;
	
		

};

}

#endif
