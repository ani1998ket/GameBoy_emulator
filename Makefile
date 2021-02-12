OBJ = obj/Gameboy.o obj/MMU.o obj/CPU.o obj/CPU_init.o
HEADERS = src/Gameboy.h src/MMU.h src/CPU.h src/utils.h src/InstructionSet.h
INC = -I src/


bin/emulator : obj/ bin/ $(OBJ) $(HEADERS)
	g++ -std=c++17 $(INC) -o bin/emulator src/main.cpp $(OBJ)

obj/%.o : src/%.cpp
	g++ -std=c++17 -c $(INC) -o $@ $<

obj/ :
	mkdir obj
bin/ :
	mkdir bin

clean:
	rm -r obj/ bin/