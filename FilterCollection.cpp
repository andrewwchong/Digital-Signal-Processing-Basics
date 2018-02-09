// FilterCollection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "time.h"
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>

// #define localtime_s(x, y) localtime_r(y, x)
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4244) // warning C4244: '=' : conversion from 'int' to 'float', possible loss of data
#pragma warning(disable : 4305) //warning C4305 : '=' : truncation from 'double' to 'float'

//pragma = precompiler
using namespace std;
typedef std::vector<float>  float_Vector;

bool processShow = false;

class Filter
{



public:


	static const int DATA_SAMPLE_SIZE = 5;
	float filteredData = 0;
	float prevData = 0, rawData = 0;
	float *pfilteredData;

	float CALL_STACK_X[5], CALL_STACK_Y[5];
	float dataXRef[5], dataYRef[5];


	void pinit()
	{

		pfilteredData = &filteredData;
	}

	void XYZero()
	{


		CALL_STACK_X[1] = 0;
		CALL_STACK_Y[1] = 0;

		CALL_STACK_X[2] = 0;
		CALL_STACK_Y[2] = 0;

		CALL_STACK_X[3] = 0;
		CALL_STACK_Y[3] = 0;

		CALL_STACK_X[4] = 0;
		CALL_STACK_Y[4] = 0;

		CALL_STACK_X[5] = 0;
		CALL_STACK_Y[5] = 0;

	}



	void queueDataX(float data)
	{
		for (int index_Q = DATA_SAMPLE_SIZE; index_Q >= 0; index_Q--) //queue 5 items
		{
			CALL_STACK_X[index_Q + 1] = CALL_STACK_X[index_Q];
		}

		CALL_STACK_X[0] = data;
		if(processShow)
		{
		printf("In the x loop\n");
		}
		outputDataX();
	}


	void outputDataX()
	{

		for (int index = 0; index < DATA_SAMPLE_SIZE; index++) //queue 5 items
		{
		if(processShow)
			{
			printf("Data at %d is %1.2f\n", index, CALL_STACK_X[index]);
			}
		}
		if(processShow)
		{
		printf("Data queue finished\n\n");
		}


	}



	void queueDataY(float data)
	{
		for (int index_Q = DATA_SAMPLE_SIZE; index_Q >= 0; index_Q--) //queue 5 items
		{
			CALL_STACK_Y[index_Q + 1] = CALL_STACK_Y[index_Q];
		}

		CALL_STACK_Y[0] = data;
		if(processShow)
			{
				printf("In the y loop\n");
			}
		outputDataY();
	}

	void outputDataY()
	{

		for (int index = 0; index < DATA_SAMPLE_SIZE; index++) //queue 5 items
		{
			if(processShow)
			{
			printf("Data at %d is %1.2f\n", index, CALL_STACK_Y[index]);
			}
		}

		if(processShow)
		{
		printf("Data queue finished\n\n");
		}

	}


private:

	float sumInput = 0, sumPrevOut = 0;
	int arrayPosition = 0, arrayPositionY = 0;

	float B_k[5];
	float A_k[5];

	float filterFunction(); //do everything insde the filter function



};



class IIRFilterClass :public Filter      //Subclass IIRFilter class of parent class: Filter
{

public:
	float *pfilteredData;


	float filter(void)  // do everything inside the filter function
	{

		return IIRFilter();

	}

	void pinit()   //Reference the filter data with a pointer to allow more flexibility
	{

		pfilteredData = &filteredData;
	}



	void B_kA_kInit()  // Constants used to weight data sample values 
	{

		B_k[1] = 0.1;
		A_k[1] = 0.1;

		B_k[2] = 0.15;
		A_k[2] = 0.15;

		B_k[3] = 0.2;
		A_k[3] = 0.2;

		B_k[4] = 0.25;
		A_k[4] = 0.25;

		B_k[5] = 0.3;
		A_k[5] = 0.3;

	}


	void B_kInit(int BK1, int BK2, int BK3, int BK4, int BK5)   // Reinitialize constants used to weight data feed forward sample values 
	{

		B_k[1] = BK1;
		B_k[2] = BK2;
		B_k[3] = BK3;
		B_k[4] = BK4;
		B_k[5] = BK5;

	}

	void A_kInit(int AK1, int AK2, int AK3, int AK4, int AK5)  // Reinitialize constants used to weight data feed forward sample values 
	{

		A_k[1] = AK1;
		A_k[2] = AK2;
		A_k[3] = AK3;
		A_k[4] = AK4;
		A_k[5] = AK5;

	}


private:

	float sumInput = 0, sumPrevOut = 0;
	int arrayPosition = 0, arrayPositionY = 0;

	float B_k[5];
	float A_k[5];

	float IIRFilter() //do everything insde the filter

