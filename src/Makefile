CC=i686-w64-mingw32-g++

COMPILER_INCLUDES = -I. -IC:/dev/glm -IC:/dev/glew-1.12.0/glew-1.12.0/include -IC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/x86_64-w64-mingw32/include

LINKER_INCLUDES = -LC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/lib/x86 -LC:/dev/glew-1.12.0/glew-1.12.0/lib

CFLAGS = -std=c++11 -Wall -O2 -DDEBUG -DGLM_FORCE_RADIANS $(COMPILER_INCLUDES)

#LDFLAGS = -lGL -lGLEW -lSDL2
LDFLAGS = -lopengl32 -lglew32 -lmingw32 -lSDL2main -lSDL2 $(LINKER_INCLUDES)

OBJ = 	main.o \
	app/Window.o \
	app/Application.o \
	serialization/TokenStream.o \
	serialization/DataParser.o \
	serialization/ObjectNode.o \
	opengl/Shader.o \
	opengl/Program.o \
	opengl/Interface.o \
	opengl/Camera.o \
	opengl/BufferObject.o \
	opengl/VertexBuffer.o \
	opengl/Texture.o \
	utils/Random.o \

EXECUTABLE = app.exe

all: $(EXECUTABLE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJ)
	$(CC) -o $(EXECUTABLE) $(OBJ) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(OBJ) $(EXECUTABLE)
