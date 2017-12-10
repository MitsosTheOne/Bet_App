CC = g++
OBJ = main.o  categories.o users.o
DEPS = categories.h users.h 
EXE = bet
CCFLAGS =

$(EXE): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $^ 

%.o: %.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c -o $@ $< 

.PHONY: clean

clean:
	rm -f $(EXE) $(OBJ)

#compile for gdb
d:
	$(CC) -o $(EXE) $(OBJ) -g3

