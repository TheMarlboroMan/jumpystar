#ifndef PLAYER_EFFECTS
#define PLAYER_EFFECTS

#include <vector>

namespace app_game
{

class player_effects
{
	public:
	
	enum class specials{none, triple_jump};
	static const int triple_jump=1;

	int		get_score() const {return score;}
	int		get_effects() const {return effects;}
	void		add_effect(int s) {effects=effects | s;}

	//Stores different scores for the same tic. 
	//TODO: In the future we'll need to add a position for the score marker to appear!.
	void		add_score(int s)
	{
		score+=s;
		scores.push_back(s);
	}

	private:

	std::vector<int>	scores;

	int		score=0,
			effects=0;
};

}

#endif
