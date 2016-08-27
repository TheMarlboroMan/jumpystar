#ifndef PLAYER_EFFECTS
#define PLAYER_EFFECTS

namespace app_game
{

class player_effects
{
	public:
	
	enum class specials{none, triple_jump};
	static const int triple_jump=1;

	int		score=0,
			effects=0;
};

}

#endif
