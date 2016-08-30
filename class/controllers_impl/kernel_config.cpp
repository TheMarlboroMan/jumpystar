#include "kernel_config.h"

//std
#include <iostream>

//sdl
#include <SDL2/SDL.h>

//libdansdl2
#include <input/sdl_input/sdl_input.h>

//tools
#include <class/text_reader.h>

//local
#include "input.h"

using namespace app;

kernel_config::kernel_config(const app_config& c)
	:config(c)
{

}

std::vector<std::string> kernel_config::get_sound_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/audio.txt"));
}

std::vector<std::string> kernel_config::get_music_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/music.txt"));
}

std::vector<std::string> kernel_config::get_texture_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/textures.txt"));
}

std::vector<std::string> kernel_config::get_surface_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/surfaces.txt"));
}

dfw::window_info kernel_config::get_window_info() const
{
	return dfw::window_info{400, 500, 400, 500, "** Jumpy star **", true};
}

std::vector<dfw::input_pair> kernel_config::get_input_pairs() const
{
	auto type_from_config=[](int t)
	{
		using namespace dfw;

		switch(t)
		{
			case 0: return input_pair::types::keyboard; break;
			case 1: return input_pair::types::joystick; break;
			case 2: return input_pair::types::mouse; break;
		}

		return input_pair::types::keyboard;
	};

	//TODO: This input stuff is hideous!!.
	auto get_user_info=[this](std::map<int, app_config::user_input>& res, int ileft, int iright, int iup, int idown, int ijump, int ipause)
	{
		res[ileft]=config.get_left();
		res[iright]=config.get_right();
		res[iup]=config.get_up();
		res[idown]=config.get_down();
		res[ijump]=config.get_jump();
		res[ipause]=config.get_pause();
	};

	using namespace dfw;
	std::vector<input_pair> res{
		input_pair{input_pair::types::keyboard, input_app::escape, SDL_SCANCODE_ESCAPE, 0}};

	std::map<int, app_config::user_input> mapa;

	get_user_info(mapa, input_app::left, input_app::right, input_app::up, input_app::down, input_app::jump, input_app::pause);

	for(const auto& p : mapa) res.push_back({type_from_config(p.second.type), p.first, p.second.code, p.second.device});

	return res;
}
