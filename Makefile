OBJ = obj/Gameboy.o obj/MMU.o obj/CPU.o
INC = -I src/

bin/emulator : obj/ bin/ $(OBJ)
	g++ -std=c++17 $(INC) -o bin/emulator src/main.cpp $(OBJ)

obj/%.o : src/%.cpp src/%.h
	g++ -std=c++17 -c $(INC) -o $@ $<

obj/ :
	mkdir obj
bin/ :
	mkdir bin

clean:
	rm -r obj/ bin/