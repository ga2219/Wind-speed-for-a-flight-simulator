//************************************************************************************************************************//
//***Author: Gulsim Azirakhmet***//
//***Date created: September 20th, 2021***//
//***Assignment 2: Flight Simulator Wind Speed***//
//***UH-1000 Computer Programming for Engineers, NYUAD***//
//************************************************************************************************************************//

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

void loadConfiguration(double wind[], int intwind[], double storm[], int intstorm[], double burst[], int intburst[]);//Function to read input variables from file
void generateWindSpeed(const double wind[], const int intwind[]);//function to create file with random wind speed
double rand_wind(double a, double b);//function to generate random value
void generateStormData(const double storm[], const int intstorm[], const double wind [], const int intwind[]);// function to create file with random storm data
void BurstData(const double storm[], const int intstorm[], const double wind[], const int intwind [], const double burst[], const int intburst[]);//fucntion to create a file with random burst data
void saveToFile(double wind[], int intwind[], double storm[], int intstorm[], double burst[], int intburst[]);//function to create a file with wind speed, storm and burst data
using namespace std;
int main()
{
	double wind[10000], storm[10000], burst[10000];//input double variables as arrays 
	int intwind[10000], intstorm[10000], intburst[10000] ;//input integer variables as arrays
	loadConfiguration(wind, intwind, storm, intstorm, burst, intburst);//calling loadconfiguration function
	generateWindSpeed(wind, intwind);//calling generateWindSpeed function
	generateStormData(storm, intstorm, wind, intwind);//calling generateStormData function
	BurstData(storm, intstorm, wind, intwind, burst, intburst);//calling BurstData function
	saveToFile(wind, intwind, storm, intstorm, burst, intburst);//calling saveToFile function
	return(0);
}



void loadConfiguration(double wind[], int intwind[], double storm[], int intstorm[], double burst[] , int intburst[])
{
	//opening the simulationConfiguration text file
	ifstream DataFile;
	DataFile.open("simulationConfiguration.txt", ios::in);
	//checking if file opens
	if (DataFile.fail())
	{
		cerr << "Error in opening the file";
		exit(1);
	}
	//reading double and integer inputs
	DataFile >> wind[0] >> wind[1] >> intwind[0] >> intwind[1] >> storm[0] >> storm[1] >> storm[2] >> intstorm[0] >> intstorm[1] >> burst[0] >> burst[1] >> burst[2] >> intburst[0]>> intburst[1];
}
	//wind[0]=average_wind, wind[1]=gust
	//storm[0]=Ps, storm[1]=min_storm_amplitude, storm[2]=max_storm_amplitude
	//burst[0]=Pb, burst[1]=min_microburst_amplitude, burst[2]=max_microburst_amplitude
	//intwind[0]=simulation_duration, intwind[1]= stepsize
	//intstorm[0]=min_storm_duration, intstorm[1]=max_storm_duration
	//intburst[0]=min_burst_duration, intburst[1]=max_burst_duration

	


void generateWindSpeed(const double wind[], const int intwind[])
{
	int time = 0;
	double AV1;
	//opening the WindSpeedData text file
	ofstream outputFile;
	outputFile.open("WindSpeedData.txt", ios::out);
	//checking if file opens
	if (outputFile.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(1);
	}
	//writing table names into the WindSpeedData text file
	outputFile.setf(ios::left);
	outputFile << left << setw(10) << "Time(s)" << setw(10) << "Wind Speed(m/h)"<<endl;
		//loop to generate the wind speed within given time and stepsize
		for (time = 0; time <= (intwind[0]*3600); time=time+intwind[1])
		{

			AV1 = rand_wind((wind[0] - wind[1]), (wind[0] + wind[1]));//random wind speed generation
			cout.setf(ios::fixed);
			outputFile << left << setw(10) << setprecision(5) << time << setw(10) << setprecision(5) << AV1 << endl;//writing time and randomly generated wind speed into the file
		}
}


