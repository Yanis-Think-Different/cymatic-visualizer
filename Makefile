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

% : $(BIN)/%.o
	$(CC) $(LPATH) $< -o $@ $(LFLAGS)

$(BIN)/%.o : $(SRC)/%.c
	$(CC) -c $(STD) $(PFLAGS) $(CFLAGS) $< -o $@

clean :
	rm -f $(BIN)/*

cleanall : clean
	rm -f $(DOC)/*