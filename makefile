#map <leader>r :!c++ -O3 -o app %; ./app<CR>
PLATFORM=0

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
COMMON_SOURCES=matrix.c
SOURCES=main.c $(COMMON_SOURCES)
OBJECTS=$(SOURCES:.c=.o)
TEST_SOURCES=test.cpp $(COMMON_SOURCES)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
EXECUTABLE=app
MPI=`which mpirun`
NUM_THREADS=1

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
	$(MPI) -n ${NUM_THREADS} ./$(EXECUTABLE)
else
	pjsub job.zsh
endif

clean:
	rm *o
