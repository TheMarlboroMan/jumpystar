#ifndef DEFINITIONS_H
#define DEFINITIONS_H

namespace app
{
struct definitions
{
	//Defines the unit for the "grid".
	static const int unit=40,

	//Defines the maximum and minimum grid positions for platforms.
			min_x_platform_position=0,
			max_x_platform_position=9,

	//Defines the minimum and maximum grid width for platforms.
			min_w_platform=1,
			max_w_platform=4,

	//Defines the maximum gap between platforms in grid units.
			max_w_platform_gap=4,

	//Half a screen height roughly.. Checked against the distance.
			distance_threshold=550,
	//Just what it seems.
			playground_min_x=0,
			playground_width=400,
			playground_height=500,

	//Bonus data
			base_bonus_chance=25,
			min_bonus_percentage=0,
			max_bonus_percentage=100,
	
	//The bonus are lifted from the ground by this value.
			bonus_units_above_ground=4,
			default_enemy_stun_time=2,
			default_enemy_trap_time=3,

	//Distance to top to activate autoscroll when high jumping.
			high_jump_scroll_threshold=200;

	static const float default_gravity;
	//Enough force to reach the middle of the screen, more or less.
};

template<typename T>
T world_to_grid(T v) {return v / definitions::unit;}

template<typename T>
T grid_to_world(T v) {return v * definitions::unit;}
}

#endif
