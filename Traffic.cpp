/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   atp867, 
*   Traffic.cpp :Driver file
*   Compliation : g++ Traffic.cpp activity.h
*   Run : ./a.out Vehicles.txt Stats.txt 1
*   1/10/2018
*************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "activity.h"

using namespace std;


int readVehicles(std::ifstream&,Vehicle*,int);
int readStats(std::ifstream&,Stats*,Road&,int);
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

    Vehicle vehicleSim[numTypeV];
    Stats vehicleStats[numTypeV];
    Road road;

    if(readVehicles(fin,vehicleSim, numTypeV) == -1)
        exit(2);

    if(readStats(ifs,vehicleStats,road,numTypeS) == -1)
        exit(2);
    for(int i = 0; i < numTypeS; i++)
    {
        std::cout << "----------Vehicle-------- " <<std::endl;
        std::cout << "Name : " << vehicleSim[i].name << std::endl;
        std::cout << "Parking : " << vehicleSim[i].parking << std::endl;
        std::cout << "Rego : " << vehicleSim[i].rego << std::endl;
        std::cout << "VolWeight : " << vehicleSim[i].volWeight << std::endl;
        std::cout << "SpeedWeight : " << vehicleSim[i].speedWeight << std::endl;
        std::cout << "----------Stats-----------" << std::endl;
        std::cout << "Name : " << vehicleStats[i].name <<std::endl;
        std::cout << "Average : " << vehicleStats[i].avg <<std::endl;
        std::cout << "StdDev : " << vehicleStats[i].stdDev <<std::endl;
        std::cout << "SpeedAverage : " << vehicleStats[i].speedAvg <<std::endl;
        std::cout << "SpeedStdDev : " << vehicleStats[i].speedStdDev <<std::endl;
    }

    return 0;
}

int readVehicles(std::ifstream& fin,Vehicle* vehicleSim,int numVehicles)
{
    char tmp[12];
    int cntr = 0;
    for(int i = 0 ; i < numVehicles; i++)
    {   //each loop is a new line
        fin.getline(tmp,18,':');
        vehicleSim[i].name = tmp;
        fin >> vehicleSim[i].parking;
        fin.ignore(3,':');
        fin.getline(tmp,18,':');
        vehicleSim[i].rego = tmp;
        fin >> vehicleSim[i].volWeight;
        fin.ignore(3,':');
        fin >> vehicleSim[i].speedWeight;
        fin.ignore(256,'\n');
        if(!checkConsistency(fin,1))
        {
            fin.close();
            return -1;
        }
    }
    if(!checkConsistency(fin,2))
    {
        fin.close();
        return -1;
    }
    fin.close();
    return numVehicles;
}

int readStats(std::ifstream& fin, Stats* stats,Road& road,int num)
{
    char tmp[25];
    fin >> road.length;
    fin >> road.speedLim;
    fin >> road.numParking;;

    for(int i = 0; i < num; i++)
    {
        fin.getline(tmp,25,':');
        stats[i].name = tmp;
        fin >> stats[i].avg;
        fin.ignore(3,':');
        fin >> stats[i].stdDev;
        fin.ignore(3,':');
        fin >> stats[i].speedAvg;
        fin.ignore(3,':');
        fin >> stats[i].speedStdDev;
        fin.ignore(256,'\n');
        if(!checkConsistency(fin,1))
        {
            fin.close();
            return -1;
        }
    }
    if(!checkConsistency(fin,2))
    {
        fin.close();
        return -1;
    }
    fin.close();
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
