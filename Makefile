CC=g++

COMPILER_INCLUDES = -I. -IC:/dev/glm -IC:/dev/glew-1.12.0/glew-1.12.0/include -IC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/x86_64-w64-mingw32/include

LINKER_INCLUDES = -LC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/lib/x86 -LC:/dev/glew-1.12.0/glew-1.12.0/lib

CFLAGS = -std=c++11 -Wall -O2 -DDEBUG -DGLM_FORCE_RADIANS 

LDFLAGS =

EXECUTABLE =

ifeq ($(OS),Windows_NT)
	CFLAGS += -I../src -IC:/dev/glm -IC:/dev/glew-1.12.0/glew-1.12.0/include -IC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/x86_64-w64-mingw32/include -IC:/dev/yaml-cpp/include -IC:/dev/boost_1_58_0 $(COMPILER_INCLUDES)
	LDFLAGS += -lopengl32 -lglew32 -lmingw32 -lSDL2main -lSDL2 $(LINKER_INCLUDES)
	EXECUTABLE += app.exe
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS +=  -I ./src
        LDFLAGS +=  -lGL -lGLEW -lSDL2 
	EXECUTABLE += app
    endif
endif

OBJ = 	src/main.o \
	src/app/Window.o \
	src/app/Application.o \
	src/serialization/TokenStream.o \
	src/serialization/DataParser.o \
	src/serialization/ObjectNode.o \
	src/opengl/Shader.o \
	src/opengl/Program.o \
	src/opengl/Interface.o \
	src/opengl/Camera.o \
	src/opengl/BufferObject.o \
	src/opengl/VertexBuffer.o \
	src/opengl/Texture.o \
	src/utils/Random.o \

all: $(EXECUTABLE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJ)
	$(CC) -o $(EXECUTABLE) $(OBJ) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(OBJ) $(EXECUTABLE)
