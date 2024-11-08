#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;

class Job
{
public:
    int id;
    int at;  // Arrival time
    int bt;  // Burst time
    int wt;  // Waiting time
    int tat; // Turnaround time
    int pr;  // Priority

    Job(int id, int at, int bt, int pr)
        : id(id), at(at), bt(bt), wt(0), tat(0), pr(pr) {}
};

void printJobTable(const vector<Job> &jobs)
{
    cout << "Job\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    cout << "---------------------------------------------------------------\n";
    for (const auto &job : jobs)
    {
        cout << job.id << "\t" << setw(12) << job.at << "\t"
             << setw(9) << job.bt << "\t" << setw(12) << job.wt
             << "\t" << setw(14) << job.tat << endl;
    }
}

void displayGanttChart(const vector<pair<int, int>> &timeline)
{
    cout << "\nGantt Chart:\n";
    for (const auto &entry : timeline)
    {
        cout << "| Job " << entry.first << " ";
    }
    cout << "|\n";

    int time = 0;
    for (const auto &entry : timeline)
    {
        cout << time << "\t";
        time = entry.second;
    }
    cout << time << "\n";
}

void FCFS(vector<Job> jobs)
{
    sort(jobs.begin(), jobs.end(), [](Job a, Job b)
         { return a.at < b.at; });

    int currTime = 0;
    vector<pair<int, int>> timeline;

    for (auto &job : jobs)
    {
        currTime = max(currTime, job.at);
        job.wt = currTime - job.at;
        job.tat = job.wt + job.bt;
        currTime += job.bt;
        timeline.push_back({job.id, currTime}); // curly braces are used to make a pair
    }

    printJobTable(jobs);
    displayGanttChart(timeline);
}

void SJF(vector<Job> jobs)
{
    int currTime = 0;
    vector<Job> completedJobs;
    vector<pair<int, int>> timeline;

    while (!jobs.empty())
    {
        int sjIdx = -1;
        int minBT = 100000;

        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].at <= currTime && jobs[i].bt < minBT)
            {
                sjIdx = i;
                minBT = jobs[i].bt;
            }
        }

        if (sjIdx == -1)
        {
            currTime++;
        }
        else
        {
            jobs[sjIdx].wt = currTime - jobs[sjIdx].at;
            jobs[sjIdx].tat = jobs[sjIdx].wt + jobs[sjIdx].bt;
            currTime += jobs[sjIdx].bt;
            timeline.push_back({jobs[sjIdx].id, currTime});
            completedJobs.push_back(jobs[sjIdx]);
            jobs.erase(jobs.begin() + sjIdx);
        }
    }

    printJobTable(completedJobs);
    displayGanttChart(timeline);
}

void RR(vector<Job> jobs, int tq)
{
    int currTime = 0;
    int n = jobs.size();
    vector<int> remainingTime(n);
    vector<bool> inQueue(n, false);
    vector<pair<int, int>> timeline;

    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = jobs[i].bt;
    }

    queue<int> jobQueue;

    for (int i = 0; i < n; i++)
    {
        if (jobs[i].at <= currTime)
        {
            jobQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (!jobQueue.empty())
    {
        int i = jobQueue.front();
        jobQueue.pop();

        if (remainingTime[i] > tq)
        {
            currTime += tq;
            remainingTime[i] -= tq;
            timeline.push_back({jobs[i].id, currTime});
        }
        else
        {
            currTime += remainingTime[i];
            jobs[i].tat = currTime - jobs[i].at;
            jobs[i].wt = jobs[i].tat - jobs[i].bt;
            remainingTime[i] = 0;
            timeline.push_back({jobs[i].id, currTime});
        }

        for (int j = 0; j < n; j++)
        {
            if (jobs[j].at <= currTime && remainingTime[j] > 0 && !inQueue[j])
            {
                jobQueue.push(j);
                inQueue[j] = true;
            }
        }

        if (remainingTime[i] > 0)
        {
            jobQueue.push(i);
        }
    }

    printJobTable(jobs);
    displayGanttChart(timeline);
}

void priorityScheduling(vector<Job> jobs)
{
    int currTime = 0;
    vector<Job> completedJobs;
    vector<pair<int, int>> timeline;

    while (!jobs.empty())
    {
        int hpIdx = -1;
        int minPr = 1e9;

        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].at <= currTime && jobs[i].pr < minPr)
            {
                hpIdx = i;
                minPr = jobs[i].pr;
            }
        }

        if (hpIdx == -1)
        {
            currTime++;
        }
        else
        {
            jobs[hpIdx].wt = currTime - jobs[hpIdx].at;
            jobs[hpIdx].tat = jobs[hpIdx].wt + jobs[hpIdx].bt;
            currTime += jobs[hpIdx].bt;
            timeline.push_back({jobs[hpIdx].id, currTime});
            completedJobs.push_back(jobs[hpIdx]);
            jobs.erase(jobs.begin() + hpIdx);
        }
    }

    printJobTable(completedJobs);
    displayGanttChart(timeline);
}

int main()
{
    int n, tq;
    cout << "Enter the number of jobs: ";
    cin >> n;
    cout << "Enter the time quantum for Round Robin: ";
    cin >> tq;

    vector<Job> jobs;
    for (int i = 0; i < n; i++)
    {
        int at, bt, pr;
        cout << "Enter arrival time for Job " << i + 1 << ": ";
        cin >> at;
        cout << "Enter burst time for Job " << i + 1 << ": ";
        cin >> bt;
        cout << "Enter priority for Job " << i + 1 << ": ";
        cin >> pr;
        jobs.push_back(Job(i + 1, at, bt, pr));
    }

    cout << "\nFirst-Come-First-Serve (FCFS) Scheduling:\n";
    FCFS(jobs);

    cout << "\nShortest Job First (SJF) Scheduling:\n";
    SJF(jobs);

    cout << "\nRound Robin (RR) Scheduling:\n";
    RR(jobs, tq);

    cout << "\nPriority Scheduling:\n";
    priorityScheduling(jobs);

    return 0;
}