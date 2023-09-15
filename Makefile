all: game

game: init.cpp src/setup/rendering_setup.cpp src/setup/fonts.a src/player/player.cpp src/common/health.cpp src/common/bullet.cpp src/common/bullet_manager.cpp src/common/weapon.cpp src/common/weapon.h src/enemy/enemy.cpp
	g++ init.cpp src/setup/rendering_setup.cpp src/setup/fonts.a src/player/player.cpp src/common/health.cpp src/common/bullet.cpp src/common/bullet_manager.cpp src/common/weapon.cpp src/enemy/enemy.cpp -o game -lrt -lX11 -lGLU -lGL -pthread -lm

clean:
	rm -f game