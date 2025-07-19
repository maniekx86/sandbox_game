CXX = g++

ifeq ($(OS),Windows_NT)
	CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -DSFML_STATIC
	LDFLAGS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -static -static-libgcc -static-libstdc++ -lgdi32 -lopengl32 -lwinmm -Wl,-Bstatic $$(pkg-config --cflags --libs --static freetype2) -Wl,-subsystem,windows
	
	ASM_SRC = resources/resources_win.s

else
	CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
	LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -Wl,-Bdynamic -Wl,-Bstatic -lstdc++ -pthread -static-libstdc++ -static-libgcc 
	
	ASM_SRC = resources/resources.s
	
endif

# optimization
CXXFLAGS += -Ofast -ffast-math -march=core2 -fomit-frame-pointer -funroll-loops 

SRC_DIR = .
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/elements/*.cpp $(SRC_DIR)/physics/*.cpp $(SRC_DIR)/ui/*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)


ASM_OBJ = $(ASM_SRC:.s=.o)

EXEC = sandbox

all: $(EXEC)

$(EXEC): $(OBJ_FILES) $(ASM_OBJ)
	$(CXX) $(OBJ_FILES) $(ASM_OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.s
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(ASM_OBJ) $(EXEC)

.PHONY: all clean