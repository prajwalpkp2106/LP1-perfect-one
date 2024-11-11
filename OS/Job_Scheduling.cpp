#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

class Job
{
public:
    string s;
    int at, bt, wt, tat, pr;

    Job()
    {
        s = "";
        at = bt = wt = tat = 0;
    }

    void getData()
    {
        cout << "Name : ";
        cin >> s;
        cout << "AT: ";
        cin >> at;
        cout << "BT: ";
        cin >> bt;
        cout << "PR: ";
        cin >> pr;
    }
};

void Table(vector<Job> v)
{
    cout << "P \t   AT \t   BT \t   WT \t   TAT" << endl;
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i].s << "\t\t" << v[i].at << "\t\t" << v[i].bt << "\t\t" << v[i].wt << "\t\t" << v[i].tat << endl;
    }
}

void FCFS(vector<Job> jobs)
{
    cout << "----FCFS----" << endl;

    int n = jobs.size();

    sort(jobs.begin(), jobs.end(), [](Job a, Job b)
         { return a.at < b.at; });

    int t = 0;

    for (int i = 0; i < n; i++)
    {
        t = max(t, jobs[i].at);
        jobs[i].wt = t - jobs[i].at;
        jobs[i].tat = jobs[i].wt + jobs[i].bt;
        t += jobs[i].bt;
    }

    Table(jobs);
}

void SJF_Preemptive(vector<Job> jobs)
{
    cout << "----- Preemptive SJF (SRTF) -----" << endl;

    int n = jobs.size();
    vector<int> rembt(n); // Remaining burst time
    int t = 0, completed = 0;
    vector<Job> temp;

    for (int i = 0; i < n; i++)
    {
        rembt[i] = jobs[i].bt; // Initialize remaining burst times
    }

    while (completed < n)
    {
        int ind = -1;
        int minBT = 999;

        // Find the job with the shortest remaining burst time at current time `t`
        for (int i = 0; i < n; i++)
        {
            if (jobs[i].at <= t && rembt[i] > 0 && rembt[i] < minBT)
            {
                ind = i;
                minBT = rembt[i];
            }
        }

        if (ind == -1)
        {
            t++; // No job is ready, increment time
        }
        else
        {
            rembt[ind]--; // Process the selected job for 1 unit of time
            t++;          // Increment time

            // If the job is completed
            if (rembt[ind] == 0)
            {
                completed++;
                jobs[ind].tat = t - jobs[ind].at;
                jobs[ind].wt = jobs[ind].tat - jobs[ind].bt;
                temp.push_back(jobs[ind]);
            }
        }
    }
    Table(temp);
}

void Priority(vector<Job> jobs)
{
    cout << "----- Priority -----" << endl;

    vector<Job> temp;
    int t = 0;

    while (!jobs.empty())
    {
        int ind = -1;
        int minPR = 999;
        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].pr < minPR && jobs[i].at <= t)
            {
                ind = i;
                minPR = jobs[i].pr;
            }
        }

        if (ind == -1)
        {
            t++;
        }
        else
        {
            jobs[ind].wt = t - jobs[ind].at;
            jobs[ind].tat = jobs[ind].wt + jobs[ind].bt;
            t += jobs[ind].bt;
            temp.push_back(jobs[ind]);
            jobs.erase(jobs.begin() + ind);
        }
    }
    Table(temp);
}

void RoundRobin(vector<Job> &jobs, int quantum)
{
    cout << "-----Round Robin-----" << endl;

    int n = jobs.size();

    vector<int> rembt(n);
    vector<bool> inq(n, false);
    vector<int> st(n, -1);

    for (int i = 0; i < n; i++)
        rembt[i] = jobs[i].bt;

    int t = 0;
    queue<int> q;

    for (int i = 0; i < n; i++)
    {
        if (jobs[i].at <= t)
        {
            q.push(i);
            inq[i] = true;
            st[i] = t;
        }
    }

    while (!q.empty())
    {
        int i = q.front();
        q.pop();
        inq[i] = false;

        if (rembt[i] > quantum)
        {
            t += quantum;
            rembt[i] -= quantum;
        }
        else
        {
            t += rembt[i];
            rembt[i] = 0;

            jobs[i].wt = t - jobs[i].at - jobs[i].bt;
            jobs[i].tat = jobs[i].bt + jobs[i].wt;

            if (st[i] != jobs[i].at)
            {
                jobs[i].wt = st[i] - jobs[i].at;
            }
        }

        for (int j = 0; j < n; j++)
        {
            if (jobs[j].at <= t && rembt[j] > 0 && !inq[j])
            {
                q.push(j);
                inq[j] = true;
                if (st[j] == -1)
                {
                    st[j] = t;
                }
            }
        }

        // Re-queue the current job if it is not finished
        if (rembt[i] > 0)
        {
            q.push(i);
            inq[i] = true;
        }
    }

    Table(jobs); // Display results
}

int main()
{
    int n;
    cout << "Enter Total Jobs: ";
    cin >> n;

    vector<Job> jobs;

    for (int i = 0; i < n; i++)
    {
        Job j;
        j.getData();
        jobs.push_back(j);
    }

    FCFS(jobs);

    SJF_Preemptive(jobs);

    Priority(jobs);
    cout << "enter quanta for rr: ";
    int qua;
    cin >> qua;
    RoundRobin(jobs, qua);

    return 0;
}