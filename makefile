CC		= g++
CFLAGS  = -Wall -Wextra -pedantic-errors -std=c++11
INCLUDE = -I ./app/include/
EXE		= $(BIN)test.out
BIN     = ./bin/
DEPS	= $(SRC_OBJ)main.o $(SRC_OBJ)Country.o $(BIN) $(SRC_OBJ)
SRC_OBJ	= ./app/src/obj/

.PHONY : clean run

$(EXE) : $(DEPS)
	$(CC) $(CFLAGS) $(SRC_OBJ)*.o -o $(EXE)

$(SRC_OBJ)main.o : ./app/src/main.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c ./app/src/main.cpp -o $(SRC_OBJ)main.o

$(SRC_OBJ)Country.o : ./app/src/Country.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c ./app/src/Country.cpp -o $(SRC_OBJ)Country.o

$(BIN) :
	@mkdir $(BIN)

$(SRC_OJB) :
	@mkdir $(SRC_OBJ)

clean :
	@rm -f ./data/database.csv
	@rm -f $(SRC_OBJ)*.o
	@rm -f $(EXE)

run : $(EXE)
	@$(EXE)
