/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   alert.cpp :Alert file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <Olivia Followes - of767 - 5395707>
*************************************/
#include "alert.h"
#include <iomanip>
#include <iostream>

using namespace std;

// Calculate intrusion threshold (2 x sum of weights speed/volume)
void alertEngine::calcThreshold(std::vector<Vehicle> vehicleSim)
{
	double volumeSum, speedSum;
	
	for(int i = 0; i < vehicleSim.size(); i++)
	{
		volumeSum += vehicleSim[i].volWeight;
		speedSum += vehicleSim[i].speedWeight;
	}
	
	volumeThresh = volumeSum * 2;
	speedThresh = speedSum * 2;
	
	cout << "---------- THRESHOLDS CALCULATED ----------" << endl;
	cout << "Volume Threshold: " << volumeThresh << endl;
	cout << "Speed  Threshold: " << speedThresh << endl;
	cout << "-------------------------------------------" << endl;
}

// Reads the analysis file from the analysis engine
void alertEngine::readAnalysisFile(int numVehicles)
{
	ifstream fin;
	string tmp = "";
    char name[20];
	fin.open("analysisLog.txt");
	
	// File reading
	cout << "Reading 'analysisLog.txt'..." << endl;
	//Throw away line
	getline(fin,tmp);
	
	//Get first int of line
	int days;
	fin >> days;
	
    stats s;
    for(int i = 0; i < numVehicles; i++)
    {
		fin.getline(name,30,':');
        s.type = name;
        cout << name << endl;
        fin >> s.averageVolume >> s.stdDevVolume >> s.averageSpeed >> s.stdDevSpeed;
		baselineStats.push_back(s);
    }
	
	getline(fin, tmp);
	getline(fin, tmp);
	getline(fin, tmp,':');
	tmp.substr(tmp.find(":") + 2);
	fin >> baseSpeedAvg;
	//cout << baseSpeedAvg << endl;
	getline(fin, tmp,':');
	tmp.substr(tmp.find(":") + 2);
	fin >> baseSpeedStdDev;
	//cout << baseSpeedStdDev << endl;
	getline(fin, tmp,':');
	tmp.substr(tmp.find(":") + 2);
	fin >> baseVolAvg;
	//cout << baseVolAvg << endl;
	getline(fin, tmp,':');
	tmp.substr(tmp.find(":") + 2);
	fin >> baseVolStdDev;
	//cout << baseVolStdDev << endl;
	
	//Reading each day of data
	day d;
    d.alertStats.resize(numVehicles);
	
	for(int i = 0; i < days; i++)
	{
		//cout << endl << "day " << i << endl;
		
		getline(fin, tmp); //blank line
		getline(fin, tmp); //blank line
		getline(fin, tmp); //blank line
		
		//"total vehicles today: "
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.vehicleTotal;
		//cout << d.vehicleTotal << endl;
		for(int x =0; x < numVehicles; x++)
        {
            getline(fin, tmp);//blank line
            
            getline(fin, tmp,':');
            tmp.substr(tmp.find(":") + 2);
            fin >> d.alertStats[x].totalVehicles >> d.alertStats[x].speedAvg;
        }
		
		baselineDays.push_back(d);
	}
	
	fin.close();
	cout << "Closed file 'analysisLog.txt'..." << endl;
}

// Reads the file requested by the user
void alertEngine::readUserFile(activityEngine& activity)
{
	// Prompt user for a file containing new statistics and a number of days
    char tmp[256];
	cout << "Enter a new file name: ";
	cin >> newFile;
	ifstream fin;
	fin.open(newFile);
	while(!fin) //Checks if file entered is valid and loops until valid file is entered
	{
		cout << "[*]Error: File cannot be opened. Please enter a valid file!" << endl;
		cout << "Enter a new file name: ";
		cin >> newFile;
		fin.open(newFile);
	}
    fin.getline(tmp,256,'\n');

/*
    for(int i = 0; i < vehicleStats.size(); i++)
    {
        fin.getline(tmp, 25, ':');
        activity.vehicleStats[i].name = tmp;
        fin.ignore(3,':');
        fin >> activity.vehicleStats[i].avg;
        fin.ignore(3,':');
        fin >> activity.vehicleStats[i].stdDev;
        fin.ignore(3,':');
        fin >> activity.vehicleStats[i].speedAvg;
        fin.ignore(3,':');
        fin >> activity.vehicleStats[i].speedStdDev;
        fin.ignore(256, '\n'); //Chops off everything after last required value
    }
    */
    fin.close();
}

// Caluclates the anomaly thingos idk
void alertEngine::readCurrDays(int i, int numVehicles)
{
	int type;
	
	day d;
    d.alertStats.resize(numVehicles);
		
		stringstream ss;
		ss << i;
		string file = "day" + ss.str() + ".txt";
		
		ifstream fin;
		fin.open(file);
		if(!fin.good())
        {
            std::cerr << "File does not exist" << std::endl;
            exit(5);
        }
        
        string temp = ""; //stores vehicle type for calculation
        
        while(getline(fin, temp))
        {
			fin.ignore(256,':'); 
			getline(fin, temp);//Vehicle type
	    	getline(fin, temp); //Vehicle Rego    
			fin.ignore(256,':'); //Type (int)
			fin >> type;
			//cout << type << endl;
	        getline(fin, temp); //Start
	    	getline(fin, temp); //ParkingTime
	        getline(fin, temp); //InitialSpeed
	      	getline(fin, temp); //Location
			getline(fin, temp); //Total
			getline(fin, temp); //End
			getline(fin, temp); //Blank (check for eof)
			
            d.alertStats[type].totalVehicles++;
			
			if(!temp.size())
			{
				break;
			}
		}
		
		/*
		cout << endl << "READING STATS" << endl;
		cout << "Bus: " << d.busNum << endl << "Motorbike: " << d.motorbikeNum << endl
		<< "Red car: " << d.carNum << endl << "Elephant: " << d.elephantNum << endl
		<< "Taxi: " << d.taxiNum << endl << "Emergency: " << d.emergencyNum << endl;
		*/
		
		testDays.push_back(d);
		
		fin.close();
}

void alertEngine::calcAnomaly(activityEngine activity, int day)
{
	float volAnomCount = 0;
	float speedAnomCount = 0;
	float volSum;
	float speedSum;
	
	/*
	cout << endl << "ANOMALY CALC STATS" << endl;
	cout << "Bus: " << testDays[day].busNum << endl << "Motorbike: " << testDays[day].motorbikeNum << endl
	<< "Red car: " << testDays[day].carNum << endl << "Elephant: " << testDays[day].elephantNum << endl
	<< "Taxi: " << testDays[day].taxiNum << endl << "Emergency: " << testDays[day].emergencyNum << endl;
	*/
	
	// Volume Calcs
    for(int i = 0; i < vehicleStats.size(); i ++)
    {
        volAnomCount+=formulaCalc(testDays[day].alertStats[i].totalVehicles, baselineStats[day].stdDevVolume, baselineStats[day].averageVolume, 
        vehicleStats[i].volWeight);
    }
	
	// Speed Calcs
    for(int i = 0; i < vehicleStats.size(); i ++)
    {
        speedAnomCount+=formulaCalc(testDays[day].alertStats[i].totalVehicles, baselineStats[day].stdDevSpeed, baselineStats[day].averageSpeed, 
        vehicleStats[i].speedWeight);
    }
	
	// Header Printout
	cout << endl << "-------------------------------------------" << endl;
	cout << "DAY" << setw(15) << "SPEED STATUS" << setw(15) << "VOLUME STATUS" << setw(10) << "WEIGHT" << endl;
	cout << "-------------------------------------------" << endl << endl;
	
	cout << volAnomCount << endl;
	cout << speedAnomCount << endl;
	
}

float alertEngine::formulaCalc(float val, float stdDev, float mean, int weight)
{
	float deduction = val - mean;
	
	bool negative = false;
	if(deduction < 0)
	{
		negative = true;
		deduction = fabs(deduction);
	}
	
	float devDed;
	
	if(stdDev == 0)
	{
		devDed = deduction;
	}
	else
	{
		devDed = (deduction/stdDev);
	}
	
	cout << val << ":" << stdDev << ":" << mean << ":" << weight << endl;
	
	float sum = ( devDed * weight);
	cout << sum << endl;
	return sum;
}

//Start Alert Engine
void alertEngine::startEngine(activityEngine activity, analysisEngine analysis)
{
	bool flag = true;
	srand(time(NULL));
	vehicleStats = activity.getVehicles();
	
	while(flag == true)
	{
		readAnalysisFile(activity.getVehicles().size());
		readUserFile(activity);
		
		cout << "Enter number of days: ";
		cin >> newDays;
		
		for(int i = 0; i < newDays; i++)
		{
			cout << "---------------- DAY " << i << " ----------------" << std::endl;
			activity.genEvents();
        	activity.simDay();
        	activity.printInstances(i);
        	activity.clearInstances();
        	readCurrDays(i, vehicleStats.size());
        	calcAnomaly(activity, i);
		}
		
		/*
		//Continue simulation or close program
		cout << "Would You like to continue simulation? [y/n]: " << endl;
		string input;
		cin >> input;
		if(input == "n")
		{
			flag = false;
		}
		*/
		
		flag = false;
	}
}
