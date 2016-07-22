#include "game.h"

#include "input.h"
#include "../app/player_input.h"

using namespace app;

game_controller::game_controller(ldt::log& log, ldv::resource_manager& vm)
	:log(log),
	camera({0,0,400,500},{0,0}),
	world(),
	draw_struct(vm)
{
	reset();
}

void game_controller::preloop(dfw::input& input, float delta, int fps)
{
	
}

void game_controller::loop(dfw::input& input, float delta)
{
	if(input().is_exit_signal() || input.is_input_down(input_app::escape))
	{
		set_leave(true);
		return;
	}

	do_world_turn(delta);
	do_player_turn(delta, player_instance, get_user_input(input));
	do_player_collisions(player_instance);
}

void game_controller::postloop(dfw::input& input, float delta, int fps)
{

}

void game_controller::draw(ldv::screen& screen)
{
	screen.clear(ldv::rgba8(255, 255, 255, 255));

	std::vector<app_interfaces::drawable const *> drawables;
	drawables.push_back(&player_instance);
	for(const auto& p : world.get_platforms()) drawables.push_back(&p);

	app_interfaces::drawable_order dorder;
 	std::sort(std::begin(drawables), std::end(drawables), dorder);

	//TODO: Weirdest bug... Render just one thing and it fucks up.
	for(const auto& d : drawables)
	{
		d->transform_draw_struct(draw_struct);
		draw_struct.rep->draw(screen, camera);
	}

	//TODO: Fix the above bug...
	ldv::box_representation caja{ldv::box_representation::type::fill, {0,0,6,6}, ldv::rgba8(255,0,0,128)};
	caja.draw(screen);
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

	//TODO: Check if player is in air to change its state.
	
	pl.get_input(pi);
	pl.turn(delta);
	pl.do_gravity(delta);
	pl.update_previos_position();
	pl.move(delta);

	if(world.is_outside_bounds(pl))
	{
		reset();
	}
}

app_game::player_input game_controller::get_user_input(const dfw::input& input)
{
	app_game::player_input pi{0, false};
	if(input.is_input_pressed(input_app::left)) pi.x=-1;
	else if(input.is_input_pressed(input_app::right)) pi.x=1;

	if(input.is_input_pressed(input_app::up)) pi.y=-1;
	else if(input.is_input_pressed(input_app::down)) pi.y=1;

	if(input.is_input_down(input_app::jump)) pi.jump=true;

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

	//world.
	for(const auto& p : world.get_platforms())
	{
		if(pl.is_colliding_with(p))
		{
			if(p.is_under(pl.get_previous_position()))
			{
				pl.adjust(p, app_game::motion_actor::adjust_pos::bottom);
			}
		}
	}
}

void game_controller::do_world_turn(float delta)
{
	if(world.is_moving())
	{
		world.do_turn(delta);
		world.generate_new_world();
		world.delete_discarded_objects();
		camera.move_by(0, -world.get_camera_movement());

//		std::cout<<"THERE ARE "<<world.get_platforms().size()<<" PLATFORMS REMAINING"<<std::endl;
	}
}

void game_controller::reset()
{
	world.reset();
	world.init();
	player_instance.set_position(20.f, 400.f);
	camera.go_to({0,0});
}
