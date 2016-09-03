#include "game.h"

//std
#include <cassert>

//tools
#include <templates/compatibility_patches.h>

//local
#include "input.h"
#include "../app/player_input.h"

using namespace app;

game_controller::game_controller(ldt::log& plog, ldv::resource_manager& vm, tools::ttf_manager& ttf_man)
	:log(plog),
	camera({0,0,400,500},{0,0}),
	fps_text(ttf_man.get("ad-mono", 12), ldv::rgba8(0,0,0,255), ""),
	distance_text(ttf_man.get("ad-mono", 16), ldv::rgba8(0,0,0,255), ""),
	hud_text(ttf_man.get("ad-mono", 16), ldv::rgba8(0,0,0,255), ""),
	world(player_instance),
	draw_ctrl(vm),
	paused(false)
{
	fps_text.go_to(12, 12);
	distance_text.go_to(12, 30);
	hud_text.go_to(12, 48);
	reset();
}

void game_controller::preloop(dfw::input& /*input*/, float /*delta*/, int fps)
{
	fps_text.set_text(compat::to_string(fps)+"FPS");
}

void game_controller::loop(dfw::input& input, float delta)
{
	if(input().is_exit_signal() || input.is_input_down(input_app::escape))
	{
		set_leave(true);
		return;
	}

	if(input.is_input_down(input_app::pause)) paused=!paused;

	if(!paused)
	{
		do_world_turn(delta);
		do_player_turn(delta, player_instance, get_user_input(input));
		do_player_collisions(player_instance);
	}
}

void game_controller::postloop(dfw::input& /*input*/, float /*delta*/, int /*fps*/)
{

}

void game_controller::draw(ldv::screen& screen)
{
	screen.clear(ldv::rgba8(255, 255, 255, 255));

	std::vector<app_interfaces::drawable const *> drawables;
	drawables.push_back(&player_instance);

	for(const auto& p : world.get_drawables()) drawables.push_back(p);

	app_interfaces::drawable_order dorder;
 	std::sort(std::begin(drawables), std::end(drawables), dorder);

	//TODO: Weirdest bug... Render just one thing and it fucks up.
	for(const auto& d : drawables)
	{	
		d->transform_draw_struct(draw_ctrl);
		const auto& dd=draw_ctrl.get_data();

		for(size_t i=0; i < dd.current_count; i++) 
		{
			assert(dd.structs.size() > i);
			dd.structs[i].rep->draw(screen, camera);
		}

		draw_ctrl.reset();
	}

	//TODO: Fix the above bug...
	ldv::box_representation caja{ldv::box_representation::type::fill, {0,0,6,6}, ldv::rgba8(255,0,0,128)};
	caja.draw(screen);

	fps_text.draw(screen);

	std::string distance_txt=std::to_string(world.get_distance())+"   "+std::to_string((int)player_instance.get_spatiable_x())+","+std::to_string((int)player_instance.get_spatiable_y());
	distance_text.set_text(distance_txt);
	distance_text.draw(screen);

	auto draw_special=[](app_game::player_effects::specials s, std::string& txt)
	{
		switch(s)
		{
			case app_game::player_effects::specials::triple_jump:		txt+="[3]";break;
			case app_game::player_effects::specials::all_friendly:		txt+="[F]";break;
			case app_game::player_effects::specials::extend_trap:		txt+="[_]";break;
			case app_game::player_effects::specials::slow_down:		txt+="[S]";break;
			case app_game::player_effects::specials::invulnerability:	txt+="[*]";break;
			case app_game::player_effects::specials::high_jump:		txt+="[J]";break;
			case app_game::player_effects::specials::score_multiplier:	txt+="[x]";break;
		}	
	};

	std::string hud_txt=std::to_string(player_instance.get_score())+"\n";
	for(const auto& s: player_instance.get_specials())
		draw_special(s, hud_txt);

	hud_txt+=player_instance.get_specials().size() == 3 ? " => " : " == ";
	draw_special(player_instance.get_next_special(), hud_txt);
	
	hud_text.set_text(hud_txt);
	hud_text.draw(screen);

	if(paused)
	{
		ldv::box_representation overlay{ldv::polygon_representation::type::fill, {0,0,screen.get_w(), screen.get_h()}, ldv::rgba8(0, 0, 0, 64)};
		overlay.set_blend(ldv::representation::blends::alpha);
		overlay.draw(screen);
	}

}

void  game_controller::awake()
{

}

void  game_controller::slumber()
{

}

