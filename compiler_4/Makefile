CC				= gcc
CFLAGS			= -std=c17
TARGET			= parse 
FLEX_OUTPUT 	= scanner.c
BISON_OUTPUT	= parser.c parser.h parser.output
OBJECTS			= main.o scanner.o parser.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lm

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

scanner.o: $(FLEX_OUTPUT)
	$(CC) -c $(CFLAGS) $(FLEX_OUTPUT) -o scanner.o

$(FLEX_OUTPUT): bminor.flex $(BISON_OUTPUT)
	flex -o $(FLEX_OUTPUT) bminor.flex

$(BISON_OUTPUT): bminor.bison
	bison --defines=parser.h --output=parser.c -v bminor.bison

clean:
	rm $(TARGET) $(OBJECTS) $(FLEX_OUTPUT) $(BISON_OUTPUT)
