CFLAGS = -Wall -Wextra -pedantic -std=c99

default: run

MODULES = \
	src/buffer/buffer.c               \
	src/terminal/termhelper.c         \
	src/error_handler/error_handler.c \
	src/logger/logger.c

HEADERS = $(MODULES:.c=.h)
OBJS = $(MODULES:.c=.o)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

EXE = bin/nicecode
$(EXE): $(OBJS) src/main.c
	$(CC) $(CFLAGS) $(OBJS) src/main.c -o bin/nicecode

.PHONY: clean
clean:
	rm -rf bin/ src/**/*.o
	mkdir bin/

run: $(EXE)
	./bin/nicecode