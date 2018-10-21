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

struct day{
	int vehicleTotal;
	float busNum;
	float busSpeedAvg;
	float motorbikeNum;
	float motorbikeSpeedAvg;
	float carNum;
	float carSpeedAvg;
	float elephantNum;
	float elephantSpeedAvg;
	float taxiNum;
	float taxiSpeedAvg;
	float emergencyNum;
	float emergencySpeedAvg;
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
		
		//Functions
		void readAnalysisFile();
		void readUserFile();
		
	public:
		void calcThreshold(std::vector<Vehicle> vehicleSim);
		void calcAnomalyCount(activityEngine activity);
		void startEngine();	
};
