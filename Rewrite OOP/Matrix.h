#include<iostream>
using namespace std;

#ifndef _MATRIX
#define _MATRIX

template<class T>
class Matrix
{
public:
	//Constructors
	Matrix() 
	{

	}
	Matrix(int r, int c, T val)
	{
		//Set and allocate data for the new matrix
		MM = r;
		NN = c;
		data = new T[MM*NN];

		//Fill the matrix with a single value
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, val);
	}

	Matrix(int r, int c, T* input_data)
	{
		//Set and allocate data for the new matrix
		MM = r;
		NN = c;
		data = new double[MM*NN];

		int index = 0;

		//Loop through the input array and allocate it to the matrix
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
			{
				Set(i, j, input_data[index]);
				index++;
			}
	}

	Matrix(const Matrix<double>& other)
	{
		//Set and allocate data for the new matrix
		MM = other.MM;
		NN = other.NN;
		data = new T[MM*NN];

		//Loop through the data in the other matrix and add it to the new matrix
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, other.Get(i, j));
	}

	//Operator overloads
	Matrix<T> operator+(const Matrix<double>& other)
	{
		//Add the elements together, element by element
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, Get(i, j) + other(i, j));
		return *this;
	}
	Matrix<T> operator-(const Matrix<double>& other)
	{
		//Subtract the elements together, element by element
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, Get(i, j) - other(i, j));
		return *this;
	}
	Matrix<T> operator*(const Matrix<double>& other)
	{
		//Multiply the elements together, element by element
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, Get(i, j) * other(i, j));
		return *this;
	}
	Matrix<T> operator/(const Matrix<double>& other)
	{
		//Divide the elements, element by element
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, Get(i, j) / other(i, j));
		return *this;
	}
	Matrix<T> operator=(const Matrix<double>& other)
	{
		//Set and allocate data for the new matrix
		MM = other.MM;
		NN = other.NN;
		delete[] data;
		data = new T[MM*NN];

		//Loop through and set the data to match the other matrix
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, other.Get(i, j));
		return *this;
	}
	Matrix<T> operator++()
	{
		//Increment the value of every element
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, Get(i, j) + 1);
		return *this;
	}
	Matrix<T> operator--()
	{
		//Decrement the value of every element
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, Get(i, j) - 1);
		return *this;
	}
	double operator()(int i, int j)
	{
		return data[i*NN + j];
	}
	//Sets and Gets
	double Get(int i, int j) const
	{
		return data[i*NN + j];
	}

	void Set(int i, int j, double val)
	{
		data[i*NN + j] = val;
	}
	Matrix<T> GetBlock(int iS, int iE, int jS, int jE)
	{
		//Set and allocate data for the new matrix
		Matrix<T> mat;
		mat.MM = iE - iS + 1;
		mat.NN = jE - jS + 1;
		mat.data = new T[mat.MM * mat.NN];

		//Set the data to the data from the block defined by iS, iE, jS, jE
		for (int i = 0; i < mat.MM; i++)
			for (int j = 0; j < mat.NN; j++)
				mat.Set(i, j, Get(i + iS, j + jS));
		return mat;
	}
	double* GetData()
	{
		return data;
	}

	

	//Deconstructor
	~Matrix()
	{
		delete[] data;
		data = 0;
	}

protected:
	int MM, NN;
	T* data;
};
#endif