CFLAGS = -Wall -Wextra -pedantic -std=c99

default: run

BUFFER = src/buffer/buffer.c
bin/buffer.o: $(BUFFER) $(BUFFER:.c=.h)
	$(CC) $(CFLAGS) -c $(BUFFER) -o bin/buffer.o

TERMINAL = src/terminal/termhelper.c
bin/termhelper.o: $(TERMINAL) $(TERMINAL:.c=.h)
	$(CC) $(CFLAGS) -c $(TERMINAL) -o bin/termhelper.o

ERROR_HANDLER = src/error_handler/error_handler.c
bin/error_handler.o: bin/termhelper.o $(ERROR_HANDLER) $(ERROR_HANDLER:.c=.h)
	$(CC) $(CFLAGS) -c $(ERROR_HANDLER) -o bin/error_handler.o

OBJS = $(addprefix bin/,buffer.o termhelper.o error_handler.o)
EXE = bin/nicecode
$(EXE): $(OBJS) src/main.c
	$(CC) $(CFLAGS) $(OBJS) src/main.c -o bin/nicecode

.PHONY: clean
clean:
	rm -rf bin/
	mkdir bin/

run: $(EXE)
	./bin/nicecode