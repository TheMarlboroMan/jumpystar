#include "player.h"

using namespace app_game;

player::player()
	:motion_actor(0.f, 0.f, 20, 32), 
	previous_position(get_box()), 
	state(states::air), wakestate(states::air), remaining_jumps(0), cancel_jump(false)
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
	cancel_jump=false;
	specials.clear();
	for(int i=0; i<max_specials; i++) specials.push_back(player_effects::specials::none);
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

				remaining_jumps=2;
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
	dc.set(1);
	auto& b=dc[0];

	b.set_type(draw_struct::types::box);

	auto color=ldv::rgba8(0,0,160,255);

	switch(state)
	{
		case states::ground: break;
		case states::air: color=ldv::rgba8(0,160,0,255); break;
		case states::stunned: color=ldv::rgba8(255,0,0,255); break;
	}

	b.set_color(color);
	b.set_location_box({(int)get_spatiable_x(), (int)get_spatiable_y(), get_spatiable_w(), get_spatiable_h()});
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
		//We did this before and worked nice in Winter.
		//Frenada o aceleración.
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

		if(p_input.jump && remaining_jumps)
		{
			//TODO: Current speed downwards should count.
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
			//Allow for shortening the jump if the button is not pressed.
			float vy=get_vector_y();

			//This takes form of a flag so the jump can only be cancelled once...
			//This affects the weight of the player.
			//TODO: I don't like the effect it has...
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
	set_vector(-150.f, axis::y);
}

void player::recieve_effects(player_effects pe)
{
	if(pe.get_effects() & player_effects::triple_jump) add_special(player_effects::specials::triple_jump);
}

void player::add_special(player_effects::specials sp)
{
	std::swap(specials[2], specials[1]);
	std::swap(specials[1], specials[0]);
	specials[0]=sp;
}

void player::pop_special()
{
	std::swap(specials[1], specials[0]);
	std::swap(specials[2], specials[1]);
	specials[2]=player_effects::specials::none;
}
