CC = gcc
CFLAGS = -Iinclude -Wall -Wextra

OBJ = main.o src/ExtendBuffer.o src/file_io.o src/ItemDefinition.o src/ItemsDat.o

all: itemsdat_decoder

itemsdat_decoder: $(OBJ)
	$(CC) $(OBJ) -o itemsdat_decoder

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) itemsdat_decoder
