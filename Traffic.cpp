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
#include "alert.h"

using namespace std;

//Function Prototypes
int readVehicles(std::ifstream&, activityEngine&, int);
int readStats(std::ifstream&, activityEngine&, int);
double getInput(std::ifstream&, int, string, char);
bool checkConsistency(std::ifstream&, int);
bool checkParams(int, char*[]);

int main(int argc, char * argv[])
{
    srand(time(NULL));
	if(!checkParams(argc, argv))
		exit(1);
	
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
	
	//Input first parameter of each file and handle invalid values
	int numTypeV, numTypeS;
    string exitChoice = "";
	if(!(fin >> numTypeV))
	{
		std::cerr << "Error: Failed input of first parameter in vehicles file" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(1);
	}
    fin.ignore(1);
    
    if(!(ifs >> numTypeS))
	{
		std::cerr << "Error: Failed input of first parameter in stats file" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(1);
	}
    ifs.ignore(1);
    
    //Convert negative to positive
    numTypeV = abs(numTypeV);
    numTypeS = abs(numTypeS);
    
    //Check that vehicles and stats files have same number of vehicle types
    if(numTypeS != numTypeV)
    {
        std::cerr << "Error: Given file arguments have a different number of Types!" << std::endl;
        std::cerr << "Terminating Program..." << std::endl;
        exit(3);
    }

    activityEngine simulation;
    analysisEngine analysis;
    alertEngine alert;
    
    //Read vehicles
    if(readVehicles(fin, simulation, numTypeV) == -1)
	{
		fin.close();
        exit(2);
	}
	fin.close();
	
	//Read stats
    if(readStats(ifs, simulation, numTypeS) == -1)
	{
		ifs.close();
        exit(2);
	}
	ifs.close();
	
    simulation.printVehicles();
        simulation.startEngine(atoi(argv[3]));
        
        analysis.startEngine(atoi(argv[3]),numTypeV,simulation.getRoad(), simulation.getVehicles());
       alert.startEngine(simulation, analysis);


    return 0;
}

