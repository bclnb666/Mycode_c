SRC = loghawk.o collector.o processor.o outputer.o
OBJ = loghawk
$(OBJ) : $(SRC)
	gcc -o $@ $^
clean:
	rm -rf $(OBJ) $(SRC)
