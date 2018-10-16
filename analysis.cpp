/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   anaylsis.cpp :Analysis file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <Olivia Followes - of767 - 5395707>
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
    std::cout << "\n\n*************Starting Analysis*************" << std::endl;
    //Initialise vector sizes
    days.resize(numDays);
    for(int i = 0; i < numDays;i++)
        days[i].vehicleStats.resize(numVehicles);
    totalStats.resize(days[0].vehicleStats.size()); 

    readLogs();
    std::cout << "Calculating statistics across data" << std::endl;
    totalStatistics(road);
    vehicleStatistics();
    printStatistics();

}

void analysisEngine::readLogs()
{
    char file[12];
    char temp[256];
    for(int i = 0; i < days.size(); i++)
    {
        std::ostringstream oss;
        oss << "day" << i << ".txt";
        strcpy(file,(oss.str()).c_str());
        std::cout << "*********Reading log file " << file << " *********" << std::endl;
        std::ifstream fin;
        fin.open(file);
        while(fin.getline(temp,256,'\n'))
        {
            Instances tmp;
            fin.getline(temp,256,'\n');
            fin.getline(temp,256,'\n');
            tmp.rego = temp;
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
            totalStats[tmp.type].total++;
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
        totalAvgSpeed += totalInstances[i].initSpeed;
    }
    //Mean
    totalAvgSpeed = totalAvgSpeed/totalInstances.size();
    totalAvgVolume = totalInstances.size() / days.size();
    for(int i = 0; i <days.size();i++)
    {
        volStdDev += pow(days[i].vehicleTotal - totalAvgVolume ,2);
    }
    volStdDev = volStdDev / days.size();
    totalStdDevVolume = sqrt(volStdDev);
    for(int i = 0; i <totalInstances.size(); i++)
    {
        temp += pow(totalInstances[i].initSpeed - totalAvgSpeed ,2);
    }
    totalStdDevSpeed = temp/totalInstances.size();
    totalStdDevSpeed = sqrt(totalStdDevSpeed);
}

void analysisEngine::vehicleStatistics()
{
    float *volStdDev = new float[totalStats.size()];
    float *temp = new float[totalStats.size()];
    for(int i =0; i < totalStats.size();i++)
    {//initialise temp variables
        volStdDev[i] = 0;
        temp[i] = 0;
    }
                /********Speed Statistics*********/
    for(int i = 0; i < days.size(); i++)// speed mean
    {
        for(int x = 0; x <days[i].vehicleStats.size(); x++)
        {
            for(int k = 0; k < days[i].vehicleStats[x].instances.size(); k++)
                totalStats[x].averageSpeed += days[i].vehicleStats[x].instances[k].initSpeed;
        }
    }
    for(int i = 0; i < totalStats.size(); i++)
        totalStats[i].averageSpeed = totalStats[i].averageSpeed /totalStats[i].total; 

    for(int i = 0; i < totalStats.size(); i++)// speed Standard Deviation
        temp[i] += pow(totalStats[i].total - totalStats[i].averageSpeed,2);
    for(int i = 0; i < totalStats.size();i++)
    {
        totalStats[i].stdDevSpeed = temp[i]/totalStats[i].total;
        totalStats[i].stdDevSpeed = sqrt(totalStats[i].stdDevSpeed);
    }

                /********Volume Statistics*********/
    for(int i = 0; i < totalStats.size(); i++)// volume mean
        totalStats[i].averageVolume = totalStats[i].total /days.size();
    for(int x = 0; x < days.size(); x++)
    {
        for(int i = 0; i < totalStats.size(); i++)// volume Standard Deviation
        {
            volStdDev[i] += pow(days[x].vehicleStats[i].instances.size() - totalStats[i].averageVolume ,2);
        }
    }
    for(int i = 0; i < totalStats.size();i++)
    {
        totalStats[i].stdDevVolume = volStdDev[i]/days.size();
        totalStats[i].stdDevVolume = sqrt(totalStats[i].stdDevVolume);
    }

}

void analysisEngine::printStatistics()
{
    std::cout << "Printing Calculated Statistics to : analysisLog.txt" << std::endl;
    std::ofstream fout;
    fout.open("analysisLog.txt");
    fout << days.size() << " days **************Statistics**************" << std::endl;
    for(int i = 0; i < totalStats.size();i++)
    {//No annoying : inbetween data :)
        fout << i << ' ' << totalStats[i].averageVolume  << ' '
            << totalStats[i].stdDevVolume << ' '
            << totalStats[i].averageSpeed << ' ' 
            << totalStats[i].stdDevSpeed << std::endl;
    }
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

    std::cout << "**************Speed Breaches**************" << std::endl;
    for(std::map<int,Instances>::iterator it = speedBreaches.begin(); it != speedBreaches.end(); it++)
    {
        std::cout << "---------Day :" << it->first << std::endl;
        std::cout << "Type :" << it->second.type << std::endl;
        std::cout << "Speed :" << it->second.initSpeed << std::endl;
        std::cout << "Rego :" << it->second.rego << std::endl;
        std::cout << "Must have went :" << it->second.speed << std::endl;
        std::cout << "Length :" << it->second.curLocation << std::endl;
        std::cout << "Parking Time :" << it->second.parkingTime << std::endl;
        std::cout << "Total Time :" << it->second.totalTime << std::endl;
        std::cout << "StartTime :" << it->second.startTime << std::endl;
        std::cout << "EndTime :" << it->second.endTime << std::endl;
    }
    fout.close();
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
