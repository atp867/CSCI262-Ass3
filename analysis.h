/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   anaylsis.h :Analysis Header file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <name - login - Student #>
*************************************/
#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "activity.h"
#include <iostream>
#include <sstream>
#include <cstring>

struct stats{
    std::string type;
    int total;
    float averageSpeed;
    float stdDevSpeed;
    float average;
    std::vector<Instances> instances;

};

struct Day{
    int vehicleTotal;
    std::vector<stats> vehicleStats;
    float averageSpeed;
};

class analysisEngine{
    private:
        std::vector<Day> days;
        std::vector<stats> totalStats;
        float totalAvgSpeed;
        float totalStdDevSpeed;
        float totalAvgVolume;
        float totalStdDevVolume;
    public:
        analysisEngine();
        void startEngine(int, int);
        void readLogs();
        void printInstances();
        void dayStatistics(int);
        void totalStatistics();
};
#endif
