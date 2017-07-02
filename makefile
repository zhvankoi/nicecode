clean:
	rm -rf bin/

createdir: clean
	mkdir bin/

CFLAGS = -Wall -Wextra -pedantic -std=c99
MAIN = ./src/main.c ./src/error_handler/error_handler.c ./src/terminal/termhelper.c

build: createdir $(MAIN)
	$(CC) $(MAIN) -o bin/nicecode $(CFLAGS)

run: build
	./bin/nicecode
