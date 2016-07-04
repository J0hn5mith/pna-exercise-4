#map <leader>r :!c++ -O3 -o app %; ./app<CR>
CCOMPILER=c++
MPI_CCOMPILER=mpicc
CFLAGS=-c -Wall
LDFLAGS=
COMMON_SOURCES=matrix.cpp
SOURCES=main.cpp $(COMMON_SOURCES)
OBJECTS=$(SOURCES:.cpp=.o)
TEST_SOURCES=test.cpp $(COMMON_SOURCES)
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
EXECUTABLE=app
MPI=`which mpirun`

all: compile link run clean
	echo "End all"

test: run_tests clean
	echo "End Tests"

run_tests: $(TEST_OBJECTS)
	$(CCOMPILER) $(LDFLAGS) $(TEST_OBJECTS)  -o tests
	./tests

compile: $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	mpicc $(LDFLAGS) $(OBJECTS) -o $@
	#$(MPI_CCOMPILER) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CCOMPILER) $(CFLAGS) $< -o $@

link: $(EXECUTABLE)

run:
	./$(EXECUTABLE)

clean:
	rm *o


