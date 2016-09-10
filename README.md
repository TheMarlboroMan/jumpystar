# Jumpy Star

A happy, family oriented little platformer.

Bugfix:
	- Do not place ground based enemies on small platforms.
	- Don't add two dissapearing or crumbling platforms in a row.
	- Special out of bounds check for projectiles.
		- Just test it, please.
	- Friendly enemies don't shoot!!!
	
Better coding:

	- The whole "can_cancel_jump" and "cancel_jump" deal on the player smells rotten.
	- Use templates to do the filters below distance.

Do better:

	- Different speeds for the patrolling enemies, along with different starting points and directions.
	- Maybe stunned enemies give less score forever.
	- Make the score bonus dependant on something instead of being completely random... 
		- Like good jumps.
		- Like not getting hit.
		- Like not bouncing unneccesarily on platforms.
		- Like god knows.
	- The trap sistem is cool but the implementation sucks a bit... Use tighter limits?.
	- World generation (perhaps branching paths?... Think about it on paper first...)
	- Tweaks to the engine so two states can be rendered at once (for example, render the game and on top of it, render the menu).
	- More than one bonus per platform.
	- More than one enemy per platform.
	- Bonus between platforms, in the vertical space between them.

Features:
	- Lives?
		- Where could we appear?
			- Maybe in a middle platform somewhere?
			- Use some kind of platform latching?
	- Score tables at caballorenoir.net. Retrieve them with DNOTs.
	- Day - Night cycle for the background: starts at midday, ends at night.
	- More enemy types:
		- Flying, descending after the player.
		- Patrolling, no jumping on.
		- Flying, stops and shoots directly at the player.
		- Descending from a side of the screen, shooting nasty patterns.
		- Cannon... Shoots at the player directly in an angle and stuff.
		- Bouncing enemy from side to side.
			- How do we calculate the bounce length to it doesn't fall off?. 
			- Do we just make it invert the direction and that's it?.
			- Could only be trapped when on the ground.
			- Stun it and it would be driven to the ground.
		- Something that chases the player flying but can be jumped on and has to be trapped this way.
			- Once stunned it would fall down.
			- It would have to check itself against platforms when falling!!!.
		- We need more variety here. Think a while...
		
	- Specials: We need more regular specials so there's a point to the best of them.

		- More stuff, maybe?

	- Game speed: get the level duration and speed with the + / - keys.
	- Study bonus random chance and distribution.
	- Study enemy random chance and distribution.
	- Study platform random chance and distribution.
	- Platform types:
		- Completely solid platforms (except for high jump).
			- These are a bitch to implement as are to be done in separate x and y passes.
		- Moving platforms (no spawns), horizontal and... vertical?. Maybe even circular motions?
			- Would involve platform "latching" for the player, with the risk of these latches being deleted.
		- Other fun features (friction).

