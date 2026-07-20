CC = gcc
CFLAGS = -Wall -g -fopenmp
INCLUDES = -I./DTAIDistanceC/
SOURCES_DYNAMIC = openMPDynamic.c \
                  DTAIDistanceC/dd_dtw.c \
                  DTAIDistanceC/dd_dtw_openmp.c \
                  DTAIDistanceC/dd_ed.c \
                  DTAIDistanceC/dd_globals.c \
                  assets/load_from_csv.c
SOURCES_ORIGINAL = example_original.c \
                   DTAIDistanceC/dd_dtw.c \
                   DTAIDistanceC/dd_dtw_openmp.c \
                   DTAIDistanceC/dd_ed.c \
                   DTAIDistanceC/dd_globals.c
TARGET_DYNAMIC = openmp_dynamic
TARGET_ORIGINAL = example_original

all: $(TARGET_DYNAMIC) $(TARGET_ORIGINAL)

$(TARGET_DYNAMIC): $(SOURCES_DYNAMIC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_DYNAMIC) $(SOURCES_DYNAMIC) -lm

$(TARGET_ORIGINAL): $(SOURCES_ORIGINAL)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_ORIGINAL) $(SOURCES_ORIGINAL) -lm

clean:
	rm -f $(TARGET_DYNAMIC) $(TARGET_ORIGINAL)

.PHONY: all clean
