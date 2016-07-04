class Matrix {

    public:
        float* values;
        int dimension;

        void set(unsigned int, unsigned int, float);
        void set(float*);
        float get(unsigned int, unsigned int);
        void row(unsigned int, float[]);
        void column(unsigned int, float[]);

        int size();

        Matrix(unsigned int dimension);

        Matrix* sub(Matrix* other);
        Matrix* mul(Matrix* other);
        void lu(Matrix*, Matrix*);

        void print();
        void setUnit();

};
