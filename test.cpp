#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "matrix.hpp"
#include <stdio.h>
#include <cstdlib>
#define MATRIX_DIMENSION 10


TEST_CASE( "Basic matrix operations", "[factorial]" ) {
    float TEST_MATRIX[3][3] = {
        {1,2,3},
        {1,2,3},
        {1,3,7},
    };
    float TEST_MATRIX_2[3][3] = {
        {1,0,0},
        {0,1,0},
        {0,0,1},
    };
    Matrix* m = new Matrix(3);
    Matrix* m2 = new Matrix(3);
    m->set((float*)TEST_MATRIX);
    m2->set((float*)TEST_MATRIX);

    for (int i = 0; i < m->size(); ++i) {
        REQUIRE( m->values[i] == ((float*)TEST_MATRIX)[i]);
    }

    m->sub(m2);
    for (int i = 0; i < m->dimension * m->dimension; ++i) {
        REQUIRE( m->values[i] == 0 );
    }

    m->set((float*)TEST_MATRIX);
    m2->set((float*)TEST_MATRIX_2);
    m->mul(m2);

    for (int i = 0; i < m->size(); ++i) {
        REQUIRE( m->values[i] == ((float*)TEST_MATRIX)[i] );
    }

}

TEST_CASE( "LU decomposition", "" ) {
    std::srand(std::time(0));
    float TEST_MATRIX[MATRIX_DIMENSION][MATRIX_DIMENSION] = {0};
    for (int i = 0; i < MATRIX_DIMENSION; ++i) {
        for (int ii = 0; ii < MATRIX_DIMENSION; ++ii) {
            TEST_MATRIX[i][ii] = (float)(std::rand()%20);
        }
    }

    Matrix* a = new Matrix(3);
    a->set((float*)TEST_MATRIX);

    Matrix* l = new Matrix(3);
    Matrix* u = new Matrix(3);

    a->lu(l,u);
    l->mul(u);

    for (int i = 0; i < l->size(); ++i) {
        REQUIRE( l->values[i] == a->values[i] );
    }

    delete[] a->values;
}
