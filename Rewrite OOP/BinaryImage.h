#include <iostream>
#include "Matrix.h"
using namespace std;

#ifndef _BINIMAGE
#define _BINIMAGE

template<class T>
class BinaryImage : public Matrix<double>
{
public:
	BinaryImage()
	{

	}

	BinaryImage(int M, int N, T val)
	{
	//set and allocate data for this matrix
		MM = M;
		NN = N;
		data = new double[MM*NN];

		for (int i = 0; i < MM; i++)
		for (int j = 0; j < NN; j++)
			Set(i, j, val);
	}
	BinaryImage(int M, int N, T* input_data, double threshold)
	{
		MM = M;
		NN = N;
		data = new double[MM*NN];

		//Loop throught input array and allocate it to the matrix
		//set the data to be 0 or 1(black or white) according to the threshold
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				if (input_data[i*N + j] > threshold)
					data[i*NN + j] = 1;
				else
					data[i*NN + j] = 0;
	}

	BinaryImage(const BinaryImage<double>& other)
	{
		//Set and allocate data for the new (other)matrix
		MM = other.MM;
		NN = other.NN;
		data = new double[MM*NN];

		//Loop through the data in the other matrix and add it to the new (other) matrix
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, other.Get(i, j));
	}
	//Operator Overloads
	BinaryImage<T> operator=(const BinaryImage<double>& bin)
	{
		//Set and allocate data for the new matrix
		MM = bin.MM;
		NN = bin.NN;
		delete[] data;
		data = new T[MM*NN];

		//Loop through and set the data to match the other matrix
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, bin.Get(i, j));
		return *this;
	}
	BinaryImage<T> GetBlock(int iS, int iE, int jS, int jE)
	{
		BinaryImage mat;
		mat.MM = iE - iS + 1;
		mat.NN = jE - jS + 1;
		mat.data = new double[mat.MM*mat.NN];

		//set the data to the data from the block defined by iStart, iEnd, jStart, jEnd
		for (int i = 0; i < mat.MM; i++)
			for (int j = 0; j < mat.NN; j++)
				mat.Set(i, j, Get(i + iS, j + jS));
		return mat;

	}
	void SetBlank()
	{
		//set the data to 0
		for (int i = 0; i < MM; i++)
			for (int j = 0; j < NN; j++)
				Set(i, j, 0);
	}

	double SSD(const BinaryImage<double>& T)
	{
		double sum = 0;
		for (int i = 0; i < 32; i++)
			for (int j = 0; j < 32; j++)
			{
				double temp = Get(i, j) - T.Get(i, j);
				sum += temp*temp;
			}
		return sum;
	}

	//Deconstructor
	~BinaryImage()
	{
		delete[] data;
		data = 0;
	}
};
#endif