void generateStormData(const double storm[], const int intstorm[], const double wind[], const int intwind[])
{
	double Rand, AV1, AV2, duration;
	string detection, c, d, e;//letters are blank variables
	//opening the StormData text file for writing
	ofstream outputFile;
	outputFile.open("StormData.txt", ios::out);
	//Checking if file opens
	if (outputFile.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(1);
	}
	//writing table names into the StormData text file
	outputFile.setf(ios::left);
	outputFile << left << setw(20) << "Time(s)" << setw(20) << "Wind speed(m/h)" << setw(20) << "New wind speed(m/h)" << setw(20) << "Storm detection" << endl;
	//opening the WindSpeedData file for reading
	ifstream DataFile;
	DataFile.open("WindSpeedData.txt", ios::in);
	//checking if file opens
	if (DataFile.fail())
	{
		cerr << "Error in opening the file";
		exit(1);
	}
	//reading string values from the file
	DataFile >> c >> d >> e;
	
	for (int i = 0; i <= (intwind[0] * 3600); i = i+intwind[1])//loop for executing outputs considering time and stepsize
	{
		int time=0;
		Rand = rand_wind(0, 1);//generating random value between 0 and 1 for probability
		duration = rand_wind((intstorm[0] * 60), (intstorm[1] * 60));//generating random value between max and min storm duration for duration of the storm
		if (Rand <= storm[0])//condition for the random value of probability to be smaller or equal to the given storm probability
		{
			for (int j = 0; j <= duration; j = j + intwind[1])//loop keep storm happening for some duration
			{
					DataFile >> time >> AV1;//reading time and randomly generated wind speed from the WindSpeedData
					AV2 = AV1 + (rand_wind((storm[1]), (storm[2])));//new wind speed with randomly generated storm value added to it
					detection = "detected";//detects the storm
					outputFile << left << setw(20) << setprecision(5) << time << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) << AV2 << setw(20) << setprecision(5) << detection << endl;//writes time, wind speed, new wind speed, and detection to StormData file
					i = i + intwind[1];
					if (time > (intwind[0] * 3600))//if time value becomes larger than it is expected program breakes the loop
					{
						break;//breaks the loop if condition is true
					}
			}

		}

		if (time > (intwind[0] * 3600))//if time value becomes larger than it is expected program breakes the loop
		{
			break;//breaks the loop if condition is true
		}
			detection = "undetected";//detects no storm
			DataFile >> time >> AV1;//reading time and randomly generated wind speed from the WindSpeedData
			outputFile << left << setw(20) << setprecision(5) << time << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) <<  AV1 << setw(20) << setprecision(5) << detection << endl;//writes time, wind speed, new wind speed, and detection to StormData file
	}

}

void BurstData(const double storm[], const int intstorm[], const double wind[], const int intwind[], const double burst[], const int intburst[])
{
	double AV1, AV2, Rand, Rand1, duration, duration1, MB;
	int time = 0;
	string detection, detection1, y, f, g, h, k, l, m, n;//letters are blank variables
	//opens BurstData file for writing
	ofstream outputFile;
	outputFile.open("BurstData.txt", ios::out);
	//checks if file opens
	if (outputFile.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(1);
	}
	//opens StormData file for reading
	ifstream DataFile;
	DataFile.open("StormData.txt", ios::in);
	//checks if file opens
	if (DataFile.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(-1);
	}
	//writes table names into the BurstData text file
	cout.setf(ios::left);
	outputFile << left << setw(20) << "Time(s)" << setw(20) << "New Wind speed(m/h)" << setw(20) << "Microburst(m/h)" << setw(20) << "Microburst detection" << endl;
	//reads first line of the StormData file
	DataFile >> y >> f >> g >> h >> k >> l >> m >> n;


	for (int i = 0; i <= (intwind[0] * 3600); i = i + intwind[1])//loop for executing outputs considering time and stepsize
	{
		DataFile >> time >> AV1 >> AV2 >> detection;//reading time, randomly generated wind speed, new wind speed, detection from the StormData
		if (detection == "detected")//condition to the storm to be detected
		{
			Rand1 = rand_wind(0, 1);//generation of random number between 0 and 1 for microburst probability
			duration1 = rand_wind((intburst[0] * 60), (intburst[1] * 60));//generation of random number between min and max microburstduration

			if (Rand1 <= burst[0])//condition for the random number to be smaller or equal to burst probability
			{
				
				for (int k = 0; k <= duration1; k = k + intwind[1])//loop for the function to execute while k is smaller or equal to random generated duration1
				{
					DataFile >> time >> AV1 >> AV2 >> detection;//reading time, wind speed, new wind speed, and detection from StormData 
					MB = AV2 + (rand_wind((burst[1]), (burst[2])));//microburst wind speed equals to the addition of storm wind speed and random generated burst amplitude
					detection1 = "detected";//detection1 detects that there is a microburst
					//time, AV1, MB, detection1 are written to the outputFile
					outputFile << left << setw(20) << setprecision(5) << i << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) << MB << setw(20) << setprecision(5) << detection1 << endl;
					i = i + intwind[1];//adds stepsize to time
					if (i > intwind[0] * 3600)//condition for the time to be larger than duration of simulation
					{
						break;//loop breaks 
					}
				}
			
				detection1 = "undetected";//detection1 detects that there is no microburst
				MB = AV2;//microburst equals to the value of wind speed with storm 
				//time, AV1, MB, detection1 are written to the outputFile
				outputFile << left << setw(20) << setprecision(5) << i << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) << MB << setw(20) << setprecision(5) << detection1 << endl;
			}
			else
			{
				detection1 = "undetected";//detection1 detects that there is no microburst
				MB = AV2;//microburst equals to the value of wind speed with storm 
				//time, AV1, MB, detection1 are written to the outputFile
				outputFile << left << setw(20) << setprecision(5) << i << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) << MB << setw(20) << setprecision(5) << detection1 << endl;
			}

		}
		else//if condtition is not true
		{
			MB = AV2;//microburst equals to the value of wind speed with storm 
			detection1 = "undetected";//detection1 detects that there is no microburst
			//time, AV1, MB, detection1 are written to the outputFile
			outputFile << left << setw(20) << setprecision(5) << i << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) << MB << setw(20) << setprecision(5) << detection1 << endl;
		}
	}
}


