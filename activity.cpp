/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   activity.cpp :Activity file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <name - login - Student #>
*************************************/

#include "activity.h"
#include <sstream>
#include <cstring>

using namespace std;

//Adds a vehicle to vehicle vector
void activityEngine::pushVehicles(Vehicle sample)
{
    vehicleSim.push_back(sample);
}

//Adds a stats object to vehicleStats vector
void activityEngine::pushStats(Stats sample)
{
    vehicleStats.push_back(sample);
}

//High-level description of function and parameters
void activityEngine::pushRoad(Road sample)
{   
    road.length = sample.length;
    road.speedLim = sample.speedLim;
    road.numParking = sample.numParking;
}

//Generates events for activity engine
void activityEngine::genEvents()
{
    srand(time(NULL));
    std::default_random_engine randEng;//not using seed for predictable testing
    int i = 0;
    //Iterate through all vehicle types
    for(std::vector<Vehicle>::iterator it = vehicleSim.begin(); it != vehicleSim.end(); it++)
    {
         //Use normal distribution to determine number of vehicles
         std::normal_distribution<float> normal(vehicleStats[i].avg, vehicleStats[i].stdDev);
        int loop = lround(normal(randEng));    
        for(int x = 0; x < loop; x++)
        {
            //Other variable intialised in constructor
            Instances temp;
            temp.type = i;//associate type by vector index
            temp.startTime = (rand() % MINUTESINDAY);//discrete event thus no stats required for generation
            while(temp.speed < 1)
            {//Vehicles must be moving forward to enter road
                std::normal_distribution<float> normalSpeed(vehicleStats[i].speedAvg, vehicleStats[i].speedStdDev);
                temp.speed = lround(normalSpeed(randEng));
            }

            temp.endTime = 0;
            instances.push_back(temp);
        }
        i++;
    }
}

//Start the activity engine
void activityEngine::startEngine(int days)
{
    for(int i = 0; i < days; i++) //Simulation main driver loop
    {
        std::cout << "---------------- DAY " << i << " ----------------" << std::endl;
        genEvents();
        simDay();
        //Statistics recording function here
        printInstances(i);
        clearInstances();
    }

}

//Clears all instances from simulation
void activityEngine::clearInstances()
{
    instances.clear();
}

//Simulates a day of activity
void activityEngine::simDay()
{
    srand(time(NULL));
    std::default_random_engine randEng;
    int hourClock = 0;
    int parkingUsed = 0;

    int exited = 0;
    int sideExited = 0;
    std::cout << " There are " << instances.size() << " instances" << std::endl;

    for(int i = 0; i < MINUTESINDAY; i++)
    {
        if(i % 60 == 0)
        {
            std::cout << "***************** Hour " << hourClock << "***************" <<std::endl;
            hourClock++;
        }
        for(std::vector<Instances>::iterator it = instances.begin(); it != instances.end(); it++)
        {
            if(it->startTime < i && it->endTime == 0)
            {
                if(it->curLocation >= road.length)
                {//vehicle reached end of road
                    //std::cout << "Vehicle has reached the end!" << std::endl;
                    it->endTime = i;
                    it->totalTime = it->endTime - it->startTime;
                    exited++;
                }
                if((rand() % MINUTESINDAY) == STREETEXIT && it->endTime == 0)
                {
                    //std::cout << "I have exited" << std::endl;
                    it->endTime = i;
                    it->totalTime = it->endTime - it->startTime;
                    sideExited++;
                }
                if(it->startTime < i && it->parked == false && it->endTime ==0)
                {
                    //std::cout << "HELP " << std::endl;
                    it->curLocation += it->speed/60;//update location if vehicle not parked 
                }
                if((rand() % MINUTESINDAY) < ISPARKED && parkingUsed < road.numParking && it->parked==false)//probability of parking
                {
                    it->parked = true;
                    if(!vehicleSim[it->type].parking)
                    {// vehicle not allowed to park
                        if(rand() % ISPARKED != 1)//probabilty they park anyway is 1/200
                        {//bit roundabout...
                            it->parked = false;
                            parkingUsed--;
                        }
                        else
                            std::cout << "Not allowed to park but i did it anyway?" << std::endl;
                    }
                    //std::cout << "I have parked" <<std::endl;
                    parkingUsed++;
                }
                int random = rand() % MINUTESINDAY;
                if(random < CHANGESPEED && it->endTime == 0)
                {//Probability to change speed
                //NEED TO FIND A WAY TO LINK TO STATS
                    if(it->parked)
                    {
                        //std::cout << "WAS PARKING" <<std::endl;
                        it->parked = false;
                        parkingUsed--;
                    }
                    //std::cout << "I have changed speeds" << std::endl;
                    std::normal_distribution<float> normal(vehicleStats[it->type].speedAvg,vehicleStats[it->type].speedStdDev);
                    it->speed = lround(normal(randEng));
                    //std::cout << "MY SPEED NOW IS " << it->speed << std::endl;
                }
                if(i == MINUTESINDAY-1 && it->endTime ==0)
                {//If day ends, remove vehicle
                    instances.erase(it);
                }
            }
        }
    }
    std::cout << exited << " MANY CARS EXITED" << std::endl;
    std::cout << sideExited << " MANY CARS EXITED VIA STREET" << std::endl;
    std::cout << " There are " << instances.size() << " instances" << std::endl;
}

//Print vehicle details and stats for all vehicles in simulation
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

//Print all current vehicle instances in simulation
void activityEngine::printInstances(int days)
{
    char file[12];
    std::ostringstream oss;
    oss << "day" << days << ".txt";
    strcpy(file,(oss.str()).c_str());

    std::ofstream fout;
    fout.open(file);
    std::cout << "Day " << days << " Simulation complete " << std::endl;
    std::cout << "Day " << days << " Complete logging data to " << file << std::endl;

    for(std::vector<Instances>::iterator it = instances.begin(); it != instances.end(); it++)
    {
        fout << "******Vehicle******" << std::endl;
        fout << "Name : " << vehicleSim[it->type].name << std::endl;
        fout << "Type : " << it->type << std::endl;
        fout << "Start : " << it->startTime << std::endl;
        fout << "Speed : " << it->speed << std::endl;
        fout << "Location : " << it->curLocation << std::endl;
        fout << "Total : " << it->totalTime << std::endl;
        fout << "End : " << it->endTime << std::endl;
    }
    fout.close();
}
