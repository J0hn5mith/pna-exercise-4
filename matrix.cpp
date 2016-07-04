#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

void print_row(float* row, int dimension){
    for (int i = 0; i < dimension; ++i) {
        printf("%f ", row[i]);
    }
    printf("\n");
}

void print_matrix(float* matrix, int dimension){
    for (int i = 0; i < dimension; ++i) {
            int offset = i * dimension;
            print_row(matrix + offset, dimension);
    }
}

float* generate_matrix(int dimension){
    std::srand(std::time(0));
    float* matrix = (float*)malloc(dimension*dimension*sizeof(float));
    for (int i = 0; i < dimension*dimension; ++i) {
        matrix[i] = (float)(std::rand()%9 + 1);
    }

    return (float*)matrix;
}

float* generate_unit_matrix(int dimension){
    float* matrix = (float*)malloc(dimension*dimension*sizeof(float));
    int row = -1;
    for (int i = 0; i < dimension*dimension; ++i) {
        int column = i % dimension;
        if(column == 0){
            row++;
        }
        if (column == row){
            matrix[i] = 1;
        } else {
            matrix[i] = 0;
        }
    }
    return (float*)matrix;
}

float set(float* matrix, int row, int column, float value, int dimension){
    matrix[dimension*row + column] = value;
    return value;
}

void set(float* matrix, int row, float* values, int dimension){
    int offset = dimension*row;
    for (int i = 0; i < dimension; ++i) {
        matrix[offset + i] = values[i];
    }
}

float get(float* matrix, int row, int column, int dimension){
    return matrix[dimension*row + column];
}

void update_values(float* l, float* u, int step, int row, float* new_values, int dimension){
    set(u, row, new_values, dimension);
    set(l, row, step, new_values[dimension], dimension);
}

void copy_matrix(float* original, float* copy, int dimension){
    for (int i = 0; i < dimension*dimension; ++i) {
        copy[i] = original[i];
    }
}

//#include <stdlib.h>
//#include <stdio.h>
//#include "matrix.hpp"


//Matrix::Matrix( unsigned int dimension ) {
    //this->dimension = dimension;
    //this->values = (float*)malloc(dimension*dimension*sizeof(float));
    //for (int i = 0; i < dimension; ++i) {
        //this->values[i] = 0;
    //};
//};

//void Matrix::set(unsigned int row, unsigned int column, float value){
    //this->values[this->dimension*row + column] = value;
//};

//void Matrix::set(float* values){
    //for (int i = 0; i < this->size(); ++i) {
        //this->values[i] = values[i];
    //}
//};

//float Matrix::get(unsigned int row, unsigned int column){
    //return this->values[this->dimension*row + column];
//};

//void Matrix::row(unsigned int row_nr, float row_values[]){
    //int offset = row_nr*this->dimension;
    //for (int i = 0; i < this->dimension; ++i) {
        //row_values[i] = this->values[offset + i];
    //}
//}

//void Matrix::column(unsigned int column_nr, float column_values[]){
    //for (int i = 0; i < this->dimension; ++i) {
        //column_values[i] = this->values[this->dimension*i + column_nr];
    //}
//}

//int Matrix::size() {
    //return this->dimension * this->dimension;
//}

//Matrix* Matrix::sub(Matrix* other) {
    //for (int i = 0; i < this->size(); ++i) {
        //this->values[i] = this->values[i] - other->values[i];
    //}
    //return this;
//}

//Matrix* Matrix::mul(Matrix* other) {
    //float* row = (float*)malloc(this->dimension*sizeof(float));
    //float* column = (float*)malloc(this->dimension*sizeof(float));
    //float* tmp_values  = (float*)malloc(dimension*dimension*sizeof(float));

    //int counter = 0;
    //for (int i = 0; i < this->dimension; ++i) {
        //for (int ii = 0; ii < this->dimension; ++ii) {
            //float sum = 0;
            //this->row(i, row);
            //other->column(ii, column);
            //for (int iii = 0; iii < this->dimension; ++iii) {
                //sum += row[iii] * column[iii];
            //}
            //tmp_values[counter] = sum;
            //counter++;
        //}
    //}

    //this->set(tmp_values);

    //delete[] row;
    //delete[] column;
    //delete[] tmp_values;

    //return this;
//}


//void Matrix::lu(Matrix* l, Matrix* u){
    //u->set(this->values);
    //l->setUnit();
    //for (int i = 0; i < this->dimension - 1; i++) {
        //for (int k = i + 1; k < this->dimension; k++) {
            //l->set(k, i, u->get(k,i)/u->get(i,i));
            //for (int j = i; j < this->dimension; j++) {
                //u->set(k,j, u->get(k,j) - l->get(k, i) * u->get(i,j));
            //}
        //}
    //}
//};

//void update_row(Matrix* u, unsigned int row, float value){
    //int world_size;
    //int rank;
    //MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ////Send out request
    ////Collect responses and update values
//}

//void update_row_parallel(Matrix* u, unsigned int row, float value){
    //int world_size;
    //int rank;
    //MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ////Send out request
    
    ////Collect responses and update values

//}

//void Matrix::print(){
    //for (int i = 0; i < this->size(); i++) {
        //if(i % this->dimension == 0){
            //printf("\n");
        //}
        //printf("%f ", this->values[i]);
    //}
    //printf("\n");
//};

//void Matrix::setUnit(){
    //for (int i = 0; i < this->dimension; ++i) {
        //for (int ii = 0; ii < this->dimension; ++ii) {
            //if(i  == ii){
                //this->set(i,ii,1);
            //} else {
                //this->set(i,ii,0);
            //}
        //}
    //}
//};