void saveToFile(double wind[], int intwind[], double storm[], int intstorm[], double burst[], int intburst[])
{
	double Rand, AV1, AV2;
	int time = 0;
	double Rand1, duration, duration1, MB;
	string detection1, y, f, g, h, k, l,m,n;//letters are blank variables
	string detection, c, d, e;//letters are blank variables
	string o, p, q, r, s, t, u;//letters are blank variables
	//opens WindSimulation file for writing
	ofstream outputFile;
	outputFile.open("WindSimulation.txt", ios::out);
	//checks if file opens
	if (outputFile.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(1);
	}
	//opens WindSpeedData file for reading
	ifstream DataFile1;
	DataFile1.open("WindSpeedData.txt", ios::in);
	//checks if file opens
	if (DataFile1.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(-1);
	}
	//opens StormData file for reading
	ifstream DataFile2;
	DataFile2.open("StormData.txt", ios::in);
	//checks if file opens
	if (DataFile2.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(-1);
	}
	//opens BurstData file for reading
	ifstream DataFile3;
	DataFile3.open("BurstData.txt", ios::in);
	//checks if file opens
	if (DataFile3.fail())
	{
		cerr << "Error in opening the file" << endl;
		exit(-1);
	}

	cout.setf(ios::left);
	//writes table names into the WindSimulation text file
	outputFile << left << setw(20) << "Time(s)" << setw(20) << "Wind Speed(m/h)" << setw(20) << "New wind speed(m/h)" << setw(20) << "Storm detection" << setw(20) << "Microburst(m/h)" << setw(20) << "Microburst detection" << endl;
	DataFile1 >> c >> d >> e;//reads string values
	DataFile2 >> y >> f >> g >> h >> k >> l>>m>>n;//reads string values
	DataFile3 >> o >> p >> q >> r >> s >> t >> u;//reads string values

	for (int time = 0; time <= (intwind[0] * 3600); time = time + intwind[1])// loop for executing outputs considering timeand stepsize
	{
		DataFile1 >> time >> AV1;//reads time and wind speed from DataFile1
		DataFile2 >> time >> AV1>>AV2 >> detection;//reads time, wind speed, new wind speed and detection from DataFile2
		DataFile3 >> time>>AV1>> MB >> detection1;//reads time, wind speed, burst wind speed and detection for burst from DataFile3
		//time, AV1, AV2, detection, MB, detection1 are written to the outputFile
		outputFile << left << setw(20) << setprecision(5) << time << setw(20) << setprecision(5) << AV1 << setw(20) << setprecision(5) << AV2 << setw(20) << setprecision(5) << detection << setw(20) << setprecision(5) << MB << setw(20) << setprecision(5) << detection1 << endl;
	}
		
}
		
double rand_wind(double a, double b)
{
	return ((double)rand() / RAND_MAX) * (b - a) + a;//formula to generate random numbers
}
	

