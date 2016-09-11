# Jumpy Star

A happy, family oriented little platformer.

Bugfix:
	- Do not place ground based enemies on small platforms.
	- Don't add two dissapearing or crumbling platforms in a row.
	- Friendly enemies don't shoot!!!
	- Befriended chaser should fly away!!!.
	
Better coding:

	- Correct the chaser path better, I'd like it to turn instead of instantly change directions.
		- Perhaps sum the previous and the current vector, normalize the result and then do the multiplication.
	- The whole "can_cancel_jump" and "cancel_jump" deal on the player smells rotten.
	- Use templates to do the filters below distance.
	- Separate factory classes for platforms, enemies and bonus, to unclutter "world".
	- The whole "period" for the enemies is starting to grow weary. Can we have a class for this with different periods?
		- It could have "tic_all", "tic({periods})", "is_done({period})", "set(period, time)"...
		- Replace on enemies.
		- Replace on particles.
		- Replace on player.
	- do_gravity() could be included in "move", maybe prior to it. Check with the player first.

Do better:

	- Hop on flying enemies and parabol and stuff. Also shoot them.
		- Would make them fall for a while.
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
		- Appears at the top of the screen, does a flyby dropping some stuff and goes away.
		- Flying, descending after the player from the top.
		- Patrolling, no jumping on.
		- Flying, stops and shoots directly at the player.
		- Descending from a side of the screen, shooting nasty patterns.
		- Cannon... Shoots at the player directly in an angle and stuff.
		- Bouncing enemy from side to side.
			- How do we calculate the bounce length to it doesn't fall off?. 
			- Do we just make it invert the direction and that's it?.
			- Could only be trapped when on the ground.
			- Stun it and it would be driven to the ground.
		- We need more variety here. Think a while...
		
	- Specials: We need more regular specials so there's a point to the best of them.

		- More stuff, maybe?

	- Redefine controls.
		- Try and set up these pesky tool menus again.
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

