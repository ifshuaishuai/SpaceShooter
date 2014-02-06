CXX = g++
FLAGS = -Wall -g -std=c++0x
LIBS = -lsfml-graphics -lsfml-window -lsfml-system 

DEPS = Aircraft.hpp \
		AnimatedCircle.hpp \
		Application.hpp \
		Category.hpp \
		Command.hpp \
		CommandQueue.hpp \
		DataTables.hpp \
		Entity.hpp \
		GameOverState.hpp \
		GameState.hpp \
		LoadingState.hpp \
		MenuState.hpp \
		ParallelTask.hpp \
		PauseState.hpp \
		Pickup.hpp \
		Player.hpp \
		Projectile.hpp \
		ResourceHolder.hpp \
		ResourceIdentifier.hpp \
		SceneNode.hpp \
		SpriteNode.hpp \
		State.hpp \
		StateIdentifiers.hpp \
		StateStack.hpp \
		TextNode.hpp \
		TitleState.hpp \
		Utility.hpp \
		World.hpp \
		templateUsed.cpp
		
OBJ = main.o \
		Aircraft.o \
		AnimatedCircle.o \
		Application.o \
		Command.o \
		CommandQueue.o \
		DataTables.o \
		Entity.o \
		GameOverState.o \
		GameState.o \
		LoadingState.o \
		MenuState.o \
		ParallelTask.o \
		PauseState.o \
		Pickup.o \
		Player.o \
		Projectile.o \
		ResourceHolder.o \
		SceneNode.o \
		SpriteNode.o \
		State.o \
		StateStack.o \
		TextNode.o \
		TitleState.o \
		Utility.o \
		World.o \
		templateUsed.o \

TARGET = main

%.o : %.cpp $(DEPS)
	$(CXX) $(FLAGS) -c -o $@ $<

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

.PHONY:clean

clean :
	rm -f $(TARGET) $(OBJ)