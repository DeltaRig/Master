CC = mpicc
CFLAGS = -Wall -g -O3 -fopenmp
INCLUDES = -I./DTAIDistanceC/
SOURCES = mainMPIV3.2Datatype.c \
          DTAIDistanceC/dd_dtw.c \
          DTAIDistanceC/dd_ed.c \
          DTAIDistanceC/dd_globals.c \
          assets/load_from_csv.c
TARGET = mpi_v3

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean
