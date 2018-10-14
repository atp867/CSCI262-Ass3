/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   activity.h :Header file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <name - login - Student #>
*************************************/

#ifndef ACTIVITY_H
#define ACTIVITY_H 

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>

const static int MINUTESINDAY = 1440;
/***********Probability***********/
const static int ISPARKED = 244;
const static int CHANGESPEED = 100;
const static int STREETEXIT = 13;

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
        int type;
        int startTime;
        float speed;
        bool parked;
        int parkingTime;
        int endTime;
        /***Stats data***/
        float initSpeed;
        float curLocation;
        int totalTime;
        Instances(){
            type = -1;
            initSpeed = 0;
            speed = 0;
            parked = 0;
            parkingTime = 0;
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
        void startEngine(int);
        void clearInstances();
        void simDay();
        Road getRoad();
        void printVehicles();
        void printInstances(int);
};
#endif
