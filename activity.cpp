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
    std::default_random_engine randEng;//not using seed for predictable testing
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
            temp.type = i;//associate type by vector index
            temp.startTime = (rand() % MINUTESINDAY);
            while(temp.speed < 1)
            {//Vehicles must be moving forward to enter road
                std::normal_distribution<float> normalSpeed(vehicleStats[i].speedAvg,vehicleStats[i].speedStdDev);
                temp.speed = lround(normalSpeed(randEng));
            }

            temp.endTime = 0;
            instances.push_back(temp);
        }
        i++;
    }
}

void activityEngine::simDay()
{
    srand(time(NULL));
    std::default_random_engine randEng;
    int hourClock = 0;
    int parkingUsed = 0;

    for(int i = 0; i < MINUTESINDAY; i++)
    {
        if(i % 60 == 0)
        {
            std::cout << "*****************Hour " << hourClock << "***************" <<std::endl;
            hourClock++;
        }
        for(std::vector<Instances>::iterator it = instances.begin(); it != instances.end(); it++)
        {
            if(it->startTime < i && it->endTime == 0)
            {
                if(it->startTime < i && it->parked == false)
                {
                    std::cout << "HELP " << std::endl;
                    it->curLocation += it->speed/60;//update location if vehicle not parked 
                }
                if((rand() % MINUTESINDAY) < ISPARKED && parkingUsed < road.numParking && it->parked==false)//probability of parking
                {
                    std::cout << "I have parked" <<std::endl;
                    it->parked = true;
                    parkingUsed++;
                }
                if(it->curLocation >= road.length)
                {//vehicle reached end of road
                    std::cout << "Vehicle has reached the end!" << std::endl;
                    it->endTime = i;
                    it->totalTime = it->endTime - it->startTime;
                }
                if((rand() % MINUTESINDAY) == STREETEXIT && it->endTime == 0)
                {
                    std::cout << "I have exited" << std::endl;
                    it->endTime = i;
                    it->totalTime = it->endTime - it->startTime;
                }
                int random = rand() % MINUTESINDAY;
                //std::cout <<"THIS STUPID NUMBER IS " << random << std::endl;
                if(random < CHANGESPEED && it->endTime == 0)
                {//Probability to change speed
                //NEED TO FIND A WAY TO LINK TO STATS
                    if(it->parked)
                    {
                        std::cout << "WAS PARKING" <<std::endl;
                        it->parked = false;
                        parkingUsed--;
                    }
                    std::cout << "I have changed speeds" << std::endl;
                    std::normal_distribution<float> normal(vehicleStats[it->type].speedAvg,vehicleStats[it->type].speedStdDev);
                    it->speed = lround(normal(randEng));
                    std::cout << "MY SPEED NOW IS " << it->speed << std::endl;
                }
                if(i == MINUTESINDAY-1 && !it->parked && it->endTime ==0)
                {//If day ends, the vehicle has left via street
                    it->endTime = MINUTESINDAY;
                    it->totalTime = it->endTime - it->startTime;
                }
            }
        }
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
        std::cout << "******Vehicle******" << std::endl;
        std::cout << "Name : " << it->type << std::endl;
        std::cout << "Start : " << it->startTime << std::endl;
        std::cout << "Speed : " << it->speed << std::endl;
        std::cout << "Parked : " << it->parked << std::endl;
        std::cout << "Location : " << it->curLocation << std::endl;
        std::cout << "Total : " << it->totalTime << std::endl;
        std::cout << "End : " << it->endTime << std::endl;
    }
}
