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
    for (const Job &job : jobs)
    {
        cout << job.id << "\t" << job.arrival_time << "\t\t" << job.burst_time << "\t\t" << job.waiting_time << "\t\t" << job.turnaround_time << endl;
    }
}

// First-Come-First-Serve (FCFS) Scheduling
void FCFS(vector<Job> jobs)
{
    sort(jobs.begin(), jobs.end(), [](const Job &a, const Job &b)
         { return a.arrival_time < b.arrival_time; });

    int currentTime = 0;
    for (Job &job : jobs)
    {
        currentTime = max(currentTime, job.arrival_time);
        job.waiting_time = currentTime - job.arrival_time;
        job.turnaround_time = job.waiting_time + job.burst_time;
        currentTime += job.burst_time;
    }
    printJobTable(jobs);
}

// Shortest Job First (SJF) Scheduling
void SJF(vector<Job> jobs)
{
    int currentTime = 0;
    vector<Job> completedJobs;

    while (!jobs.empty())
    {
        int shortestJobIdx = -1;
        int shortestBurstTime = 100000;

        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].arrival_time <= currentTime && jobs[i].burst_time < shortestBurstTime)
            {
                shortestJobIdx = i;
                shortestBurstTime = jobs[i].burst_time;
            }
        }

        if (shortestJobIdx == -1)
        {
            currentTime++;
        }
        else
        {
            Job &job = jobs[shortestJobIdx];
            job.waiting_time = currentTime - job.arrival_time;
            job.turnaround_time = job.waiting_time + job.burst_time;
            currentTime += job.burst_time;
            completedJobs.push_back(job);
            jobs.erase(jobs.begin() + shortestJobIdx);
        }
    }
    printJobTable(completedJobs);
}

// Round Robin (RR) Scheduling
void RR(vector<Job> &jobs, int timeQuantum)
{
    int currentTime = 0;
    int n = jobs.size();
    vector<int> remainingTime(n);
    vector<bool> inQueue(n, false); // Tracks if a job is already in the queue

    // Initialize remaining time
    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = jobs[i].burst_time;
    }

    queue<int> jobQueue;

    // Enqueue jobs that have arrived at the initial time
    for (int i = 0; i < n; i++)
    {
        if (jobs[i].arrival_time <= currentTime)
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
