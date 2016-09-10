#include "player.h"

#include <cassert>

using namespace app_game;

player::player()
	:motion_actor(0.f, 0.f, 20, 32), 
	previous_position(get_box()), 
	state(states::air), wakestate(states::air), remaining_jumps(0), 
	max_jumps(default_jump_quantity), score(0), score_multiplier(1), signals(0),
	can_cancel_jump(false), cancel_jump(false), next_special(player_effects::specials::triple_jump),
	facing(faces::right)
{
	reset();
}

//TODO: What if we just copy a new player over??? and set the position in the constructor?
void player::reset()
{
	set_position(20.f, 400.f);
	previous_position=get_box(); 
	state=states::air;
	wakestate=states::air;
	remaining_jumps=0; //This is so a trap won't spawn when the game starts.
	max_jumps=default_jump_quantity;
	score=0;
	score_multiplier=1;
	signals=0;
	can_cancel_jump=false;
	cancel_jump=false;
	specials.clear();
	specials.push_back(player_effects::specials::high_jump);
	specials.push_back(player_effects::specials::high_jump);
	specials.push_back(player_effects::specials::high_jump);

	timers_period[timers::always_trap]=0.f;
	timers_period[timers::stunned]=0.f;
	timers_period[timers::triple_jump]=0.f;
	timers_period[timers::extend_trap]=0.f;
	timers_period[timers::slow_down]=0.f;
	timers_period[timers::invulnerability]=0.f;
	timers_period[timers::score_multiplier]=0.f;

	shuffle_next_special();
}

void player::adjust_callback(float /*position*/, motion_actor::adjust_pos apos)
{
	switch(apos)
	{
		//This is falling against a platform: stop y and halve x speed.
		case motion_actor::adjust_pos::bottom:
			set_vector(0.f, axis::y);
			
			//TODO: If stunned, stop in your tracks

			if(state!=states::ground)
			{
				//This was annoying.
//				set_vector(get_vector_x()/2.f, axis::x);

				//TODO: This seems weird here, disjointed from the rest.
				if(state!=states::stunned) 
				{
					state=states::ground;
				}
				else
				{
					//Set the state for when we next wake.
					wakestate=states::ground;
					set_vector(get_vector_x()/2.f, axis::x);
				}

				remaining_jumps=max_jumps;
			}
		break;
		case motion_actor::adjust_pos::top:

		break;
		case motion_actor::adjust_pos::left:
			set_vector(0.f, axis::x);
		break;
		case motion_actor::adjust_pos::right:
			set_vector(0.f, axis::x);
		break;
		case motion_actor::adjust_pos::none:

		break;
	}
}

void player::transform_draw_struct(draw_control& dc)const
{
	
	int 	steps=1,
		curstep=0;

	bool	with_triple_jump=timers_period.at(timers::triple_jump),
		with_extended_trap=timers_period.at(timers::extend_trap),
		with_invulnerability=timers_period.at(timers::invulnerability),
		with_score_multiplier=timers_period.at(timers::score_multiplier),
		with_always_trap=timers_period.at(timers::always_trap);

	steps+=with_triple_jump+with_extended_trap+with_score_multiplier+with_always_trap;

	dc.set(steps);
	auto& b=dc[curstep++];

	b.set_type(draw_struct::types::box);

	int alpha=with_invulnerability ? 128 : 255;

	auto color=ldv::rgba8(0,0,160, alpha);

	switch(state)
	{
		case states::ground: break;
		case states::air: color=ldv::rgba8(0,160,0,alpha); break;
		case states::high_jump: color=ldv::rgba8(160,160,0,alpha); break;
		case states::stunned: color=ldv::rgba8(255,0,0,alpha); break;
	}

	b.set_color(color);
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});

	int 	y_indicators=get_spatiable_y(),
		x_indicators=get_spatiable_x()-5;
		if(x_indicators < 0) x_indicators=0;

	auto do_indicator=[&dc, &curstep, &y_indicators, x_indicators](ldv::rgba_color col)
	{
		y_indicators-=7;
		auto& indicator=dc[curstep++];
		indicator.set_color(col);
		indicator.set_type(draw_struct::types::polygon);
		indicator.set_polygon_fill(ldv::polygon_representation::type::fill);
		indicator.set_polygon_points({{0,0}, {5,-10}, {10,0}});
		indicator.go_to({x_indicators, y_indicators});
	};

	if(with_triple_jump) do_indicator(ldv::rgba8(0, 0, 255, 255));
	if(with_extended_trap) do_indicator(ldv::rgba8(245, 120, 176, 255));
	if(with_score_multiplier) do_indicator(ldv::rgba8(0, 0, 0, 255));
	if(with_always_trap) do_indicator(ldv::rgba8(0, 255, 0, 255));
}

