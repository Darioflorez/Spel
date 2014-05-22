C++ = g++
CFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lenet -lpthread
FILES = client_last_version.c window.c update_the_surface.c menu2.c
OUT_EXE = CLIENT
#DEPS = window.h
#OBJ = client_last_version.o window.o update_the_surface.o menu2.o
#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)
#CLIENT: $(OBJ)
#	g++ -o $@ $^ $(CFLAGS)

build: $(FILES)
	$(C++) -o $(OUT_EXE) $(FILES) $(CFLAGS) 

clean:
	rm -f *.o core

rebuild: clean build
