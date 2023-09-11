all: game

game: init.cpp src/setup/rendering_setup.cpp src/setup/fonts.a src/player/player.cpp
	g++ init.cpp src/setup/rendering_setup.cpp src/setup/fonts.a src/player/player.cpp -o game -lrt -lX11 -lGLU -lGL -pthread -lm

clean:
	rm -f game