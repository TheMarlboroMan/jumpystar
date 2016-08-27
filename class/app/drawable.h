#ifndef DRAWABLE_INTERFACE_H
#define DRAWABLE_INTERFACE_H

#include <libDan2.h>
#include "draw_control.h"

namespace app_interfaces
{

class drawable
{
	public:

	virtual 		~drawable() {}
	virtual int		get_draw_order() const=0;
	virtual void 		transform_draw_struct(app_game::draw_control&) const=0;
};

class drawable_order
{
	public: 

	bool operator()(const drawable* a, const drawable* b) const
	{
		return a->get_draw_order() < b->get_draw_order();
	}
};

}
#endif
