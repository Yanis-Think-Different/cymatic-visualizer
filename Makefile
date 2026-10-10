CC = clang
STD = -std=c17

SRC = ./src
INC = ./inc
BIN = ./bin
DOC = ./doc

PFLAGS = -I$(INC)
CFLAGS = -Wall -Wpointer-arith
LPATH =
LFLAGS = -lm -lraylib

EXEC = test
OBJ = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(wildcard $(SRC)/*.c))

$(EXEC) : $(OBJ)
	$(CC) $(LPATH) $^ -o $@ $(LFLAGS)

$(BIN)/%.o : $(SRC)/%.c
	$(CC) -c $(STD) $(PFLAGS) $(CFLAGS) $< -o $@

clean :
	rm -f $(BIN)/*

cleanall : clean
	rm -f $(DOC)/*
