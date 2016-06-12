CC = gcc
CFLAGS = -g -Wall -Werror -lm -lSOIL -lGLEW -lGL -lGLU -lglut
TARGET = shaderTest

default: $(TARGET)

$(TARGET): $(TARGET).o ezloader.o ezshader.o
	$(CC) -o $(TARGET) $(TARGET).o ezloader.o ezshader.o $(CFLAGS)

$(TARGET).o: $(TARGET).c ezloader.h ezshader.h
	$(CC) -c $(TARGET).c $(CFLAGS)

ezloader.o: ezloader.c ezloader.h
	$(CC) -c ezloader.c $(CFLAGS)

ezshader.o: ezshader.c ezshader.h
	$(CC) -c ezshader.c $(CFLAGS)

clean:
	$(RM) $(TARGET) *.o *~
