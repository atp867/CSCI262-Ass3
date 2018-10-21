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
	getline(fin,tmp);
	tmp = tmp.at(0);
	int days = stoi(tmp);
	
    stats s;
    for(int i = 0; i < numVehicles; i++)
    {
		fin.getline(name,30,':');
        s.type = name;
        cout << name << endl;
        fin >> s.averageVolume >> s.stdDevVolume >> s.averageSpeed >> s.stdDevSpeed;
		baselineStats.push_back(s);
    }
    /*
	//Read stats
	stats s;
	for(int i = 0; i < 2; i++) // Read 'Bus' and 'Motorbike'
	{
		fin >> s.type >> s.averageVolume >> s.stdDevVolume >> s.averageSpeed >> s.stdDevSpeed;
		baselineStats.push_back(s);
	}
	
	// Read 'Red Car'
	fin >> s.type;
	fin.get();
	string t;
	fin >> t;
	s.type = s.type + t;
	fin >> s.averageVolume >> s.stdDevVolume >> s.averageSpeed >> s.stdDevSpeed;
	baselineStats.push_back(s);
	
	for(int i = 0; i < 3; i++) // Read 'Elephant', 'Taxi' and 'Emergency'
	{
		fin >> s.type >> s.averageVolume >> s.stdDevVolume >> s.averageSpeed >> s.stdDevSpeed;
		baselineStats.push_back(s);
	}
    */
	
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
		
		getline(fin, tmp);//blank line
		
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.busNum >> d.busSpeedAvg;
		//cout << d.busNum << " " << d.busSpeedAvg << endl;
		
		getline(fin, tmp);//blank line
		
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.motorbikeNum >> d.motorbikeSpeedAvg;
		//cout << d.motorbikeNum << " " << d.motorbikeSpeedAvg << endl;
		
		getline(fin, tmp);//blank line
		
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.carNum >> d.carSpeedAvg;
		//cout << d.carNum << " " << d.carSpeedAvg << endl;
		
		getline(fin, tmp);//blank line
		
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.elephantNum >> d.elephantSpeedAvg;
		//cout << d.elephantNum << " " << d.elephantSpeedAvg << endl;
		
		getline(fin, tmp);//blank line
		
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.taxiNum >> d.taxiSpeedAvg;
		//cout << d.taxiNum << " " << d.taxiSpeedAvg << endl;
		
		getline(fin, tmp);//blank line
		
		getline(fin, tmp,':');
		tmp.substr(tmp.find(":") + 2);
		fin >> d.emergencyNum >> d.emergencySpeedAvg;
		//cout << d.emergencyNum << " " << d.emergencySpeedAvg << endl;
		
		baselineDays.push_back(d);
	}
	
	fin.close();
	cout << "Closed file 'analysisLog.txt'..." << endl;
}

// Reads the file requested by the user
void alertEngine::readUserFile(activityEngine& activity)
{
	// Prompt user for a file containing new statistics and a number of days
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
	fin.close();
}

// Caluclates the anomaly thingos idk
void alertEngine::calcAnomalyCount(activityEngine activity)
{
	// Header Printout
	cout << endl << "-------------------------------------------" << endl;
	cout << "DAY" << setw(15) << "SPEED STATUS" << setw(15) << "VOLUME STATUS" << setw(10) << "WEIGHT" << endl;
	cout << "-------------------------------------------" << endl;
	
	
}

//Start Alert Engine
void alertEngine::startEngine(activityEngine& simulation)
{
	bool flag = true;
	
	while(flag == true)
	{
		readAnalysisFile(simulation.getVehicles().size());
		readUserFile(simulation);
		
		cout << "Enter a number of days: ";
		cin >> newDays;
		
		
		for(int i = 0; i < newDays; i++)
		{
			/*
			cout << "---------------- DAY " << i << " ----------------" << std::endl;
			activity.genEvents();
        	activity.simDay();
        	activity.printInstances(i);
        	activity.clearInstances();
        	*/
        	activityEngine activity;
        	activity.startEngine(1);
			calcAnomalyCount(activity);
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
