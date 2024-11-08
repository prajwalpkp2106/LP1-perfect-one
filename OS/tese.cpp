#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Job
{
public:
    int id;
    int at;
    int bt;
    int wt;
    int tat;
    int pr; // Priority

    Job(int id, int at, int bt, int wt, int tat, int pr)
    {
        this->id = id;
        this->at = at;
        this->bt = bt;
        this->wt = 0;
        this->tat = 0;
        this->pr = pr;
    }
};
void printJobTable(vector<Job> jobs)
{
    cout << "Job\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (auto job : jobs)
    {
        cout << job.id << "\t" << job.at << "\t" << job.bt << "\t" << job.wt << "\t" << job.tat << endl;
    }
}
void FCFS(vector<Job> jobs)
{
    sort(jobs.begin(), jobs.end(), [](Job &a, Job &b)
         { return a.at < b.at; });
    int currentTime = 0;
    vector<pair<int, int>> timeline;
    for (auto &job : jobs)
    {
        currentTime = max(currentTime, job.at);
        job.wt = currentTime - job.at;
        job.tat = job.wt + job.bt;
        currentTime += job.bt;
        timeline.push_back({job.id, currentTime});
    }
    printJobTable(jobs);
}
void SJF(vector<Job> jobs)
{
    int currtime = 0;
    vector<Job> compjob;
    vector<pair<int, int>> timeline;
    while (!jobs.empty())
    {
        int indx = -1;
        int minBT = 99999;
        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].at <= currtime && jobs[i].bt < minBT)
            {
                indx = i;
                minBT = jobs[i].bt;
            }
        }
        if (indx == -1)
        {
            currtime++;
        }
        else
        {
            jobs[indx].wt = currtime - jobs[indx].at;
            jobs[indx].tat = jobs[indx].wt + jobs[indx].bt;
            currtime += jobs[indx].bt;
            timeline.push_back({jobs[indx].id, currtime});
            compjob.push_back(jobs[indx]);
            jobs.erase(jobs.begin() + indx);
        }
    }
    printJobTable(compjob);
}

void prioritysch(vector<Job> jobs)
{
    int currtime = 0;
    vector<Job> compjob;
    vector<pair<int, int>> timeline;
    while (!jobs.empty())
    {
        int indx = -1;
        int minpri = 99999;
        for (int i = 0; i < jobs.size(); i++)
        {
            if (jobs[i].at <= currtime && jobs[i].pr < minpri)
            {
                indx = i;
                minpri = jobs[i].pr;
            }
        }
        if (indx == -1)
        {
            currtime++;
        }
        else
        {
            jobs[indx].wt = currtime - jobs[indx].at;
            jobs[indx].tat = jobs[indx].wt + jobs[indx].bt;
            currtime += jobs[indx].bt;
            timeline.push_back({jobs[indx].id, currtime});
            compjob.push_back(jobs[indx]);
            jobs.erase(jobs.begin() + indx);
        }
    }
    printJobTable(compjob);
}

int main()
{
    int n;
    cout << "Enter the number of jobs:";
    cin >> n;
    vector<Job> jobs;
    for (int i = 0; i < n - 1; i++)
    {
        int id;
        int at;
        int bt;
        int pr;
        cout << "Enter the at for job " << i + 1 << " ";
        cin >> at;
        cout << "Enter the bt for job " << i + 1 << " ";
        cin >> bt;
        cout << "Enter the pr for job " << i + 1 << " ";
        cin >> pr;
        jobs.push_back(Job(i + 1, at, bt, 0, 0, pr));
    }
    FCFS(jobs);
    cout << endl;
    prioritysch(jobs);
    cout << endl;
    SJF(jobs);
    return 0;
}