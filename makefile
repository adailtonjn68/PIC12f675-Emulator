PROJECT:=pic12f675_emulator
INC_DIRS:=./Core/Inc


CFLAGS:=-c -g3 $(addprefix -I,$(INC_DIRS))


all: Debug/$(PROJECT)

Debug/$(PROJECT): Debug/Core/Src/main.o Debug/Core/Src/pic12f675.o
	gcc Debug/Core/Src/main.o Debug/Core/Src/pic12f675.o -o Debug/$(PROJECT)

Debug/Core/Src/main.o: Core/Src/main.c Core/Inc/pic12f675.h	
	gcc Core/Src/main.c $(CFLAGS) -o Debug/Core/Src/main.o

Debug/Core/Src/pic12f675.o: Core/Src/pic12f675.c Core/Inc/pic12f675.h 
	gcc Core/Src/pic12f675.c $(CFLAGS) -o Debug/Core/Src/pic12f675.o

debug: Debug/$(PROJECT)
	gdb -x gdb_commands.txt Debug/$(PROJECT) -tui -q


clean:
	rm -f Debug/Core/Src/*.o
	rm -f Debug/$(PROJECT)
