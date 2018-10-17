/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   Traffic.cpp :Driver file
*   Compliation : g++ -std=c++11 Traffic.cpp activity.h
*   Run : ./a.out Vehicles.txt Stats.txt 1
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <Olivia Followes - of767 - 5395707>
*************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "activity.h"
#include "analysis.h"

using namespace std;

//Function Prototypes
int readVehicles(std::ifstream&, activityEngine&, int);
int readStats(std::ifstream&, activityEngine&, int);
bool checkConsistency(std::ifstream&, int);

int main(int argc, char * argv[])
{
    if(argc != 4)
    {
        std::cerr << "Error: Invalid number of Arguement Parameters" << std::endl;
        std::cerr << "Terminating Program..." << std::endl;
        exit(0);
    }
    
    //cout << argv[3] << endl;
    
    int numDays;
    std::istringstream iss(argv[3]);
    if(!(iss >> numDays))
    {
    	std::cerr << "Error: Days parameter is not an integer" << std::endl;
        std::cerr << "Terminating Program..." << std::endl;
        exit(0);
	}
    if(numDays > 999999)
    {
    	std::cerr << "Error: Too many days! Max number of days is 999,999" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(0);
	}
	if(numDays <= 0)
    {
    	std::cerr << "Error: There must be at least one day to simulate" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(0);
	}
    
    std::cout << "-------------Beginning Program------------" << std::endl;
    std::cout << "--------------Reading in Files------------" << std::endl;
    
    ifstream fin; //Vehicles.txt
    ifstream ifs; //Stats.txt
    fin.open(argv[1]);
    ifs.open(argv[2]);
    
    //Check if files exist
    if(!checkConsistency(fin, 0))
        exit(1);
	if(!checkConsistency(ifs, 0))
		exit(1);
	
	int numTypeV, numTypeS;
    fin >> numTypeV; //First integer of Vehicles.txt
    fin.ignore(1);
    ifs >> numTypeS; //First integer of Stats.txt  
    ifs.ignore(1);
    
    //Convert negative to positive
    numTypeV = abs(numTypeV);
    numTypeS = abs(numTypeS);
    
    if(numTypeS != numTypeV)
    {
        std::cerr << "Error: Given file arguements have a different number of Types!" << std::endl;
        std::cerr << "Terminating Program..." << std::endl;
        exit(3);
    }

    activityEngine simulation;
    analysisEngine analysis;

    if(readVehicles(fin, simulation, numTypeV) == -1)
	{
		fin.close();
        exit(2);
	}
	fin.close();
	
    if(readStats(ifs, simulation, numTypeS) == -1)
	{
		ifs.close();
        exit(2);
	}
	ifs.close();
	
    simulation.printVehicles();
    
    simulation.startEngine(atoi(argv[3]));
    
    analysis.startEngine(atoi(argv[3]),numTypeV,simulation.getRoad(), simulation.getVehicles());
    
    return 0;
}

//Takes current ifstream, simulation object and the number of vehicles to read, and pushes them to simulation object
int readVehicles(std::ifstream& fin, activityEngine& simulation, int numVehicles)
{
    char tmp[12];
    int cntr = 0;
    Vehicle temp;
    
    for(int i = 0 ; i < numVehicles; i++)
    {   //each loop is a new line
        fin.getline(tmp, 18, ':');
        temp.name = tmp;
        fin >> temp.parking;
        fin.ignore(3,':');
        fin.getline(tmp, 18, ':');
        temp.rego = tmp;
        
        float num;
        fin >> num;
        num = abs(num);
        temp.volWeight = num;
        fin.ignore(3, ':');
        
        fin >> num;
        num = abs(num);
        temp.speedWeight = num;
        fin.ignore(256, '\n');
        
        simulation.pushVehicles(temp);
        
        if(!checkConsistency(fin, 1)) //Check if there are not enough vehicles
            return -1;
    }
    if(!checkConsistency(fin, 2)) //Check if there are too many vehicles
        return -1;
    return numVehicles;
}

//Takes current ifstream, simulation object and the number of stats to read, and pushes them to simulation object
int readStats(std::ifstream& fin, activityEngine& simulation, int num )
{
    char tmp[25];
    Stats temp;
    Road tempRoad;
    
    //Assigning a negative to unsigned int does BAD things
    float nmbr;
    fin >> nmbr;
    nmbr= abs(nmbr);
    tempRoad.length = nmbr;
    
    fin >> nmbr;
    nmbr = abs(nmbr);
    tempRoad.speedLim = nmbr;
    
    fin >> nmbr;
    nmbr = abs(nmbr);
    tempRoad.numParking = nmbr;
    
    simulation.pushRoad(tempRoad);

    for(int i = 0; i < num; i++)
    {
        fin.getline(tmp, 25, ':');
        temp.name = tmp;
        fin >> temp.avg;
        fin.ignore(3, ':');
        fin >> temp.stdDev;
        fin.ignore(3, ':');
        fin >> temp.speedAvg;
        fin.ignore(3, ':');
        fin >> temp.speedStdDev;
        fin.ignore(256, '\n');
        
        //No negatives pls
        temp.avg = abs(temp.avg);
        temp.speedAvg = abs(temp.speedAvg);
        temp.speedStdDev = abs(temp.speedStdDev);
        temp.stdDev = abs(temp.stdDev);
        
        simulation.pushStats(temp);
        
        if(!checkConsistency(fin, 1)) //Check if there are not enough stats
            return -1;
    }
    if(!checkConsistency(fin, 2)) //Check if there are too many stats
        return -1;
    return 1;
}

//Takes the ifstream to be tested and the state of input (i.e. what stage input is up to), and runs a corresponding consistency check
bool checkConsistency(std::ifstream& fin, int state)
{
    bool check = true;
    char tmp[25];
    switch(state){
        case 0:
            if(!fin.good())
            {   //If File does not exist
                std::cerr << "Error: File does not exist!" << std::endl;
                std::cerr << "Unacceptable! Terminating Program..." << std::endl; 
                check = false;
            }
            break;
        case 1:
            if(fin.eof())
            {   //If number of vehicles is larger than vehicles given
                std::cerr << "Error: Inconsistency found in file!" << std::endl;
                std::cerr << "Unacceptable! Terminating Program..." << std::endl;
                check = false;
            }
            break;
        case 2:
            if(fin.getline(tmp, 256, '\n'))
            {   //If number of vehicles stated at start is less than number of vehicles in file
                std::cerr << "Error: Outside and still Inconsistency found in file!" << std::endl;
                std::cerr << "Unacceptable! Terminating Program..." << std::endl;
                check = false;
            }
            break;
        default:
            break;
    }
    return check;
}
