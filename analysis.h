/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   anaylsis.h :Analysis Header file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <Olivia Followes - of767 - 5395707>
*************************************/
#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "activity.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <map>

struct stats{
    std::vector<Instances> instances;
    int total;
    /****Potentially redundant data****/
    std::string type;
    float averageSpeed;
    float stdDevSpeed;
    float averageVolume;
    float stdDevVolume;
};

struct Day{
    int vehicleTotal;
    std::vector<stats> vehicleStats;
    /****Potentially redundant data****/
    float meanSpeed;
    float meanVolume;
    float stdDevSpeed;
    float stdDevVolume;
};

class analysisEngine{
    private:
        std::vector<Day> days;
        std::vector<stats> totalStats;
        std::vector<Instances> totalInstances;
        std::map<int,Instances> speedBreaches;
        float totalAvgSpeed;
        float totalStdDevSpeed;
        float totalAvgVolume;
        float totalStdDevVolume;
    public:
        analysisEngine();
        void startEngine(int, int,Road);
        void readLogs();
        void totalStatistics(Road);
        void printStatistics();
        void vehicleStatistics();
        /*
        void dayStatistics(int,Road);
        void printInstances();
        */
};
#endif