//Takes current ifstream, simulation object and the number of vehicles to read, and pushes them to simulation object
int readVehicles(std::ifstream& fin, activityEngine& simulation, int numVehicles)
{
    char tmp[12];
    int cntr = 0;
    Vehicle temp;
    
    //Input each line in file
    for(int i = 0 ; i < numVehicles; i++)
    {   //each loop is a new line
    
    	//Support DOS AND Unix
   		if(fin.peek()=='\n')
    		fin.ignore();
    		
    	//get vehicle type
        fin.getline(tmp, 18, ':');
        temp.name = tmp;
        
        //get parking bool
        if(!(fin.peek()=='0'||fin.peek()=='1'))
        {
        	std::cerr << "Error: Failed input of parking bool at line " << i+2 << std::endl;
			std::cerr << "Terminating Program..." << std::endl;
   			exit(1);
		}
        fin >> temp.parking;
        if(fin.peek()!=':')
        {
        	std::cerr << "Error: Failed input of parking bool at line " << i+2 << std::endl;
			std::cerr << "Terminating Program..." << std::endl;
   			exit(1);
		}
        fin.ignore(3,':');
        
        //get rego pattern
        fin.getline(tmp, 18, ':');
        if(strlen(tmp)==0){
        	std::cerr << "Error: Failed input of rego pattern at line " << i+2 << std::endl;
			std::cerr << "Terminating Program..." << std::endl;
   			exit(1);
		}
		for(int j = 0; j<strlen(tmp); j++)
	    {
	    	if(!(tmp[j]=='L' || tmp[j]=='D'))
	    	{
	    		std::cerr << "Error: Failed input of rego pattern at line " << i+2 << std::endl;
				std::cerr << "Terminating Program..." << std::endl;
	   			exit(1);
			}
		}
        temp.rego = tmp;
        
        //get volume weight
        temp.volWeight = abs(getInput(fin, i, "volume weight", ':'));
        
        //get speed weight
        temp.speedWeight = abs(getInput(fin, i, "speed weight", ':'));
        fin.ignore(256, '\n'); //Chops off everything after last required value
        
        simulation.pushVehicles(temp);
        
        //cout<<temp.name<<":"<<temp.parking<<":"<<temp.rego<<":"<<temp.volWeight<<":"<<temp.speedWeight<<":\n";
        
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
    if(!(fin >> nmbr))
	{
		std::cerr << "Error: Failed input of second parameter in stats file" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(1);
	}
    nmbr = abs(nmbr);
    tempRoad.length = nmbr;
    
    if(!(fin >> nmbr))
	{
		std::cerr << "Error: Failed input of third parameter in stats file" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(1);
	}
    nmbr = abs(nmbr);
    tempRoad.speedLim = nmbr;
    
    if(!(fin >> nmbr))
	{
		std::cerr << "Error: Failed input of fourth parameter in stats file" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        exit(1);
	}
    nmbr = abs(nmbr);
    tempRoad.numParking = nmbr;
    
    simulation.pushRoad(tempRoad);
    
    //Input each line in file
    for(int i = 0; i < num; i++)
    {
    	//Support DOS AND Unix
    	if(fin.peek()=='\n')
    		fin.ignore();
    		
    	//get vehicle type
        fin.getline(tmp, 25, ':');
        temp.name = tmp;
		temp.avg = getInput(fin, i, "volume average", ':');
		temp.stdDev = getInput(fin, i, "volume stdDev", ':');
		temp.speedAvg = getInput(fin, i, "speed average", ':');
		temp.speedStdDev = getInput(fin, i, "speed stdDev", ':');
		fin.ignore(256, '\n'); //Chops off everything after last required value
        
        //No negatives pls
        temp.avg = abs(temp.avg);
        temp.speedAvg = abs(temp.speedAvg);
        temp.speedStdDev = abs(temp.speedStdDev);
        temp.stdDev = abs(temp.stdDev);
        
        simulation.pushStats(temp);
        
        //cout<<temp.name<<":"<<temp.avg<<":"<<temp.stdDev<<":"<<temp.speedAvg<<":"<<temp.speedStdDev<<":\n";
        
        if(!checkConsistency(fin, 1)) //Check if there are not enough stats
            return -1;
    }
    if(!checkConsistency(fin, 2)) //Check if there are too many stats
        return -1;
    return 1;
}

//gets the next number value from file input
double getInput(std::ifstream& fin, int line, string type, char delim)
{
	char tmp[25];
	fin.getline(tmp, 25, delim);
    if(strlen(tmp)==0){
    	std::cerr << "Error: Failed input of " << type << " at line " << line+2 << std::endl;
    	std::cerr << "Empty value" << std::endl;
		std::cerr << "Terminating Program..." << std::endl;
		exit(1);
	}
    for(int j = 0; j<strlen(tmp); j++)
    {
    	if(!(isdigit(tmp[j]) || tmp[j]=='.')||strlen(tmp)==0)
    	{
    		std::cerr << "Error: Failed input of " << type << " at line " << line+2 << std::endl;
			std::cerr << "Terminating Program..." << std::endl;
   			exit(1);
		}
	}
	return strtod(tmp, NULL);
}

//Check program parameters
bool checkParams(int argc, char * argv[])
{
	//Check number of parameters
	if(argc != 4)
    {
        std::cerr << "Error: Invalid number of Arguement Parameters" << std::endl;
        std::cerr << "Terminating Program..." << std::endl;
        return false;
    }
    
    //Check number of days
    int numDays;
    std::istringstream iss(argv[3]);
	if(!(iss >> numDays))
    {
    	std::cerr << "Error: Days parameter is not an integer" << std::endl;
        std::cerr << "Terminating Program..." << std::endl;
        return false;
	}
    if(numDays > 999999)
    {
    	std::cerr << "Error: Too many days! Max number of days is 999,999" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        return false;
	}
	if(numDays <= 0)
    {
    	std::cerr << "Error: There must be at least one day to simulate" << std::endl;
    	std::cerr << "Terminating Program..." << std::endl;
        return false;
	}
	return true;
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
            {   //If specified number of vehicles is more than number of vehicles in file
                std::cerr << "Error: Inconsistency found in file!" << std::endl;
                std::cerr << "Unacceptable! Terminating Program..." << std::endl;
                check = false;
            }
            break;
        case 2:
            if(fin.getline(tmp, 256, '\n'))
            {   //If specified number of vehicles is less than number of vehicles in file
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

