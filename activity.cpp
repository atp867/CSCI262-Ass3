#include "activity.h"
#include <iostream>

void activityEngine::pushVehicles(Vehicle sample)
{
    vehicleSim.push_back(sample);
}

void activityEngine::pushStats(Stats sample)
{
    vehicleStats.push_back(sample);
}

void activityEngine::pushRoad(Road sample)
{   
    road.length = sample.length;
    road.speedLim = sample.speedLim;
    road.numParking = sample.numParking;
}

void activityEngine::genEvents()
{
    srand(time(NULL));
    std::default_random_engine randEng;
    int i = 0;
    //Iterate through all vehicle types
    for(std::vector<Vehicle>::iterator it = vehicleSim.begin(); it != vehicleSim.end(); it++)
    {
         //Use normal distribution to determine number of vehicles
         std::normal_distribution<float> normal(vehicleStats[i].avg,vehicleStats[i].stdDev);
        int loop = lround(normal(randEng));    
        for(int x = 0; x < loop; x++)
        {
            //Other variable intialised in constructor
            Instances temp;
            temp.type = it->name;
            temp.startTime = (rand() % MINUTESINDAY);
            std::normal_distribution<float> normalSpeed(vehicleStats[i].speedAvg,vehicleStats[i].speedStdDev);
            temp.speed = lround(normalSpeed(randEng));
            instances.push_back(temp);
            
        }
        i++;
    }
}


void activityEngine::printVehicles()
{
    std::vector<Stats>::iterator iterateStats = vehicleStats.begin();
    for(std::vector<Vehicle>::iterator it = vehicleSim.begin(); it != vehicleSim.end(); it++)
    {   //each loop is a new line
        std::cout << "----------Vehicle-------- " <<std::endl;
        std::cout << "Name : " << it->name << std::endl;
        std::cout << "Parking : " << it->parking << std::endl;
        std::cout << "Rego : " << it->rego << std::endl;
        std::cout << "VolWeight : " << it->volWeight << std::endl;
        std::cout << "SpeedWeight : " << it->speedWeight << std::endl;
        std::cout << "----------Stats-----------" << std::endl;
        std::cout << "Name : " << iterateStats->name <<std::endl;
        std::cout << "Average : " << iterateStats->avg <<std::endl;
        std::cout << "StdDev : " << iterateStats->stdDev <<std::endl;
        std::cout << "SpeedAverage : " << iterateStats->speedAvg <<std::endl;
        std::cout << "SpeedStdDev : " << iterateStats->speedStdDev <<std::endl;
        iterateStats++;
    }
}

void activityEngine::printInstances()
{
    
    for(std::vector<Instances>::iterator it = instances.begin(); it != instances.end(); it++)
    {
        std::cout << "Name : " << it->type << std::endl;
        std::cout << "Start : " << it->startTime << std::endl;
        std::cout << "Speed : " << it->speed << std::endl;
        std::cout << "Parked : " << it->parked << std::endl;
        std::cout << "Location : " << it->curLocation << std::endl;
        std::cout << "Total : " << it->totalTime << std::endl;
        std::cout << "End : " << it->endTime << std::endl;
    }
}
