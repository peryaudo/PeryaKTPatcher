CC = gcc 

CFLAGS = -Wall -DWIN32 -mwindows
LDFLAGS = 

TARGET = PeryaKTPatcher.exe
OBJS = peryaktpatcher.o resource.o

all : $(TARGET)

.SUFFIXES: .o .c

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS)
$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(CFLAGS)
	strip $(TARGET)

clean : 
	rm -rf $(TARGET) *.o *~

test : $(TARGET)
	.\\$(TARGET)

peryaktpatcher.o : peryaktpatcher.c

resource.o : resource.rc
	windres resource.rc -o resource.o

