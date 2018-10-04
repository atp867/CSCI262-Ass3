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

class Events{
    private:
    public:

};
#endif
