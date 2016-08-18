#ifndef DEFINITIONS_H
#define DEFINITIONS_H

namespace app
{
struct definitions
{
	//Defines the unit for the "grid".
	static const int unit=40;

	//Defines the maximum and minimum grid positions for platforms.
	static const int min_x_platform_position=0,
			max_x_platform_position=9;

	//Defines the minimum and maximum grid width for platforms.
	static const int min_w_platform=1,
			max_w_platform=4;

	//Defines the maximum gap between platforms in grid units.
	static const int max_w_platform_gap=4;

	//Half a screen height roughly.. Checked against the distance.
	static const int distance_threshold=550;

	//Bonus data
	static const int base_bonus_chance=25,
			min_bonus_percentage=0,
			max_bonus_percentage=100;
	
	static const int bonus_units_above_ground=4;
};

template<typename T>
T world_to_grid(T v) {return v / definitions::unit;}

template<typename T>
T grid_to_world(T v) {return v * definitions::unit;}
}

#endif
