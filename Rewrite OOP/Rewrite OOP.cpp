#include "stdafx.h"
#include <sstream> //Allows input on output of strings.
#include <iostream> //Allow to you write to the console.
#include <fstream> //Allows input/output stream class to work on files.
#include <istream> //Can read on interpret input from sequences of characters.
#include <cmath> //Declares a common set of mathematical functions.
#include "Matrix.h"
#include "BinaryImage.h"
using namespace std;

// Reads .txt file representing an image of R rows and C Columns stored in filename 
// and converts it to a 1D array of doubles of size R*C
// Memory allocation is performed inside readTXT
double* readTXT(char *fileName, int sizeR, int sizeC);

// Converts a 1D array of doubles of size R*C to .pgm image of R rows and C Columns 
// and stores .pgm in filename
// Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q);

int main()
{
	
	//ComparisionValue is the value to check the result of the NC or SSD against
	//startComparisionValue is the starting value of ComparisionValue
	double ComparisionValue = 500, startTestValue = 500, scoreSSD;

	std::cout << "CMP2090M" << endl;
	std::cout << "Object Oriented Programming" << endl;
	std::cout << "STO13457465" << endl;
	
	//.pgm image is stored in inputFileName, the .pgm is created to the outputFileName
	char* outputFileName;
	char* inputFileName;
	//input_data is the data from the file
	double* input_data = 0;

	//rowCount is the amount of rows
	//columnCount is the amount of columns
	//imageFormat is the maximum number for writing the files, 1 = Black and White image, 255 = Grayscale image
	//threshold is the number that starts the white values in the binary image
	//blockRowCount is the number of rows in a block
	//blockColumnCount is the number of columns in a block
	//count is the amount of loops the program does
	int rowCount = 512, columnCount = 512, imageFormat = 1, threshold = 150, blockRowCount = 32, blockColumnCount = 32, count = 0;

	inputFileName = "unshuffled_logo_noisy.txt";
	input_data = readTXT(inputFileName, rowCount, columnCount);

	//Create the source logo(as a binary image) and remove the noise from the logo
	BinaryImage<double> source(rowCount, columnCount, input_data, threshold);

	inputFileName = "shuffled_logo.txt";
	input_data = readTXT(inputFileName, rowCount, columnCount);

	//Create the shuffled logo(as a binary image)
	BinaryImage<double> other(rowCount, columnCount, input_data, threshold);

	cout << "Restoring the shuffled logo and removing the noise" << endl;
	cout << "Please Wait..." << endl;

	//currentSourceBlock is the current block from the unshuffled image with noise
	//currentOtherBlock is the current block from the shuffled image
	//outputBlock is the block to be added back to the new .PGM
	BinaryImage<double> currentSourceBlock;
	BinaryImage<double> currentOtherBlock;
	BinaryImage<double> outputBlock(blockRowCount, blockColumnCount, 0);

	//Loop source blocks
	for (int i = 0; i < rowCount; i += blockRowCount)
		for (int j = 0; j < columnCount; j += blockColumnCount)
		{
			//Get the current source block
			currentSourceBlock = source.GetBlock(i, i + blockRowCount - 1, j, j + blockColumnCount - 1);

			//Reset the testValue
			ComparisionValue = startTestValue;

			//Loop other blocks
			for (int m = 0; m < rowCount; m += blockRowCount) //will add 32 each time when the loop completes
			{
				for (int n = 0; n < columnCount; n += blockColumnCount) //will add 32 each time when the loop completes
				{
					//Get the current shuffled block
					currentOtherBlock = other.GetBlock(m, m + blockRowCount - 1, n, n + blockColumnCount - 1);

					//Get its SSD score
					scoreSSD = currentOtherBlock.SSD(currentSourceBlock);

					count++;
					

					//Check if more similar than the previous selected block
					if (ComparisionValue > scoreSSD)
					{
						//Update ComparisionValue and set the new block
						ComparisionValue = scoreSSD;
						outputBlock = currentOtherBlock;
						//If the ComparisionValue is 0, the block is a match break
						if (ComparisionValue == 0) break;
					}
			
				}
				//If the ComparisionValue is 0, the block is a match break
				if (ComparisionValue == 0) break;
				
			}

			//Add block to output matrix
			for (int s = i, x = 0; s < i + blockRowCount; s++, x++)
			{
				for (int t = j, y = 0; t < j + blockColumnCount; t++, y++)
				{
					source.Set(s, t, outputBlock(x, y));
				}
			}
			//Clear the outputBlock
			outputBlock.SetBlank();
		}
	//Create the output image
	outputFileName = "Unshuffled_Image.pgm";
	WritePGM(outputFileName, source.GetData(), rowCount, columnCount, imageFormat);

	cout << "Logo has now been unshuffled and the noise has been reduced." << endl << "Number of loops: " << count << endl << endl;
	cout << "The file is called: " << outputFileName << endl << endl;
	//Free the memory used by NEW
	source.~BinaryImage();
	other.~BinaryImage();
	currentSourceBlock.~BinaryImage();
	currentOtherBlock.~BinaryImage();
	outputBlock.~BinaryImage();

	cout << "The allocated memory has been deleted." << endl << endl;



	return 0;
}

// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(char *fileName, int sizeR, int sizeC)
{
	double* data = new double[sizeR*sizeC];
	int i = 0;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{

		while (myfile.good())
		{
			if (i>sizeR*sizeC - 1) break;
			myfile >> *(data + i);
			// cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out. 
			i++;
		}
		myfile.close();
	}

	else cout << "Unable to open file";
	//cout << i;

	return data;
}

// convert data from double to .pgm stored in filename
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q)
{

	int i;
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	// convert the integer values to unsigned char

	for (i = 0; i<sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << sizeC << " " << sizeR << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << filename << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;

}