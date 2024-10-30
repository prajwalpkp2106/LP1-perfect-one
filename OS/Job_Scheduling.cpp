//Turnaround time represents the total time from a job's arrival to its completion.
// burst time is time of execution
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Job
{
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int priority;
};

void printJobTable(vector<Job> &jobs)
{
    cout << "Job\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (Job &job : jobs)
    {
        cout << job.id << "\t" << job.arrival_time << "\t\t" << job.burst_time << "\t\t" << job.waiting_time << "\t\t" << job.turnaround_time << endl;
    }
}

// First-Come-First-Serve (FCFS) Scheduling
void FCFS(vector<Job> jobs)
{
    sort(jobs.begin(), jobs.end(), [](Job a, Job b)
         { return a.arrival_time < b.arrival_time; });//sorts the jobs vector by each job’s arrival_time

    int currentTime = 0;//representing the start of processing time. It will increment as each job executes, simulating the passage of time in the CPU.
    for (Job &job : jobs)
    {
        currentTime = max(currentTime, job.arrival_time);//CPU waits until the job has arrived before starting its execution
        //Suppose the currentTime is 3, and a job arrives at arrival_time = 5. The CPU will wait until time 5 to start this job, so currentTime is updated to 5
        job.waiting_time = currentTime - job.arrival_time;//The waiting time is calculated by subtracting the job’s arrival_time from currentTime
        job.turnaround_time = job.waiting_time + job.burst_time;
        currentTime += job.burst_time;//increment time of the CPU by the burst time of the job
    }
    printJobTable(jobs);//print the job table
}

// Shortest Job First (SJF) Scheduling- jobs are executed in the order of their burst times (the time each job takes to execute) whenever they arrive
void SJF(vector<Job> jobs)
{
    int currentTime = 0;
    vector<Job> completedJobs;//use this to record and display the final results.


    while (!jobs.empty())
    {
        int shortestJobIdx = -1;//keeps track of the index of the job with the shortest burst time available at the currentTime. It's initially set to -1, indicating that no job has been selected.
        int shortestBurstTime = 100000;//shortestBurstTime is initialized to a large number (100000) to ensure any burst time in the job list will be smaller and thus replace it.

        for (int i = 0; i < jobs.size(); i++)//Find the Job with the Shortest Burst Time that has Arrived
        {
            if (jobs[i].arrival_time <= currentTime && jobs[i].burst_time < shortestBurstTime)
            {
                shortestJobIdx = i;
                shortestBurstTime = jobs[i].burst_time;
            }
        }

        if (shortestJobIdx == -1)//it means that no job has arrived by the current time. In this case, we increment the currentTime by 1.
        {
            currentTime++;//ncremented by 1 to simulate the passage of time while waiting for a job to arrive.
        }
        else
        {
            Job &job = jobs[shortestJobIdx];//If a job has arrived, we update the waiting time, turnaround time, and currentTime.
            job.waiting_time = currentTime - job.arrival_time;//same as fcfs
            job.turnaround_time = job.waiting_time + job.burst_time;
            currentTime += job.burst_time;
            completedJobs.push_back(job);//Adds the job to completedJobs, as it’s now completed.
            jobs.erase(jobs.begin() + shortestJobIdx);//Removes the job from the jobs list since it’s now completed.
        }
    }
    printJobTable(completedJobs);
}

// Round Robin (RR) Scheduling-meaning each job gets a turn to execute for a specified amount of time. If a job doesn’t complete in its time slice, it goes back into the queue for another turn.
void RR(vector<Job> &jobs, int timeQuantum)
{
    int currentTime = 0;
    int n = jobs.size();
    vector<int> remainingTime(n);//remainingTime vector to keep track of the remaining time for each job
    vector<bool> inQueue(n, false); // Tracks if a job is already in the queue

    // Initialize remaining time
    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = jobs[i].burst_time;
    }

    queue<int> jobQueue;//queue to store the jobs that are currently in the queue or executing

    // Enqueue jobs that have arrived at the initial time
    for (int i = 0; i < n; i++)//Adds all jobs with arrival_time <= currentTime to jobQueue, marking them in inQueue to avoid re-adding them before they complete
    {
        if (jobs[i].arrival_time <= currentTime)//cureent time is 0 so ek koi to arrival 0 hona chaiiye varna error ayega
        {
            jobQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (!jobQueue.empty())
    {
        int i = jobQueue.front();
        jobQueue.pop();

        // Process the current job
        if (remainingTime[i] > timeQuantum)
        {
            currentTime += timeQuantum;
            remainingTime[i] -= timeQuantum;
        }
        else
        {
            currentTime += remainingTime[i];
            jobs[i].turnaround_time = currentTime - jobs[i].arrival_time;
            jobs[i].waiting_time = jobs[i].turnaround_time - jobs[i].burst_time;
            remainingTime[i] = 0;
        }

        // Check for new arrivals during the current time slice and enqueue them
        for (int j = 0; j < n; j++)
        {
            if (jobs[j].arrival_time <= currentTime && remainingTime[j] > 0 && !inQueue[j])
            {
                jobQueue.push(j);
                inQueue[j] = true;
            }
        }

        // Re-queue the current job if it's not completed
        if (remainingTime[i] > 0)
        {
            jobQueue.push(i);
        }
    }

    printJobTable(jobs); // Print the results
}

// Priority Scheduling
void priorityScheduling(vector<Job> jobs)
{
    int currentTime = 0;
    vector<Job> completedJobs;

    while (!jobs.empty())
    {
        int highestPriorityIdx = -1;
        int highestPriority = 100000;

        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].arrival_time <= currentTime && jobs[i].priority < highestPriority)
            {
                highestPriorityIdx = i;
                highestPriority = jobs[i].priority;
            }
        }

        if (highestPriorityIdx == -1)
        {
            currentTime++;
        }
        else
        {
            Job &job = jobs[highestPriorityIdx];
            job.waiting_time = currentTime - job.arrival_time;
            job.turnaround_time = job.waiting_time + job.burst_time;
            currentTime += job.burst_time;
            completedJobs.push_back(job);
            jobs.erase(jobs.begin() + highestPriorityIdx);
        }
    }
    printJobTable(completedJobs);
}

int main()
{
    int n, timeQuantum;
    cout << "Enter the number of jobs: ";
    cin >> n;
    cout << "Enter the time quantum for Round Robin: ";
    cin >> timeQuantum;

    vector<Job> jobs(n);

    for (int i = 0; i < n; i++)
    {
        jobs[i].id = i + 1;
        cout << "Enter arrival time for Job " << i + 1 << ": ";
        cin >> jobs[i].arrival_time;
        cout << "Enter burst time for Job " << i + 1 << ": ";
        cin >> jobs[i].burst_time;
        cout << "Enter priority for Job " << i + 1 << ": ";
        cin >> jobs[i].priority;
    }

    cout << "\nFirst-Come-First-Serve (FCFS) Scheduling:" << endl;
    FCFS(jobs); // Pass a copy to prevent modification

    cout << "\nShortest Job First (SJF) Scheduling:" << endl;
    SJF(jobs); // Pass a copy

    cout << "\nRound Robin (RR) Scheduling:" << endl;
    RR(jobs, timeQuantum); // Pass a copy

    cout << "\nPriority Scheduling:" << endl;
    priorityScheduling(jobs); // Pass a copy

    return 0;
}
