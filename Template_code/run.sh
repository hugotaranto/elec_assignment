gcc wall.c formulas.c robot.c main.c -o main $(pkg-config --cflags-only-I sdl2) $(pkg-config --libs-only-L sdl2) $(pkg-config --libs-only-l sdl2)
./main