void player::turn(float delta)
{
	const float ACELERACION_HORIZONTAL_SEGUNDO=200.f; //300.f;
	const float MAXIMA_VELOCIDAD_HORIZONTAL=150.f; //250.f;

	//Timers...
	auto do_timers=[this, delta](timers key, float& value)
	{
		if(value)
		{
			value-=delta;
			if(value <= 0.f)
			{
				value=0.f;
				switch(key)
				{
					case timers::stunned:
						state=wakestate;
						set_timer_period(timers::invulnerability, 1.5f); //Small invulnerability period added :).
					break;
					case timers::triple_jump: 
						this->max_jumps=2; 
						if(remaining_jumps > max_jumps) remaining_jumps=max_jumps;
					break;
					case timers::extend_trap: 
						this->signals|=s_reset_trap; 
					break;
					case timers::slow_down: 
						this->signals|=s_reset_slowdown; 
					break;
					case timers::score_multiplier: 
						score_multiplier=1;
					break;
					case timers::always_trap:
					case timers::invulnerability: 
					break;
				}
			}
		}
		
	};
	for(auto& sp : timers_period) do_timers(sp.first, sp.second);

	if(state==states::stunned) return;

	//Specials.
	if(specials.size())
	{
		if(p_input.y < 0) activate_special();
		else if(p_input.y > 0)
		{
			if(specials.size()==max_specials) trade_special();
			else if(specials.size() > 0) remove_special();
		}
	}

	//Horizontal movement...
	if(p_input.x)
	{
		float v=get_vector_x();
		//Controlar el "contravolante...".
		float extra=(p_input.x > 0 && v > 0.0f) 
			|| (p_input.x < 0 && v < 0.0f) 
			? 1.0f : 2.5f; 

		v+=(delta * ACELERACION_HORIZONTAL_SEGUNDO * extra) * p_input.x;
		if(v > MAXIMA_VELOCIDAD_HORIZONTAL) v=MAXIMA_VELOCIDAD_HORIZONTAL;
		else if(v < -MAXIMA_VELOCIDAD_HORIZONTAL) v=-MAXIMA_VELOCIDAD_HORIZONTAL;

		set_vector(v, axis::x);

		//Control facing
		facing=v > 0.f ? faces::right : faces::left;
	}
	else
	{
		float v=abs(get_vector_x());
		v-=(delta * ACELERACION_HORIZONTAL_SEGUNDO);
		if(v < 0.0) v=0;
			//Aproximarse al cero.
		float vr=get_vector_x();
		if(vr > 0.0) set_vector(v, axis::x);
		else if(vr < 0.0) set_vector(-v, axis::x);
	}

	switch(state)
	{
		case states::ground:
			if(p_input.jump && remaining_jumps) do_jump(jump_types::regular);
		break;
		case states::stunned:

		break;
		case states::air:
			if(p_input.jump && remaining_jumps) do_jump(jump_types::regular);

			//This takes form of a flag so the jump can only be cancelled once...
			cancel_jump=can_cancel_jump && (cancel_jump || !p_input.jump_press);

			if(cancel_jump && get_vector_y() < 0.f)
			{
				set_vector(get_vector_y() / 1.3f, axis::y);
			}
		break;
		case states::high_jump:
			//Revert state to air once peak has been reached.
			if(get_vector_y() >= 0.f) state=states::air;
		break;
	}

}

void player::set_falling()
{
	state=states::air;
	remaining_jumps=0;
}

