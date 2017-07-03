clean:
	rm -rf bin/

createdir:
	mkdir bin/

CFLAGS = -Wall -Wextra -pedantic -std=c99

BUFFER = src/buffer/buffer.c
BUFFER.OBJ = bin/buffer.o

bin/buffer.o: $(BUFFER) src/terminal/termhelper.h 
	$(CC) src/buffer/buffer.c -c -o $(BUFFER.OBJ) $(CFLAGS)

TERMHELPER = src/terminal/termhelper.c
TERMHELPER.OBJ = bin/termhelper.o

bin/termhelper.o: $(TERMHELPER) src/terminal/termhelper.h 
	$(CC) src/terminal/termhelper.c -c -o $(TERMHELPER.OBJ) $(CFLAGS)

ERORHANDLER = src/error_handler/error_handler.c
ERORHANDLER.OBJ = bin/errorhandler.o

bin/errorhandler.o: $(ERORHANDLER) ./src/error_handler/error_handler.h
	$(CC) $(ERORHANDLER) -c -o $(ERORHANDLER.OBJ) $(CFLAGS)

MAIN = src/main.c

bin/nicecode: bin/termhelper.o bin/errorhandler.o bin/buffer.o $(MAIN)
	$(CC) $(MAIN) $(TERMHELPER.OBJ) $(ERORHANDLER.OBJ) -o bin/nicecode  $(CFLAGS)

run: bin/nicecode
	./bin/nicecode
