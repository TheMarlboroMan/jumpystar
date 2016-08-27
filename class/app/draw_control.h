#ifndef DRAW_CONTROL_H
#define DRAW_CONTROL_H

#include <vector>

#include "draw_struct.h"

namespace app_game
{

class draw_control
{
	private:

	class draw_data
	{
		public:
		std::vector<draw_struct>	structs;
		size_t				current_count=0;
	};

	public:

					draw_control(ldv::resource_manager&);
					draw_control(const draw_control&)=delete;
	draw_control&			operator=(const draw_control&)=delete;

	size_t				get_count() const {return data.current_count;}
	void				ignore() {data.current_count=0;}
	void				reset() {data.current_count=0;}

	//Set the current count.
	void				set(size_t);
	//Get the nth draw_struct
	draw_struct&			operator[](size_t);
	//Get all draw structs and reset current count to 0.
	const draw_data& get_data() {return data;}

	private:

	ldv::resource_manager&		v_manager;
	draw_data			data;
};

}

#endif
