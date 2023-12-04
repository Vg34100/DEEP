# Compiler and flags
CXX = g++
CXXFLAGS = -Wall
LDLIBS = -lX11 -lGL -lGLU
# Source files
SRCS = main.cpp \
       initGLX.cpp \
       world.cpp \
       player.cpp \
       enemy.cpp \
       util_vector2.cpp \
       util_health.cpp \
       util_CollisionManager.cpp \
       world_level.cpp \
       vto.cpp \
       jlo.cpp \
       apacheco2.cpp \
       prodriguezqu.cpp \
       image.cpp \
       libggfonts.a \
       gui.cpp \
       accessories.cpp

HEDS = initGLX.h \
       world.h \
       player.h \
       enemy.h \
       util_vector2.h \
       util_health.h \
       util_CollisionManager.h \
       world_level.h \
       vto.h \
       jlo.h \
       apacheco2.h \
       prodriguezqu.h \
       image.h \
       fonts.h \
       gui.h  \
       keybinds.h \
       accessories.h

# Object files (this is a "phony" target, no actual .o files are generated)
OBJS = $(SRCS:%.cpp=objects/%.o)
DOBJS = $(SRCS:%.cpp=objects/D_%.o)

all: game

game: $(OBJS) $(HEDS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

debug: CXXFLAGS += -DDEBUG
debug: $(DOBJS) $(HEDS)
	$(CXX) $(CXXFLAGS) -o $@ $(DOBJS) $(LDLIBS)

objects/%.o: %.cpp
	mkdir -p objects
	$(CXX) $(CXXFLAGS) -c $< -o $@

objects/D_%.o: %.cpp
	mkdir -p objects
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f game debug objects/*.o
	rm -rf objects
