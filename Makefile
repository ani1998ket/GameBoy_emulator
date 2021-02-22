OBJ = obj/Gameboy.o obj/MMU.o obj/CPU.o obj/CPU_init.o obj/Flag.o obj/Screen.o
HEADERS = src/Gameboy.h src/MMU.h src/CPU.h src/utils.h src/InstructionSet.h src/Flag.h src/Screen.h
INC = -I src/
LINK = -lsfml-system -lsfml-window -lsfml-graphics


bin/emulator : obj/ bin/ $(OBJ) $(HEADERS)
	g++ -std=c++17 $(INC) -o bin/emulator src/main.cpp $(OBJ) $(LINK)

obj/CPU_init.o : src/CPU_init.cpp
	g++ -std=c++17 -c $(INC) -o $@ $< $(LINK)

obj/%.o : src/%.cpp src/%.h
	g++ -std=c++17 -c $(INC) -o $@ $< $(LINK)

obj/ :
	mkdir obj
bin/ :
	mkdir bin

clean:
	rm -r obj/ bin/