bool game_controller::can_leave_state() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void game_controller::do_player_turn(float delta, app_game::player& pl, app_game::player_input pi)
{
	if(!world.is_moving() && pl.get_spatiable_y() < 350.f)
	{
		world.set_moving(true);
	}

	//Try and force a state change.
	if(pl.can_fall())
	{
		const auto below=pl.get_below_position();
		const auto& co=world.get_collidables();
		if(std::none_of(std::begin(co), std::end(co), [&below](const app_interfaces::spatiable * s) {return s->is_colliding_with(below);}))
		{
			pl.set_falling();
		}
	}
	
	pl.reset_signals();
	pl.get_input(pi);
	pl.turn(delta);
	pl.do_gravity(delta, app::definitions::default_gravity);
	pl.update_previos_position();
	pl.move(delta);

	if(world.is_outside_bounds(pl))
	{
		reset();
	}

	//Signal treatment...
	int sig=pl.get_signals();

	if(sig & app_game::player::s_all_friendly)
	{
		auto cb=camera.get_focus_box();
		app_game::player_effects pe;
		world.trigger_all_friendly_signal(app_interfaces::spatiable::t_box(cb.origin.x, cb.origin.y, cb.w, cb.h), pe);
		pl.recieve_effects(pe);
	}

	if(sig & app_game::player::s_extend_trap)
	{
		app_game::player_trap::set_width(app_game::player_trap::extended_width);
	}

	if(sig & app_game::player::s_reset_trap)
	{
		app_game::player_trap::set_width(app_game::player_trap::default_width);
	}

	if(sig & app_game::player::s_slowdown)
	{
		world.trigger_slowdown(true);
	}

	if(sig & app_game::player::s_reset_slowdown)
	{
		world.trigger_slowdown(false);
	}
}

app_game::player_input game_controller::get_user_input(const dfw::input& input)
{
	app_game::player_input pi{0, 0, false, false};
	if(input.is_input_pressed(input_app::left)) pi.x=-1;
	else if(input.is_input_pressed(input_app::right)) pi.x=1;

	if(input.is_input_down(input_app::up)) pi.y=-1;
	else if(input.is_input_down(input_app::down)) pi.y=1;

	if(input.is_input_down(input_app::jump)) pi.jump=true;
	if(input.is_input_pressed(input_app::jump)) pi.jump_press=true;

//	if(input.is_input_down(input_app::down))
//	{
//		world.create_new_enemy();
//	}

	return pi;
}

void game_controller::do_player_collisions(app_game::player& pl)
{
	//Horizontal limits...
	//TODO: Add constants.
	if(pl.get_spatiable_x() < 0.f)
	{
		pl.adjust(0.f, app_game::motion_actor::adjust_pos::left);
	}
	else if(pl.get_spatiable_ex() > 400.f)
	{
		pl.adjust(400.f, app_game::motion_actor::adjust_pos::right);
	}

	//pickups
	app_game::player_effects pe;
	for(auto& i : world.get_pickups())
	{
		auto& p=*i;
		if(pl.is_colliding_with(p))
		{
			p.get_picked_up(pe);
		}
	}

	//world.
	bool trap_set=false;
	for(const auto& i : world.get_collidables())
	{
		const auto& p=*i;

		if(pl.is_colliding_with(p))
		{
			if(p.is_under(pl.get_previous_position()))
			{
				//Only one trap can be set per tic.
				if(pl.can_set_trap() && !trap_set)
				{
					trap_set=true;
					world.set_player_trap(pl, p); //Cute hearts :D.
				}

				pl.adjust(p, app_game::motion_actor::adjust_pos::bottom);

			}
		}
	}

	//enemies
	for(auto& i : world.get_enemies())
	{
		auto& e=*i;

		if(pl.is_colliding_with(e))
		{
			if(e.is_trapped())
			{
				e.be_friendly(pe);
			}
			else if(pl.can_land_on_enemies() && e.can_be_jumped_on() && e.is_under(pl.get_previous_position()) )
			{
				e.get_jumped_on();
				pl.bounce_on_enemy();
			}
			else if(e.can_harm() && pl.is_vulnerable())
			{
				//The order is important as the player will be propelled in the inverse x direction and the enemy will reverse its direction.
				e.collide_with_player();
				pl.collide_with_harm_actor(e);
			}
		}
	}

	//projectiles...
	for(auto i : world.get_projectiles())
	{
		auto& p=*i;

		if(pl.is_colliding_with(p) && pl.is_vulnerable())
		{
			p.collide_with_player();
			pl.collide_with_harm_actor(p);
		}
	}

	pl.recieve_effects(pe);
}

void game_controller::do_world_turn(float delta)
{
	world.do_turn(delta);

	if(world.is_moving())
	{
		camera.move_by(0, -world.get_camera_movement());
	}
}

void game_controller::reset()
{
	world.reset();
	world.init();
	player_instance.reset();
	camera.go_to({0,0});
}
