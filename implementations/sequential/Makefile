CC = gcc
CFLAGS = -Wall -g -lm
INCLUDES = -I./DTAIDistanceC/
SOURCES = dtwSequential.c \
          DTAIDistanceC/dd_dtw.c \
          DTAIDistanceC/dd_ed.c \
          DTAIDistanceC/dd_globals.c \
          assets/load_from_csv.c
TARGET = dtw_seq

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean
