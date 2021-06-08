CC		= g++
CFLAGS  = -Wall -Wextra -Werror -pedantic-errors -std=c++11
INCLUDE = -I ./app/include/
EXE		= ./bin/test.out
DEPS	= $(SRC_OBJ)main.o
SRC_OBJ	= ./app/src/obj/

.PHONY : clean run

$(EXE) : $(DEPS)
	$(CC) $(CFLAGS) $(SRC_OBJ)*.o -o $(EXE)

$(SRC_OBJ)main.o : ./app/src/main.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c ./app/src/main.cpp -o $(SRC_OBJ)main.o

clean :
	@rm -f ./data/countryRecords.csv
	@rm -f $(SRC_OBJ)*.o
	@rm -f $(INC_OBJ)*.o
	@rm -f $(EXE)

run : $(EXE)
	@$(EXE)
