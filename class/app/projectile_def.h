#ifndef PROJECTILE_DEF_H
#define PROJECTILE_DEF_H

#include "motionable.h"
#include "spatiable.h"

namespace app_game
{
//Definition for a projectile... Not all enemies will shoot, but whatever
//makes it easier.
class projectile_def
{
	public:
	app_interfaces::spatiable::t_point		origin;
	app_interfaces::motionable::t_vector		direction;
	enum class types {parabol}			type;
	enum class sides {enemy, player}		side;
};
}

#endif
