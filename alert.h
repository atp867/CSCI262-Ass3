/*************************************
*   CSCI262 Assignment 3
*   Intrusion Detection
*   alert.h :Header file
*   13/10/2018
*   <Anthony Pham - atp867 - 5146562>
*   <Daniel Nichols - dn688 - 5728356>
*   <Olivia Followes - of767 - 5395707>
*************************************/

//#ifndef ALERT_H
//#define ALERT_H

#include "activity.h"
#include "analysis.h"

struct AlertStats{
    float totalVehicles;
    float speedAvg;
};

struct day{
	int vehicleTotal;
    std::vector<AlertStats> alertStats;
};

class alertEngine
{
	private:
		//Variables
		char newFile[30]; // New stats file input by user
		int newDays; // New days input by user
		double volumeThresh;
		double speedThresh;
		float baseSpeedAvg;
		float baseSpeedStdDev;
		float baseVolAvg;
		float baseVolStdDev;
		
		std::vector<stats> baselineStats;
		std::vector<day> baselineDays;
		std::vector<day> testDays;
		std::vector<Vehicle> vehicleStats;
        std::vector<Stats> helpMe;
		
		//Functions
		void readAnalysisFile(int numVehicles);
		void readUserFile(activityEngine& activity);
		
	public:
		void calcThreshold(std::vector<Vehicle> vehicleSim);
		void readCurrDays(int days,int numVehicles);
		void startEngine(activityEngine activity, analysisEngine analysis);	
		void calcAnomaly(activityEngine activity, int day);
		float formulaCalc(float val, float stdDev, float mean, int weight);
};
