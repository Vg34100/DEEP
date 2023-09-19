# DEEP
### Dungeon Explorer's Endless Pursuit

###### Current Version
- Launches Window
- CONTROLS
    - WASD  - Move
    - Mouse - Look
    - R     - UseWeapon
    - ESC   - Quit
    - F     - Fullscreen (kinda bugged)
- Player facing direction dependent on Mouse Cursor Location
- Top Down Camera with Camera Lag
- Basic Dummy Enemy (Orc)
    - Health
    - Takes Damage
- Based Dummy Weapon (LIGHTS)
    - Deals Damage
- Hitbox structs handle collision between Weapon and Enemy
- World generates a random Level at initialization
    - After Enemies are cleared
        - Collision Detection with Hallways opens
        - If pass Hallways, new Level generated in direction
    - World collision with player handled by raycast, checks what TILE is underneath Player (could use some work)
    - World made up of Tiles
        - WALL | FLOOR | ENEMY | PIT | HALLWAY(N|E|S|W)
###### Current State of Files - TBI = To be implemented
- Collision Manager
    - Manages the Colliions between various objects (Player->World | Weapon->Enemy)
- Health
    - Manages health, with an InitialHealth, MaxHealth, and TakeDamge functions
    - Displays healthbar if rendering the function
    - Should/Could handle Temperary Health, Invulnerability, and Regeneration
- ICollidable
    - for collidable objects? 
- Vector 2
    - Holds Vector 2 Struct, holds x,y variables and has various functions for Vector2
    - Also holds Hitbox Struct which is composed of two Vectors, a topLeft and bottomRight position, could be moved
- Enemy
    - Holds the base enemy class, enemies hold a name, attackDamage, attackCooldown, health, size, position, a reference to world, invicibilityDuration, invulnerability bool, lastDamageTime float, a hitBox, and a isDead bool
    - Add subchildren to base enemy class with other enemies
        - Can override to have specific render or attack functions (probably should have others too)
- GUI-TitleScreen
    - Titlescreen when the game starts
- Player
    - is the Player class
    - Players have stats for Health, Mana(TBI), Ammo(TBI), other Major Attributes
    - Speed, Damage, Resistance, AttackSpeed, Range, Luck, Size, playerWidth, and playerHeight
    - playerPos, mousePos, cameraPos, playerVelocity, playerDirection, activeWeapon, and a Hitbox
    - player movement is handled in handleInput and in handleMovement
    - useWeapon causes an attack from the atctive weapon
- FPSManger
    - idk man
- initGLX
    - most of the initialization stuff for starting the window, or closing the window, so stuff related to X11, and OpenGL
    - also holds GLUT's text rendering method, which could still be improved opon
    - handles initial events, which are the initial inputs
    - exports the keyspressed, the width and height of the window, and the mouse position
    - handles window resizing and fullscreen
- Weapon
    - has a base weapon class, extends to 4 different classes of Melee, Magic, Ranged, or Summoning
    - weapon has a hitbox, attacking "activates" the hitbox
    - weapons should override use, render, and update?
    - weapons hold a name, damage, cooldown (time till next attack), attackSize, speed, duration(how far it will go), damageType(TBI), weaponClass(no real difference yet), bool for isAttacking, bool for onCooldown, and a lastUseTime float, as well as a hitBox
- Level
    - holds the level class
    - a level is just a class that holds 3 things, a name, a layout, and enemies
    - the layout is made up of strings that each character represents a tiletype
    - "-" = Wall | "x" = Floor | "^" = Pit (doesnt do anything yet) | "*" = Enemy (Causes an enemy to spawn from the determined enemy list) | "||" = Doors | Directional Dependent Hallways "N"(North) "E"(East) "W"(West) "S"(South)
    - also holds a levelManager class with some helpful functions when working with levels
- World
    - World on initialzation talks to LevelManger to get a randomLevel to spawn
    - generated the Level based on the tiles
    - spawns the proceeding level when it communiates with collisionManager to know which directional hallway the player took


###### Stuff to Fix
- Memory Leaks when Game Closes
- FPSManager doesn't actually fix the same FPS problem
    - Fix FPS so the game runs the same on different systems
- Movement - Specifically Direction Movement
- World not loading sometimes for Vincent? 
- Fix the Fullscreen stuff
- World collision with player not all accurate. 
- Text thickness, or a better method for text in general
- Add more tiles (Wildcard idea)
- Make tiles rendering dependent on a theme (Forest? - Green Tiles | Cavern? - Dark Tiles | etc...)
- Enemy AI
- Attacking should probably be reworked to allow multiple attacks out at a time when it comes to ranged weapons


###### Initial Draft
- PLAYER (Player.cpp)
    - COMBAT
        - "SLASH"
            - When "Clicking" -> Attack (Summon another cube that lasts a bit before disspearing to simulate a slash for melee combat)
        - Weapon Variety?
            - Ranged weapons, magic wands, different melee weapons, etc.
    - HEALTH - (Modularity - Should be Health.cpp - Can be used with Player.cpp {enemy}.cpp)
        - GUI for Hearts
        - Starting point 5 hearts, total of 10 hit points
    - GAME OVER
        - If Player Health == 0 -> GAME OVER (Stop the game action, spawn GUI screen, allow for RESTART or QUIT)
    - DODGE
        - A quick dodge or roll to evade enemy attacks.
    - INVENTORY?
        - To carry potions, artifacts, and other usable items.
    - UPGRADES?
        - Acquire new skills or enhance existing ones.
    - CURRENCY
        - Coins or some form of currency to purchase items or upgrades.
- ENEMIES
    - Some Cube that will move towards the Player
    - If Enemy collides with Player -> Player loses health
    - (LATER)
        - Different types of enemies with unique behaviors and attack patterns.
        - Boss Battles
- WORLD
    - Procedurally Generated Dungeons
    - Different Themes
    - Traps
        - Spikes - Pitfalls, Moving Walls, etc.
    - Backstory? 
        - Why is the player exploring these dungeons? What's the ultimate goal?
        - Non-player characters that offer quests, lore, or other interactions.
- PROGRESSION
    - While players start from the beginning upon death, they retain some form of progression (e.g., unlocked weapons, skills).
- GRAPHICS/SOUNDS (LATER)
    - ART STYLE
    - SOUND EFFECTS
        - Background music for different biomes, sound effects for actions like attacks, getting hit, etc.
- UI
    - HEALTH
    - PAUSE & SETTINGS
    - MINIMAP?
    - TOOL TIPS? 
        - Hover over items to see their descriptions.
