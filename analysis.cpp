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

void analysisEngine::startEngine(int numDays, int numVehicles, Road road)
{
    std::cout << "*************Starting Analysis*************" << std::endl;
    //Initialise vector sizes
    days.resize(numDays);
    for(int i = 0; i < numDays;i++)
        days[i].vehicleStats.resize(numVehicles);

    readLogs();
    totalStatistics(road);
    printStatistics();

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
            fin >> tmp.initSpeed;
            fin.ignore(256,':');
            fin >> tmp.curLocation;
            fin.ignore(256,':');
            fin >> tmp.totalTime;
            fin.ignore(256,':');
            fin >> tmp.endTime;
            fin.ignore(256,'\n');
            days[i].vehicleStats[tmp.type].instances.push_back(tmp);
            totalInstances.push_back(tmp);
            days[i].vehicleTotal++;
        }
        fin.close();
    }
}


void analysisEngine::totalStatistics(Road road)
{
    float volStdDev = 0;
    float temp = 0;
    for(int i = 0; i < days.size(); i++)
    {
        for(int x = 0; x <days[i].vehicleStats.size(); x++)
        {
            for(int k = 0; k < days[i].vehicleStats[x].instances.size(); k++)
            {//Speeding vehicles
                temp = (days[i].vehicleStats[x].instances[k].curLocation/days[i].vehicleStats[x].instances[k].totalTime) *60;
                if(temp > road.speedLim)
                {
                    days[i].vehicleStats[x].instances[k].speed = temp;
                    speedBreaches.insert({i,days[i].vehicleStats[x].instances[k]});
                }
            }
        }
    }
    for(int i = 0; i < totalInstances.size(); i++)
    {
        if(totalInstances[i].curLocation > road.length)
            totalAvgSpeed += totalInstances[i].initSpeed;
    }
    //Mean
    totalAvgSpeed = totalAvgSpeed/totalInstances.size();
    totalAvgVolume = totalInstances.size() / days.size();
    for(int i = 0; i <days.size();i++)
    {
        volStdDev += pow(totalAvgVolume - days[i].vehicleTotal,2);
    }
    volStdDev = volStdDev / days.size();
    totalStdDevVolume = sqrt(volStdDev);
    for(int i = 0; i <totalInstances.size(); i++)
    {
        temp += pow(totalAvgSpeed - totalInstances[i].initSpeed,2);
    }
    totalStdDevSpeed = temp/totalInstances.size();
    totalStdDevSpeed = sqrt(totalStdDevSpeed);
}

void analysisEngine::printStatistics()
{
    std::ofstream fout;
    fout.open("analysisLog.txt");
    fout << "**************Statistics**************" << std::endl;
    fout << "Total Speed average : " << totalAvgSpeed << std::endl;
    fout << "Total Speed Standard Deviation : " << totalStdDevSpeed << std::endl;
    fout << "Total Volume average : " << totalAvgVolume << std::endl;
    fout << "Total Volume Standard Deviation : " << totalStdDevVolume << std::endl << std::endl;
    for(int i = 0; i < days.size(); i++)
    {
        fout << "**************Day " << i << "**************" << std::endl;
        fout << "Total vehicles today : " << days[i].vehicleTotal << std::endl;
        for(int x = 0; x < days[i].vehicleStats.size(); x++)
        {
            fout << "------Type " << x << "------" << std::endl;
            fout << "Total vehicles : " << days[i].vehicleStats[x].instances.size() << std::endl;
        }
        fout << std::endl;
    }

    fout << "**************Speed Breaches**************" << std::endl;
    for(std::map<int,Instances>::iterator it = speedBreaches.begin(); it != speedBreaches.end(); it++)
    {
        fout << "---------Day :" << it->first << std::endl;
        fout << "Type :" << it->second.type << std::endl;
        fout << "Speed :" << it->second.initSpeed << std::endl;
        fout << "StartTime :" << it->second.startTime << std::endl;
        fout << "EndTime :" << it->second.endTime << std::endl;
    }
}

/*
void analysisEngine::dayStatistics(int dayIndex,Road road)
{
    float speedMean = 0;
    float speedStdDev = 0;
    for(int i = 0; i < days[dayIndex].vehicleStats.size(); i++)
    {
        for(int x = 0; x < days[dayIndex].vehicleStats[i].instances.size(); x++)
        {
            if(days[dayIndex].vehicleStats[i].instances[x].curLocation >= road.length)
            {//Only vehicles that reached end of road
                days[dayIndex].vehicleStats[i].total++;
                speedMean += (days[dayIndex].vehicleStats[i].instances[x].curLocation/days[dayIndex].vehicleStats[i].instances[x].totalTime) *60;
            }
        }
        days[dayIndex].vehicleStats[i].averageSpeed = speedMean;
        for(int x = 0; x < days[dayIndex].vehicleStats[i].instances.size(); x++)
        {
            if(days[dayIndex].vehicleStats[i].instances[x].curLocation >= road.length)
            {//Only vehicles that reached end of road
                speedStdDev = speedMean - days[dayIndex].vehicleStats[i].instances[x].initSpeed;
                speedStdDev = pow(speedStdDev,2);
                days[dayIndex].vehicleStats[i].stdDevSpeed += speedStdDev;
                days[dayIndex].vehicleStats[i].stdDevSpeed /= days[dayIndex].vehicleStats[i].total;
                days[dayIndex].vehicleStats[i].stdDevSpeed = sqrt(days[dayIndex].vehicleStats[i].stdDevSpeed);
            }
        }
    }
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
                std::cout <<"Speed " << days[i].vehicleStats[x].instances[k].initSpeed << std::endl;
                std::cout <<"Location " << days[i].vehicleStats[x].instances[k].curLocation << std::endl;
                std::cout <<"TotalTime " << days[i].vehicleStats[x].instances[k].totalTime << std::endl;
                std::cout <<"EndTime " << days[i].vehicleStats[x].instances[k].endTime << std::endl;
            }
        }
    }
}

*/
