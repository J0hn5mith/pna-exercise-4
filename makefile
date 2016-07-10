#map <leader>r :!c++ -O3 -o app %; ./app<CR>
OS := $(shell uname)
ifeq ($(OS), LINUX)
PLATFORM=1
else
PLATFORM=0
endif


ifeq ($(PLATFORM), 0)
CCOMPILER=gcc
MPI_CCOMPILER=mpicc
CFLAGS=-c -Wall
else
CCOMPILER=mpiFCCpx
MPI_CCOMPILER=mpiFCCpx
CFLAGS=-Kfast
endif

LDFLAGS=
COMMON_SOURCES=matrix.c utils.c
SOURCES=main.c $(COMMON_SOURCES)
OBJECTS=$(SOURCES:.c=.o)
TEST_SOURCES=test.cpp $(COMMON_SOURCES)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
EXECUTABLE=app
MPI_RUN=`which mpirun`
MPI_RUN_FLAGS=
NUM_THREADS=4

all: compile link run clean

test: run_tests clean

run_tests: $(TEST_OBJECTS)
	$(CCOMPILER) $(LDFLAGS) $(TEST_OBJECTS)  -o tests
	./tests

ifeq ($(PLATFORM), 0)
compile: $(SOURCES)
else
compile:
	$(CCOMPILER) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)
endif

$(EXECUTABLE): $(OBJECTS)
ifeq ($(PLATFORM), 0)
	$(MPI_CCOMPILER) $(LDFLAGS) $(OBJECTS) -o $@
else
	$(MPI_CCOMPILER) $(LDFLAGS) $(OBJECTS)
endif

.c.o:
	$(CCOMPILER) $(CFLAGS) $< -o $@

link: $(EXECUTABLE)

run:
ifeq ($(PLATFORM), 0)
	$(MPI_RUN) $(MPI_RUN_FLAGS) -n ${NUM_THREADS} ./$(EXECUTABLE)
else
	pjsub job.zsh
endif

clean:
	rm -f *o
	rm -f job\.zsh\.o*
