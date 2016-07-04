float* generate_matrix(int dimension);
float* generate_unit_matrix(int dimension);
float set(float* matrix, int row, int column, float value, int dimension);
void set(float* matrix, int row, float* value, int dimension);
float get(float* matrix, int row, int column, int dimension);
void update_values(float* l, float* u, int step, int row, float* new_values, int dimension);
void print_row(float* row, int dimension);
void print_matrix(float* matrix, int dimension);
void copy_matrix(float* original, float* copy, int dimension);