	{
		/*
		for (arrayPosition = 1; arrayPosition <= 5; arrayPosition++)    //filter 5 items     OR    GET ALL THE DATA FIRST
		{
			printf("Input Data:");
			scanf_s("%f", &rawData);
			queueDataX(rawData);
		}
		*/

		for (arrayPositionY = 1; arrayPositionY <= 5; arrayPositionY++)    //filter 5 items
		{


			sumInput += float(B_k[arrayPositionY] * (CALL_STACK_X[DATA_SAMPLE_SIZE - arrayPositionY]));         //equation of IIR separated into 2 parts and then
			sumPrevOut += float(A_k[arrayPositionY] * (CALL_STACK_Y[DATA_SAMPLE_SIZE - arrayPositionY]));		// subtracted to make it more readable

			*pfilteredData = sumInput - sumPrevOut;
			
			if(processShow)
			{
			printf("Filtered Data is %f\n\n", *pfilteredData);
			}
			queueDataY(*pfilteredData);


		}

		return *pfilteredData;

	}



};



class ButterworthFilterClass :public Filter      //Subclass IIRFilter class of parent class: Filter
{

public:
	float *pfilteredData;


	float filter(void)  // do everything inside the filter function
	{

		return ButterworthFilter();

	}

	void pinit()   //Reference the filter data with a pointer to allow more flexibility
	{

		pfilteredData = &filteredData;
	}



	void B_kA_kInit()  // Constants used to weight data sample values 
	{

		B_k[1] = 0.1;
		A_k[1] = 0.1;

		B_k[2] = 0.15;
		A_k[2] = 0.15;

		B_k[3] = 0.2;
		A_k[3] = 0.2;

		B_k[4] = 0.25;
		A_k[4] = 0.25;

		B_k[5] = 0.3;
		A_k[5] = 0.3;

	}


	void B_kInit(int BK1, int BK2, int BK3, int BK4, int BK5)   // Reinitialize constants used to weight data feed forward sample values 
	{

		B_k[1] = BK1;
		B_k[2] = BK2;
		B_k[3] = BK3;
		B_k[4] = BK4;
		B_k[5] = BK5;

	}

	void A_kInit(int AK1, int AK2, int AK3, int AK4, int AK5)  // Reinitialize constants used to weight data feed forward sample values 
	{

		A_k[1] = AK1;
		A_k[2] = AK2;
		A_k[3] = AK3;
		A_k[4] = AK4;
		A_k[5] = AK5;

	}


private:

	float sumInput = 0, sumPrevOut = 0;
	int arrayPosition = 0, arrayPositionY = 0;

	float B_k[5];
	float A_k[5];

	float ButterworthFilter() //do everything insde the filter

	{
		/*
		for (arrayPosition = 1; arrayPosition <= 5; arrayPosition++)    //filter 5 items     OR    GET ALL THE DATA FIRST
		{
		printf("Input Data:");
		scanf_s("%f", &rawData);
		queueDataX(rawData);
		}
		*/

		for (arrayPositionY = 1; arrayPositionY <= 5; arrayPositionY++)    //filter 5 items
		{


		//	sumInput += float(B_k[arrayPositionY] * (CALL_STACK_X[DATA_SAMPLE_SIZE - arrayPositionY]));         //equation of IIR separated into 2 parts and then
		//	sumPrevOut += float(A_k[arrayPositionY] * (CALL_STACK_Y[DATA_SAMPLE_SIZE - arrayPositionY]));		// subtracted to make it more readable




			sumInput += float((B_k[arrayPositionY] * CALL_STACK_X[DATA_SAMPLE_SIZE - arrayPositionY]));


			*pfilteredData = (CALL_STACK_X[0] + CALL_STACK_X[4]) - sumInput;
			if(processShow)
			{
			printf("Filtered Data is %f\n\n", *pfilteredData);
			}
			queueDataY(*pfilteredData);


		}

		return *pfilteredData;

	}



};





class FIRFilterClass :public Filter        //Subclass FIRFilter class of parent class: Filter
{

public:
	float *pfilteredData;

	float filter(void)  // do everything inside the filter function
	{

		return FIRFilter();

	}

	void pinit()		//Reference the filter data with a pointer to allow more flexibility
	{

		pfilteredData = &filteredData;
	}



	void B_kA_kInit()  // Constants used to weight data sample values 
	{

		H_k[1] = 0.1;
		H_k[2] = 0.15;
		H_k[3] = 0.2;
		H_k[4] = 0.25;
		H_k[5] = 0.3;

	}


	void H_kInit(int HK1, int HK2, int HK3, int HK4, int HK5)   // Reinitialize constants used to weight data sample values 
	{

		H_k[1] = HK1;
		H_k[2] = HK2;
		H_k[3] = HK3;
		H_k[4] = HK4;
		H_k[5] = HK5;

	}



private:

