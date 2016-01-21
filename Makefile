CC = arm-linux-gcc
CPP = arm-linux-g++

TARGET = hawk

CFLAGS += -fmessage-length=0 -Wall -D_GNU_SOURCE
LDFLAGS = -L.
LIBS =

SOURCES = $(wildcard ./*.c)
OBJS = $(SOURCES:./%.c=./%.o)

TAGS =  project_vim
RM = rm -rf

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -c $<

clean:
	$(RM) $(OBJS) $(TARGET) $(TAGS)
