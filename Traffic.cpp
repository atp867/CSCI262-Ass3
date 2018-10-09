/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   atp867, 
*   Traffic.cpp :Driver file
*   Compliation : g++ -std=c++11 Traffic.cpp activity.h
*   Run : ./a.out Vehicles.txt Stats.txt 1
*   1/10/2018
*************************************/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string>
#include "activity.h"

using namespace std;


int readVehicles(std::ifstream&,activityEngine&,int);
int readStats(std::ifstream&,activityEngine&,int);
bool checkConsistency(std::ifstream&, int);

int main(int argc, char * argv[])
{
    if(argc != 4)
    {
        std::cerr << "Invalid number of Arguement Parameters" << std::endl;
        std::cerr << "Ending Life itself" << std::endl;
        exit(0);
    }

    std::cout << "-------------Beginning Program------------" << std::endl;
    std::cout << "--------------Reading in Files------------" << std::endl;
    int numTypeV = 0, numTypeS;
    ifstream fin;
    ifstream ifs;
    fin.open(argv[1]);
    ifs.open(argv[2]);
    if(!checkConsistency(fin,0))
        exit(1);
	if(!checkConsistency(ifs,0))
		exit(1);

    fin >> numTypeV;//First integer of Vehicles.txt
    ifs >> numTypeS;//First integer of Stats.txt   
    if(numTypeS != numTypeV)
    {
        std::cerr << "Given file arguements have a different number of Types!" << std::endl;
        exit(3);
    }

    activityEngine simulation;

    if(readVehicles(fin,simulation, numTypeV) == -1)
	{
		fin.close();
        exit(2);
	}
	fin.close();

    if(readStats(ifs,simulation,numTypeS) == -1)
	{
		fin.close();
        exit(2);
	}
	fin.close();

    simulation.printVehicles();
    simulation.startEngine(atoi(argv[3]));

    return 0;
}

int readVehicles(std::ifstream& fin,activityEngine& simulation,int numVehicles)
{
    char tmp[12];
    int cntr = 0;
    Vehicle temp;
    for(int i = 0 ; i < numVehicles; i++)
    {   //each loop is a new line
        fin.getline(tmp,18,':');
        temp.name = tmp;
        fin >> temp.parking;
        fin.ignore(3,':');
        fin.getline(tmp,18,':');
        temp.rego = tmp;
        fin >> temp.volWeight;
        fin.ignore(3,':');
        fin >> temp.speedWeight;
        fin.ignore(256,'\n');
        simulation.pushVehicles(temp);
        if(!checkConsistency(fin,1))
            return -1;
    }
    if(!checkConsistency(fin,2))
        return -1;
    return numVehicles;
}

int readStats(std::ifstream& fin, activityEngine& simulation,int num )
{
    char tmp[25];
    Stats temp;
    Road tempRoad;
    fin >> tempRoad.length;
    fin >> tempRoad.speedLim;
    fin >> tempRoad.numParking;;

    simulation.pushRoad(tempRoad);


    for(int i = 0; i < num; i++)
    {
        fin.getline(tmp,25,':');
        temp.name = tmp;
        fin >> temp.avg;
        fin.ignore(3,':');
        fin >> temp.stdDev;
        fin.ignore(3,':');
        fin >> temp.speedAvg;
        fin.ignore(3,':');
        fin >> temp.speedStdDev;
        fin.ignore(256,'\n');
        simulation.pushStats(temp);
        if(!checkConsistency(fin,1))
            return -1;
    }
    if(!checkConsistency(fin,2))
        return -1;
    return 1;
}

bool checkConsistency(std::ifstream& fin, int state)
{
    bool check = true;
    char tmp[25];
    switch(state){
        case 0:
            if(!fin.good())
            {   //If File does not exist
                std::cerr << "File does not exist!" << std::endl;
                std::cerr << "Unacceptable! Aborting!" << std::endl; 
                check = false;
            }
            break;
        case 1:
            if(fin.eof())
            {   //If number of vehicles is larger than vehicles given
                std::cerr << "Inconsistency found in file!" << std::endl;
                std::cerr << "Unacceptable! Aborting Life!" << std::endl;
                check = false;
            }
            break;
        case 2:
            if(fin.getline(tmp,256,'\n'))
            {   //If number of vehicles stated at start is less than number of vehicles in file
                std::cerr << " Outside and still Inconsistency found in file!" << std::endl;
                std::cerr << "Unacceptable! Aborting Life!" << std::endl;
                check = false;
            }
            break;
        default:
            break;
    }
    return check;
}