void player::collide_with_harm_actor(const motion_actor& e)
{
	state=states::stunned;
	set_timer_period(timers::stunned, 1.f);

	//By default we'll wake on air, as this will send us flying.
	//In case the player touches ground, wakestate will change.
	wakestate=states::air;

	//Player will be pushed according to its own speed, enemy speed or a preset value.
	struct push
	{
		float val;
		bool operator<(const push& o) const {return abs(val) < abs(o.val);}
	};

	auto push_val=std::max({push{-get_vector_x()}, push{-e.get_vector_x()}, push{-100.f}});
	set_vector({push_val.val, -100.f});
}

void player::recieve_effects(player_effects pe)
{
	if(pe.get_effects().size()) for(const auto& e : pe.get_effects()) add_special(e);
	score+=pe.get_score()*score_multiplier;
}

void player::add_special(player_effects::specials sp)
{
	specials.insert(std::begin(specials), sp);
	while(specials.size() > max_specials) specials.pop_back();
}

void player::remove_special()
{
	assert(specials.size() > 0);
	specials.erase(std::begin(specials));
}

void player::activate_special()
{
	assert(specials.size() > 0);

	switch(specials.front())
	{
		case player_effects::specials::triple_jump:
			max_jumps=extended_jump_quantity;
			if(remaining_jumps==default_jump_quantity) remaining_jumps=extended_jump_quantity;
			set_timer_period(timers::triple_jump, 10.f);
		break;
		case player_effects::specials::all_friendly:
			signals|=s_all_friendly;
		break;
		case player_effects::specials::extend_trap:
			signals|=s_extend_trap;
			set_timer_period(timers::extend_trap, 10.f);
		break;
		case player_effects::specials::slow_down:
			signals|=s_slowdown;
			set_timer_period(timers::slow_down, 5.f);
		break;
		case player_effects::specials::invulnerability:
			set_timer_period(timers::invulnerability, 5.f);
		break;
		case player_effects::specials::high_jump:
			do_jump(jump_types::high_jump);
		break;
		case player_effects::specials::score_multiplier:
			//TODO: Maybe stack more of them????
			score_multiplier=2;
			set_timer_period(timers::score_multiplier, 5.f);
		break;
		case player_effects::specials::projectile:
			signals|=s_projectile;
		break;
		case player_effects::specials::always_trap:
			set_timer_period(timers::always_trap, 5.f);
		break;
	}
	remove_special();
}

void player::set_timer_period(timers index, float ptime)
{
	assert(timers_period.count(index));
	timers_period[index]=ptime;
}

void player::shuffle_next_special()
{
	std::vector<player_effects::specials> sp{player_effects::specials::all_friendly, player_effects::specials::slow_down, player_effects::specials::invulnerability};
	std::random_shuffle(std::begin(sp), std::end(sp));
	next_special=sp.front();
}

void player::trade_special()
{
	specials.clear();
	specials.push_back(next_special);
	shuffle_next_special();
}

void player::land_on_platform(const platform& pl)
{	
	//TODO: Some constant please.
	if(pl.is_bouncy() && get_vector_y() > 80.f)
	{
		do_jump(jump_types::bounce_platform);
	}
	else
	{
		adjust(pl, app_game::motion_actor::adjust_pos::bottom);
	}
}

void player::bounce_on_enemy()
{
	do_jump(jump_types::bounce_enemy);
}

void player::do_jump(jump_types jt)
{
	//Base values, may be modified later.
	state=jt==jump_types::high_jump ? states::high_jump : states::air;
	cancel_jump=false;
	float vel=0.f;

	switch(jt)
	{
		case jump_types::regular:
			can_cancel_jump=true;
			vel=-300.f;
			--remaining_jumps;
		break;

		case jump_types::high_jump:
			can_cancel_jump=false;
			vel=-500.f;
			remaining_jumps=max_jumps;
		break;

		case jump_types::bounce_platform: //Takes into account downward speed.
			can_cancel_jump=false;

			vel=p_input.jump_press ? 
					(-get_vector_y() / 4.f)*5.f : //Fifth fourths of the speed.
					(-get_vector_y() / 4.f)*3.f; //Three fourths of the speed.

			if(vel)
			{
				if(vel < -450.f) vel=-450.f;
				remaining_jumps=0;
			}
		break;

		case jump_types::bounce_enemy:
			can_cancel_jump=true;
			remaining_jumps=1;
			vel=p_input.jump_press ? -300.f : -200.f;
		break;
	}

	if(vel) set_vector(vel, axis::y);
}
