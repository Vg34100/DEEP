# Compiler and flags
CXX = g++
CXXFLAGS = -Wall
LDLIBS = -lX11 -lGL -lGLU
# Source files
SRCS = main.cpp \
       setup/initGLX.cpp \
       setup/FPSManager.cpp \
       gui/titleScreen.cpp \
       world/world.cpp \
       player/player.cpp \
       enemies/enemy.cpp \
       common/vector2.cpp \
       common/health.cpp \
       common/CollisionManager.cpp \
       world/level.cpp \
       vto.cpp \
       jlo.cpp \
       apacheco2.cpp \
       prodriguezqu.cpp

# Object files (this is a "phony" target, no actual .o files are generated)
OBJS = $(SRCS:.cpp=.o)

all: game debug

game: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

debug: CXXFLAGS += -DDEBUG
debug: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f game debug $(OBJS)
