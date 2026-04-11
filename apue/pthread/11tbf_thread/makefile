LDFLAGS += -pthread
SRC = main.o tbf.o
OBJ = target
$(OBJ) : $(SRC) $(LDFLAGS)
	gcc -o $@ $^
clean :
	rm -rf $(OBJ) $(SRC)
