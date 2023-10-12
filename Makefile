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
       libggfonts.a

# Object files (this is a "phony" target, no actual .o files are generated)
OBJS = $(SRCS:%.cpp=objects/%.o)

all: game debug

game: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

debug: CXXFLAGS += -DDEBUG
debug: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

objects/%.o: %.cpp
	mkdir -p objects
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f game debug objects/*.o
	rm -rf objects
