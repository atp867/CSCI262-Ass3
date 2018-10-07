/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   atp867, 
*   activity.h :Header file
*   1/10/2018
*************************************/
#ifndef ACTIVITY_H
#define ACTIVITY_H 

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

const static int MINUTESINDAY = 1440;

struct Vehicle{
    std::string name;
    bool parking;
    std::string rego;
    unsigned int volWeight;
    unsigned int speedWeight;
};

struct Stats{
    std::string name;
    float avg;
    float stdDev;
    float speedAvg;
    float speedStdDev;
};

struct Road{
    unsigned int length;
    unsigned int speedLim;
    unsigned int numParking;
};

struct Instances{
        std::string type;
        int startTime;
        float speed;
        bool parked;
        /***Stats data***/
        float curLocation;
        int totalTime;
        int endTime;
        Instances(){
            type = "";
            speed = 0;
            parked = 0;
            curLocation = 0;
            totalTime = 0;
            endTime = 0;
        };
};

class activityEngine{
    private:
        Road road;
        std::vector<Vehicle> vehicleSim;
        std::vector<Stats> vehicleStats;
        std::vector<Instances> instances;
    public:
        void pushVehicles(Vehicle);
        void pushStats(Stats);
        void pushRoad(Road);
        void genEvents();
        void printVehicles();
        void printInstances();
};
#endif
