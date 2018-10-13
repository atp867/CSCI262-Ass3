/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   anaylsis.cpp :Analysis file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <name - login - Student #>
*************************************/
#include "analysis.h"

using namespace std;

analysisEngine::analysisEngine()
{
    totalAvgSpeed = 0;
    totalStdDevSpeed = 0;
    totalAvgVolume = 0;
    totalStdDevVolume = 0;
}

void analysisEngine::startEngine(int numDays, int numVehicles)
{
    std::cout << "*************Starting Analysis*************" << std::endl;
    days.resize(numDays);
    for(int i = 0; i < numDays;i++)
        days[i].vehicleStats.resize(numVehicles);
    readLogs();
    for(int i = 0; i < numDays; i++)
        dayStatistics(i);
    totalStatistics();

}

void analysisEngine::readLogs()
{
    std::cout << "******************** READING SHIT ********************" << std::endl;
    char file[12];
    char temp[256];
    for(int i = 0; i < days.size(); i++)
    {
        int statCntr = 0;
        std::ostringstream oss;
        oss << "day" << i << ".txt";
        strcpy(file,(oss.str()).c_str());
        std::ifstream fin;
        fin.open(file);
        while(fin.getline(temp,256,'\n'))
        {
            Instances tmp;
            fin.getline(temp,256,'\n');
            fin.ignore(256,':');
            fin >> tmp.type;
            fin.ignore(256,':');
            fin >> tmp.startTime;
            fin.ignore(256,':');
            fin >> tmp.parkingTime;
            fin.ignore(256,':');
            fin >> tmp.speed;
            fin.ignore(256,':');
            fin >> tmp.curLocation;
            fin.ignore(256,':');
            fin >> tmp.totalTime;
            fin.ignore(256,':');
            fin >> tmp.endTime;
            fin.ignore(256,'\n');
            days[i].vehicleStats[tmp.type].instances.push_back(tmp);
        }
        fin.close();
    }
}

void analysisEngine::dayStatistics(int dayIndex)
{
    float speedMean = 0;
    float speedStdDev = 0;
    for(int i = 0; i < days[dayIndex].vehicleStats.size(); i++)
    {
        for(int x = 0; x < days[dayIndex].vehicleStats[i].instances.size(); x++)
        {
            days[dayIndex].vehicleStats[i].total++;
            speedMean += (days[dayIndex].vehicleStats[i].instances[x].curLocation/days[dayIndex].vehicleStats[i].instances[x].totalTime) *60;

        }
        days[dayIndex].vehicleStats[i].averageSpeed = speedMean;
        for(int x = 0; x < days[dayIndex].vehicleStats[i].instances.size(); x++)
        {
            speedStdDev = speedMean - days[dayIndex].vehicleStats[i].instances[x].speed;
            speedStdDev = pow(speedStdDev,2);
            days[dayIndex].vehicleStats[i].stdDevSpeed += speedStdDev;
            days[dayIndex].vehicleStats[i].stdDevSpeed /= days[dayIndex].vehicleStats[i].total;
            days[dayIndex].vehicleStats[i].stdDevSpeed = sqrt(days[dayIndex].vehicleStats[i].stdDevSpeed);

        }
    }
}

void analysisEngine::totalStatistics()
{

}

void analysisEngine::printInstances()
{
    for(int i = 0; i < days.size();i++)
    {
        std::cout << "*********Day********* " << i << std::endl;
        for(int x = 0; x < days[i].vehicleStats.size(); x++)
        {
            std::cout << "*********Vehicle Type********* " << x << std::endl;
            for(int k = 0; k < days[i].vehicleStats[x].instances.size(); k ++)
            {
                std::cout <<"Type " << days[i].vehicleStats[x].instances[k].type << std::endl;
                std::cout <<"StartTime " << days[i].vehicleStats[x].instances[k].startTime << std::endl;
                std::cout <<"Speed " << days[i].vehicleStats[x].instances[k].speed << std::endl;
                std::cout <<"Location " << days[i].vehicleStats[x].instances[k].curLocation << std::endl;
                std::cout <<"TotalTime " << days[i].vehicleStats[x].instances[k].totalTime << std::endl;
                std::cout <<"EndTime " << days[i].vehicleStats[x].instances[k].endTime << std::endl;
            }
        }
    }
}


