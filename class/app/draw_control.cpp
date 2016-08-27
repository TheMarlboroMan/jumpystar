#include "draw_control.h"

#include <stdexcept>

#include <templates/compatibility_patches.h>

using namespace app_game;

draw_control::draw_control(ldv::resource_manager& rm)
	:v_manager(rm)
{

}

void draw_control::set(size_t n)
{
	data.current_count=n;

	while(data.structs.size() < n)
		data.structs.push_back({v_manager});
}

draw_struct& draw_control::operator[](size_t n)
{
	if(data.structs.size() <= n)
	{
		throw std::runtime_error("Requested draw_struct outside bounds ("+compat::to_string(n)+" of "+compat::to_string(data.structs.size())+")");
	}
	
	return data.structs[n];
}
