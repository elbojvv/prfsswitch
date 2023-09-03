TARGET=prfs_switch
CC=gcc
CFLAGS=-Wall 
LIBS=-lpigpio
OBJ=prfs_switch.o

.PHONY: all clean

#all: $(TARGET)

#$(TARGET):

prfs_switch: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o $(TARGET)
#	rm -f $(TARGET)


#all:
#	gcc -Wall -pthread -o prfs_switch prfs_switch.c -lpigpio

