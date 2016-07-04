#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "matrix.hpp"
#include <stdio.h>
#include <cstdlib>
#define DIMENSION 3
#define SIZE DIMENSION*DIMENSION


TEST_CASE( "Basic matrix operations", "[factorial]" ) {
    float TEST_MATRIX[3][3] = {
        {1,2,3},
        {1,2,3},
        {1,3,7},
    };
    float TEST_MATRIX_2[9] = {
        1,2,3,
        1,2,3,
        1,3,7,
    };
    float UNIT[9] = {
        1,0,0,
        0,1,0,
        0,0,1,
    };
    int counter = 0;
    for (int i = 0; i < DIMENSION; ++i) {
        for (int ii = 0; ii < DIMENSION; ++ii) {
            REQUIRE( get(TEST_MATRIX_2,i,ii, DIMENSION) == TEST_MATRIX[i][ii] );
            counter++;
        }
    }

    float matrix[9];
    for (int i = 0; i < DIMENSION; ++i) {
        for (int ii = 0; ii < DIMENSION; ++ii) {
            set(matrix, i, ii, TEST_MATRIX[i][ii], DIMENSION);
        }
    }

    for (int i = 0; i < DIMENSION; ++i) {
        for (int ii = 0; ii < DIMENSION; ++ii) {
            REQUIRE(get(matrix,i,ii, DIMENSION) == TEST_MATRIX[i][ii] );
        }
    }

    float l_matrix[9] = {0};
    float u_matrix[9] = {0};
    float row[4] = {1, 1, 1, 1};

    update_values((float*)l_matrix, (float*)u_matrix, 0, 1, (float*)row, DIMENSION);

    for (int i = 0; i < DIMENSION; ++i) {
        for (int ii = 0; ii < DIMENSION; ++ii) {
            if(i == 1) {
                REQUIRE(get(u_matrix,i,ii, DIMENSION) == 1 );
            } else {
                REQUIRE(get(u_matrix,i,ii, DIMENSION) == 0 );
            }
        }
    }
    REQUIRE(get(l_matrix,1,0, DIMENSION) == 1 );
    float* mul_mat = mul_matrix(TEST_MATRIX_2, UNIT, DIMENSION);
    print_matrix(mul_mat, DIMENSION);
    for (int i = 0; i < DIMENSION; ++i) {
        for (int ii = 0; ii < DIMENSION; ++ii) {
                REQUIRE(get(mul_mat,i,ii, DIMENSION) == get(TEST_MATRIX_2,i,ii, DIMENSION)  );
        }
    }

}

TEST_CASE( "LU", "[lu]" ) {
}
