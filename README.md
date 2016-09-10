# Jumpy Star

A happy, family oriented little platformer.

Bugfix:
	- Do not place ground based enemies on small platforms.
	
Better coding:

	- High jump from the top: less robotic movement, slowly drag the players towards the limit.
	- The whole "can_cancel_jump" and "cancel_jump" deal on the player smells rotten.
	- Use templates to do the filters below distance.
	- Perhaps make DNOT compatible with JSON.

Do better:

	- The trap sistem is cool but the implementation sucks a bit... Use tighter limits?.
	- World generation (perhaps branching paths?... Think about it on paper first...)
	- Tweaks to the engine so two states can be rendered at once (for example, render the game and on top of it, render the menu).
	- More than one bonus per platform.
	- More than one enemy per platform.
	- Bonus between platforms.

Features:
	- Lives?
		- Where could we appear?
			- Maybe in a middle platform somewhere?
	- Score tables at caballorenoir.net. Retrieve them with DNOTs.
	- Day - Night cycle for the background: starts at midday, ends at night.
	- More enemy types:
		- Patrolling horizontal shooter (walks, pauses, shoots, walks...).
		- Flying, descending after the player.
		- Patrolling, no jumping on.
		- Flying, stops and shoots directly at the player.
		- Descending from a side of the screen, shooting nasty patterns.
		- Cannon... Shoots at the player directly in an angle and stuff.
		- We need more variety here. Think a while...
		
	- Specials: We need more regular specials so there's a point to the best of them.

		- More stuff, maybe?

	- Game speed: get the level duration and speed with the + / - keys.
	- Add bonus random chance.
	- Platform types:
		- Completely solid platforms (except for high jump).
			- These are a bitch to implement as are to be done in separate x and y passes.
		- Moving platforms (no spawns), horizontal and... vertical?. Maybe even circular motions?
			- Would involve platform "latching" for the player, with the risk of these latches being deleted.
		- Other fun features (friction).

