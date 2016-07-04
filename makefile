#map <leader>r :!c++ -O3 -o app %; ./app<CR>
PLATFORM=0

CCOMPILER=c++
ifeq ($(PLATFORM), 0)
MPI_CCOMPILER=mpicc
CFLAGS=-c -Wall
else
MPI_CCOMPILER=mpiFCCpx
CFLAGS=-Kfast
endif

LDFLAGS=
COMMON_SOURCES=matrix.cpp
SOURCES=main.cpp $(COMMON_SOURCES)
OBJECTS=$(SOURCES:.cpp=.o)
TEST_SOURCES=test.cpp $(COMMON_SOURCES)
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
EXECUTABLE=app
MPI=`which mpirun`
NUM_THREADS=1

all: compile link run clean

test: run_tests clean

run_tests: $(TEST_OBJECTS)
	$(CCOMPILER) $(LDFLAGS) $(TEST_OBJECTS)  -o tests
	./tests

compile: $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	$(MPI_CCOMPILER) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CCOMPILER) $(CFLAGS) $< -o $@

link: $(EXECUTABLE)

run:
	$(MPI) -n ${NUM_THREADS} ./$(EXECUTABLE)

clean:
	rm *o