	float sumInput = 0;
	int arrayPosition = 0, arrayPositionY = 0;

	float H_k[5];


	float FIRFilter() //do everything insde the filter

	{
		/*
		for (arrayPosition = 1; arrayPosition <= 5; arrayPosition++)    //filter 5 items     OR    GET ALL THE DATA FIRST
		{
		printf("Input Data:");
		scanf_s("%f", &rawData);
		queueDataX(rawData);
		}
		*/

		for (arrayPositionY = 1; arrayPositionY <= 5; arrayPositionY++)    //filter 5 items
		{


			sumInput += float(H_k[arrayPositionY] * (CALL_STACK_X[DATA_SAMPLE_SIZE - arrayPositionY]));         //equation of IIR separated into 2 parts and then

			*pfilteredData = sumInput;
			if(processShow)
			{
			printf("Filtered Data is %f\n\n", *pfilteredData);
			}
			queueDataY(*pfilteredData);


		}

		return *pfilteredData;

	}



};



void main(void)
{


	auto start = chrono::system_clock::now();								 //Used to display start and end time of data sampling for later reference
	time_t timestamp = chrono::system_clock::to_time_t(start);

	cout << "Opened On " << ctime(&timestamp) << endl;

	ofstream outputStream;													//Used to write your data into a text file on your computer
	outputStream.open("dataFiltered.txt", ios_base::app);
	cout << "dataFiltered.txt" << endl;

	outputStream << "Opened" << "Filter" << "Data" << endl;
	outputStream << "Opened" << "on" << ctime(&timestamp);



	IIRFilterClass iir;					//Initialize IIRFilterClass
	FIRFilterClass fir;					//Initialize FIRFilterClass
	ButterworthFilterClass butter;			//Initialize ButterworthFilterClass


	iir.XYZero();						//Initialize IIR constants
	iir.B_kA_kInit();
	iir.pinit();

	fir.XYZero();						//Initialize FIR constants
	fir.B_kA_kInit();
	fir.pinit(); 

	butter.XYZero();						//Initialize FIR constants
	butter.B_kA_kInit();
	butter.pinit();

	float rawData = 0.0;
	char filterData[1000];
	int counter = 1;
	static const int FIL_T_LIM = 5;

	while (counter <= FIL_T_LIM)		//Filter Data
	{
		for (int i = 1; i <= 5; i++)    //filter 5 items     OR    GET ALL THE DATA FIRST
		{
			printf("Input Data:");	//Input 5 user generated data samples
			scanf_s("%f", &rawData);
			iir.rawData = rawData;
			fir.rawData = rawData;
			butter.rawData = rawData;


			iir.queueDataX(iir.rawData);
			fir.queueDataX(fir.rawData);
			butter.queueDataX(butter.rawData);

		}

		fir.filteredData = fir.filter();
		iir.filteredData = iir.filter();
		butter.filteredData = butter.filter();



		printf("Filtered Main IIR Data is %f\n\n", iir.filteredData);
		printf("Filtered Main FIR Data is %f\n\n", fir.filteredData);
		printf("Filtered Main Butterworth Data is %f\n\n", butter.filteredData);


		cin >> filterData;
		cin >> filterData;
		cin >> filterData;

		outputStream << "IIR FilterData" << counter << "is" << iir.filteredData << endl;
		outputStream << "FIR FilterData" << counter << "is" << fir.filteredData << endl;
		outputStream << "Butterworth FilterData" << counter << "is" << butter.filteredData << endl;


		counter++;

	}



	outputStream << "Test 1." << endl;


	outputStream.close();

	ifstream inputStream;										    

	inputStream.open("dataFiltered.txt");

	printf("Opening dataFiltered.txt for Viewing\n");

	cout << filterData << endl;									   //PRINT OPENED IIR FILTERED DATA
	cout << filterData << endl;
	cout << filterData << endl;

	cout << filterData << endl;									   //PRINT Opened FIR FILTERED DATA
	cout << filterData << endl;
	cout << filterData << endl;

	cout << filterData << endl;									   //PRINT Opened Butterworth FILTERED DATA
	cout << filterData << endl;
	cout << filterData << endl;


	for (int i = 1; i <= 10; i++)
	{
		inputStream >> filterData;
		cout << filterData << endl;
	}

	auto end = chrono::system_clock::now();						 

	chrono::duration<double> elapsedSeconds = end - start;

	time_t endTime = chrono::system_clock::to_time_t(end);
	cout << "Filter ended on " << ctime(&endTime) << endl;
	cout << "Elapsed Time:" << elapsedSeconds.count() << "Seconds";



	getchar();
	getchar();


}

