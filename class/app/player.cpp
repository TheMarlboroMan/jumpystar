#include "player.h"

#include <cassert>

using namespace app_game;

player::player()
	:motion_actor(0.f, 0.f, 20, 32), 
	previous_position(get_box()), 
	state(states::air), wakestate(states::air), remaining_jumps(0), 
	max_jumps(default_jump_quantity), score(0), score_multiplier(1), signals(0),
	cancel_jump(false)
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
	cancel_jump=false;
	specials.clear();

	specials_period[player_effects::specials::triple_jump]=0.f;
	specials_period[player_effects::specials::extend_trap]=0.f;
	specials_period[player_effects::specials::slow_down]=0.f;
	specials_period[player_effects::specials::invulnerability]=0.f;
	specials_period[player_effects::specials::score_multiplier]=0.f;
}

void player::adjust_callback(float /*position*/, motion_actor::adjust_pos apos)
{
	switch(apos)
	{
		//This is falling against a platform: stop y and halve x speed.
		case motion_actor::adjust_pos::bottom:
			set_vector(0.f, axis::y);
			
			if(state!=states::ground)
			{
				set_vector(get_vector_x()/2.f, axis::x);

				//TODO: This seems weird here, disjointed from the rest.
				if(state!=states::stunned) 
				{
					state=states::ground;
				}
				else
				{
					//Set the state for when we next wake.
					wakestate=states::ground;
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

	bool	with_triple_jump=specials_period.at(player_effects::specials::triple_jump),
		with_extended_trap=specials_period.at(player_effects::specials::extend_trap),
		with_invulnerability=specials_period.at(player_effects::specials::invulnerability),
		with_score_multiplier=specials_period.at(player_effects::specials::score_multiplier);

	steps+=with_triple_jump+with_extended_trap+with_score_multiplier;

	dc.set(steps);
	auto& b=dc[curstep++];

	b.set_type(draw_struct::types::box);

	int alpha=with_invulnerability ? 128 : 255;

	auto color=ldv::rgba8(0,0,160, alpha);

	switch(state)
	{
		case states::ground: break;
		case states::air: color=ldv::rgba8(0,160,0,alpha); break;
		case states::stunned: color=ldv::rgba8(255,0,0,alpha); break;
	}

	b.set_color(color);
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});

	int 	y_indicators=get_spatiable_y(),
		x_indicators=get_spatiable_x()-5;
		if(x_indicators < 0) x_indicators=0;

	if(with_triple_jump)
	{
		y_indicators-=7;
		auto& jump_indicator=dc[curstep++];
		jump_indicator.set_color(ldv::rgba8(0, 0, 255, 255));
		jump_indicator.set_type(draw_struct::types::polygon);
		jump_indicator.set_polygon_fill(ldv::polygon_representation::type::fill);
		jump_indicator.set_polygon_points({{0,0}, {5,-10}, {10,0}});
		jump_indicator.go_to({x_indicators, y_indicators});
	}

	if(with_extended_trap)
	{
		y_indicators-=7;
		auto& extend_trap_indicator=dc[curstep++];
		extend_trap_indicator.set_color(ldv::rgba8(245, 120, 176, 255));
		extend_trap_indicator.set_type(draw_struct::types::polygon);
		extend_trap_indicator.set_polygon_fill(ldv::polygon_representation::type::fill);
		extend_trap_indicator.set_polygon_points({{0,0}, {5,-10}, {10,0}});
		extend_trap_indicator.go_to({x_indicators, y_indicators});
	}

	if(with_score_multiplier)
	{
		y_indicators-=7;
		auto& score_multiplier_indicator=dc[curstep++];
		score_multiplier_indicator.set_color(ldv::rgba8(0, 0, 0, 255));
		score_multiplier_indicator.set_type(draw_struct::types::polygon);
		score_multiplier_indicator.set_polygon_fill(ldv::polygon_representation::type::fill);
		score_multiplier_indicator.set_polygon_points({{0,0}, {5,-10}, {10,0}});
		score_multiplier_indicator.go_to({x_indicators, y_indicators});
	}
}

void player::turn(float delta)
{
	const float ACELERACION_HORIZONTAL_SEGUNDO=200.f; //300.f;
	const float MAXIMA_VELOCIDAD_HORIZONTAL=150.f; //250.f;

	if(state==states::stunned)
	{
		stunned_time+=delta;
		//TODO: Gimme my constants.
		if(stunned_time >= 1.f)
		{
			stunned_time=0.f;
			state=wakestate;
		}
	}
	else
	{
		//Timers...
		auto do_specials_timer=[this, delta](player_effects::specials key, float& value)
		{
			if(value)
			{
				value-=delta;
				if(value <= 0.f)
				{
					value=0.f;
					switch(key)
					{
						case player_effects::specials::triple_jump: 
							this->max_jumps=2; 
							if(remaining_jumps > max_jumps) remaining_jumps=max_jumps;
						break;
						case player_effects::specials::extend_trap: 
							this->signals|=s_reset_trap; 
						break;
						case player_effects::specials::slow_down: 
							this->signals|=s_reset_slowdown; 
						break;
						case player_effects::specials::score_multiplier: 
							score_multiplier=1;
						break;
			
						//These are cases that won't apply but I prefer to list them
						//instead of defaulting (in case new effects are added this
						//will trigger a warning).
						case player_effects::specials::invulnerability: break;
						case player_effects::specials::all_friendly: break;
						case player_effects::specials::high_jump: break;
					}
				}
			}
			
		};
		for(auto& sp : specials_period) do_specials_timer(sp.first, sp.second);

		//Specials.
		if(specials.size())
		{
			if(p_input.y < 0) activate_special();
			else if(p_input.y > 0) remove_special();
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

		//Jumps...
		if(p_input.jump && remaining_jumps)
		{
			//TODO: Current speed downwards should count.

			//TODO: This could be another method invoked by high jump too.
			set_vector(-300.f, axis::y);
			state=states::air;
			--remaining_jumps;
			cancel_jump=false;
		}
	}

	switch(state)
	{
		case states::ground:

		break;
		case states::stunned:

		break;
		case states::air:
		{
			//TODO: This should not work if activating high jump.
			//Allow for shortening the jump if the button is not pressed.
			float vy=get_vector_y();

			//This takes form of a flag so the jump can only be cancelled once...
			//This affects the weight of the player.
			//TODO: I don't like the effect it has...
			//TODO: Try it some other way please, I don't like this!!!!.
			cancel_jump=cancel_jump || (!p_input.jump_press && vy < 0.f);
		}
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

	//By default we'll wake on air, as this will send us flying.
	//In case the player touches ground, wakestate will change.
	wakestate=states::air;

	//Guard against things like getting hit by a projectile mid air and landing into an enemy.
	//Pushes the player far to the left.
	if(abs(get_vector_x() < min_vector_hit_guard))
	{
		set_vector({-200.f, -100.f});
	}
	else
	{
		set_vector({-e.get_vector_x(), -100.f});
	}
}

void player::bounce_on_enemy()
{
	state=states::air;
	remaining_jumps=1;

	float vel=p_input.jump_press ? -300.f : -150.f;
	set_vector(vel, axis::y);
}

void player::recieve_effects(player_effects pe)
{
	//TODO: Can we do this in a more idiomatic way?. There's no switch-case for this.
	if(pe.get_effects() & player_effects::triple_jump) add_special(player_effects::specials::triple_jump);
	if(pe.get_effects() & player_effects::all_friendly) add_special(player_effects::specials::all_friendly);
	if(pe.get_effects() & player_effects::extend_trap) add_special(player_effects::specials::extend_trap);
	if(pe.get_effects() & player_effects::slow_down) add_special(player_effects::specials::slow_down);
	if(pe.get_effects() & player_effects::invulnerability) add_special(player_effects::specials::invulnerability);
	if(pe.get_effects() & player_effects::high_jump) add_special(player_effects::specials::high_jump);
	if(pe.get_effects() & player_effects::score_multiplier) add_special(player_effects::specials::score_multiplier);
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
		//TODO:Create new method to avoid referencing periods directly and assert that everything is cool.

		case player_effects::specials::triple_jump:
			max_jumps=extended_jump_quantity;
			if(remaining_jumps==default_jump_quantity) remaining_jumps=extended_jump_quantity;
			specials_period[player_effects::specials::triple_jump]=10.f;
		break;
		case player_effects::specials::all_friendly:
			signals|=s_all_friendly;
		break;
		case player_effects::specials::extend_trap:
			signals|=s_extend_trap;
			specials_period[player_effects::specials::extend_trap]=10.f;
		break;
		case player_effects::specials::slow_down:
			signals|=s_slowdown;
			specials_period[player_effects::specials::slow_down]=5.f;
		break;
		case player_effects::specials::invulnerability:
			specials_period[player_effects::specials::invulnerability]=5.f;
		break;
		case player_effects::specials::high_jump:
			set_vector(-500.f, axis::y);
			remaining_jumps=max_jumps;
			state=states::air;
		break;
		case player_effects::specials::score_multiplier:
			//TODO: Maybe stack more of them????
			score_multiplier=2;
			specials_period[player_effects::specials::score_multiplier]=5.f;
		break;
	}
	remove_special();
}
