#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

class Job
{
public:
    string s; // Job name
    int at;   // Arrival Time
    int bt;   // Burst Time
    int wt;   // Waiting Time
    int tat;  // Turnaround Time
    int pr;   // Priority

    Job()
    {
        s = "";
        at = 0;
        bt = 0;
        wt = 0;
        tat = 0;
        pr = 0;
    }

    void getData()
    {
        cout << "Job Name: ";
        cin >> s;
        cout << "Arrival Time: ";
        cin >> at;
        cout << "Burst Time: ";
        cin >> bt;
        cout << "Priority: ";
        cin >> pr;
    }
};

void printTable(vector<Job> v)
{
    cout << "Job\tAT\tBT\tWT\tTAT\n";
    cout << "----------------------------------------------------\n";
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i].s << "\t" << v[i].at << "\t" << v[i].bt << "\t" << v[i].wt << "\t" << v[i].tat << endl;
    }
}

void FCFS(vector<Job> jobs)
{
    cout << "-----FCFS-----" << endl;
    sort(jobs.begin(), jobs.end(), [](Job a, Job b) { return a.at < b.at; });

    int c = 0;
    for (int i = 0; i < jobs.size(); i++)
    {
        c = max(c, jobs[i].at);
        jobs[i].wt = c - jobs[i].at;
        jobs[i].tat = jobs[i].wt + jobs[i].bt;
        c += jobs[i].bt;
    }
    printTable(jobs);
}

void SJF(vector<Job> jobs)
{
    cout << "-----SJF-----" << endl;
    int c = 0;
    vector<Job> temp;

    while (!jobs.empty())
    {
        int ind = -1;
        int minBT = 999;

        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].at <= c && jobs[i].bt < minBT)
            {
                ind = i;
                minBT = jobs[i].bt;
            }
        }

        if (ind == -1)
        {
            c++;
        }
        else
        {
            jobs[ind].wt = c - jobs[ind].at;
            jobs[ind].tat = jobs[ind].wt + jobs[ind].bt;
            c += jobs[ind].bt;
            temp.push_back(jobs[ind]);
            jobs.erase(jobs.begin() + ind);
        }
    }
    printTable(temp);
}

void Priority(vector<Job> jobs)
{
    cout << "-----Priority-----" << endl;
    int c = 0;
    vector<Job> temp;

    while (!jobs.empty())
    {
        int ind = -1;
        int maxPr = 999;

        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].at <= c && jobs[i].pr < maxPr)
            {
                ind = i;
                maxPr = jobs[i].pr;
            }
        }

        if (ind == -1)
        {
            c++;
        }
        else
        {
            jobs[ind].wt = c - jobs[ind].at;
            jobs[ind].tat = jobs[ind].wt + jobs[ind].bt;
            c += jobs[ind].bt;
            temp.push_back(jobs[ind]);
            jobs.erase(jobs.begin() + ind);
        }
    }
    printTable(temp);
}
void printQueue(std::queue<int> q) {
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << std::endl;
}

void RoundRobin(vector<Job> &jobs, int quantum) {
    cout << "-----Round Robin-----" << endl;
    int n = jobs.size();
    vector<int> rem_bt(n);           // Remaining burst times for each job
    vector<bool> inQueue(n, false);   // Track if job is already in the queue
    vector<int> start_time(n, -1);    // Track the first time each job starts
    
    // Initialize remaining burst times
    for (int i = 0; i < n; i++)
        rem_bt[i] = jobs[i].bt;

    int t = 0; // Current time
    queue<int> q;

    // Add initial jobs to the queue based on arrival time
    for (int i = 0; i < n; i++) {
        if (jobs[i].at <= t) {
            q.push(i);
            inQueue[i] = true;
            start_time[i] = t;
        }
    }

    while (!q.empty()) {
        int i = q.front();
        q.pop();
        inQueue[i] = false;

        // Process the job for a time slice or until completion
        if (rem_bt[i] > quantum) {
            t += quantum;
            rem_bt[i] -= quantum;
        } else {
            t += rem_bt[i];
            rem_bt[i] = 0;

            // Calculate turnaround and waiting times upon completion
            jobs[i].wt = t - jobs[i].at - jobs[i].bt;
            jobs[i].tat = jobs[i].bt + jobs[i].wt;
            // Correct WT for jobs that start exactly at their arrival time
            if (start_time[i] != jobs[i].at) {
                jobs[i].wt = start_time[i] - jobs[i].at;
            }
        }

        // Add newly arrived jobs to the queue
        for (int j = 0; j < n; j++) {
            if (jobs[j].at <= t && rem_bt[j] > 0 && !inQueue[j]) {
                q.push(j);
                inQueue[j] = true;
                if (start_time[j] == -1) {
                    start_time[j] = t;
                }
            }
        }

        // Re-queue the current job if it is not finished
        if (rem_bt[i] > 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    printTable(jobs); // Display results
}

int main()
{
    int n;
    cout << "Enter Total number of Jobs: ";
    cin >> n;

    vector<Job> jobs;

    for (int i = 0; i < n; i++)
    {
        Job j;
        j.getData();
        jobs.push_back(j);
    }

    int choice;
    int quantum;
    do
    {
        cout << "\nSelect Scheduling Algorithm:\n";
        cout << "1. FCFS\n";
        cout << "2. SJF\n";
        cout << "3. Priority\n";
        cout << "4. Round Robin\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        vector<Job> jobsCopy = jobs;

        switch (choice)
        {
            case 1:
                FCFS(jobsCopy);
                break;
            case 2:
                SJF(jobsCopy);
                break;
            case 3:
                Priority(jobsCopy);
                break;
            case 4:
                cout << "Enter Time Quantum: ";
                cin >> quantum;
                RoundRobin(jobsCopy, quantum);
                break;
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 5);

    return 0;
}