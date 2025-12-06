#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif
#include "schedwork.h"
using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Add prototypes for any helper functions here
bool helper(size_t day, DailySchedule& schedule, size_t dailyWorkerCount, size_t currWorker, size_t maxShiftCount, vector<size_t>& workerShiftCounts, const AvailabilityMatrix& available);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    size_t dayCount = avail.size(); 
    size_t workerCount = avail[0].size(); 

    //set the schedule to number of days 
    sched.resize(dayCount);

    vector<size_t> workerShifts(workerCount);    //make a list to track shifts worked per worker
    for (size_t i =0; i < workerCount; i++){
        workerShifts[i] = 0; //start with 0 shifts 
    }

    return helper(0, sched, dailyNeed, 0, maxShifts, workerShifts, avail);
}

bool helper(size_t day, DailySchedule& schedule, size_t dailyWorkerCount, size_t currWorker, size_t maxShiftCount, vector<size_t>& workerShiftCounts, const AvailabilityMatrix& available){
    size_t dayCount = available.size(); 
    size_t workerCount = available[0].size(); 

    //check if the days are filled 
    if (day == dayCount){
        return true;    //done
    }

    //check if day has enough workers 
    size_t scheduledWorkerCount = schedule[day].size(); 
    if (scheduledWorkerCount == dailyWorkerCount){
        //go to next day 
        return helper(day+1, schedule, dailyWorkerCount, 0, maxShiftCount, workerShiftCounts, available); 
    }

    //there aren't enough workers scheduled and all workers have been tried 
    if(currWorker >= workerCount){
        return false;   //backtrack
    }
    
    //check if worker is already scheduled for this day
    bool working = false;
    for (size_t i = 0; i < schedule[day].size(); i++){
        if (schedule[day][i] == currWorker){
            working = true; 
            break;
        }
    }

    bool availableToWork = available[day][currWorker]; //worker must be available to work
    bool maxShiftSafe = workerShiftCounts[currWorker] < maxShiftCount; //worker must not exceed maxShiftCount
    //add needed worker to the day
    if (availableToWork == true && maxShiftSafe == true && working == false){
        
        schedule[day].push_back(currWorker);    
        workerShiftCounts[currWorker]++;    //update shift count

        //try adding the next worker  
        bool canAdd = helper(day, schedule, dailyWorkerCount, currWorker + 1, maxShiftCount, workerShiftCounts, available); 
        if (canAdd == true){
            return true; 
        }
        //else there is no one to add; backtrack and update count 
        else{
            schedule[day].pop_back(); 
            workerShiftCounts[currWorker] -= 1; 
        }
    }
    return helper(day, schedule, dailyWorkerCount, currWorker + 1, maxShiftCount, workerShiftCounts, available); 
}