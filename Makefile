SRC_DIR = src
HEADER_DIR = headers

objects = list.o hash_table.o main.o

# executable
mvote : $(objects)
	gcc -o mvote $(objects) -lm

# compile all object files
list.o: $(SRC_DIR)/list.c $(HEADER_DIR)/structs.h $(HEADER_DIR)/header.h
	gcc -c $(SRC_DIR)/list.c

hash_table.o: $(SRC_DIR)/hash_table.c $(HEADER_DIR)/structs.h $(HEADER_DIR)/header.h
	gcc -c $(SRC_DIR)/hash_table.c

main.o: $(SRC_DIR)/main.c $(HEADER_DIR)/structs.h $(HEADER_DIR)/header.h
	gcc -c $(SRC_DIR)/main.c

clean:
	rm mvote $(objects)
