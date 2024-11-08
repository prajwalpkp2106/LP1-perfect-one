#include <iostream>
#include <queue>
using namespace std;

// Function to find the waiting time for all processes
void findWaitingTime(int processes[], int n, int bt[], int wt[], int at[], int quantum)
{
    int rem_bt[n]; // Remaining burst times
    for (int i = 0; i < n; i++)
        rem_bt[i] = bt[i];

    int t = 0; // Current time
    queue<int> q; // Queue to hold processes in Round Robin order
    bool inQueue[n] = {false}; // Track if process is in queue

    // Add processes to queue based on arrival time
    for (int i = 0; i < n; i++) {
        if (at[i] <= t && !inQueue[i]) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    // Process in Round Robin manner until all processes are done
    while (!q.empty()) {
        int i = q.front(); // Pick the front process
        q.pop(); // Remove it from the queue

        // Process the selected task
        if (rem_bt[i] > 0) {
            // Check if process can fully run in the given quantum
            if (rem_bt[i] > quantum) {
                t += quantum;
                rem_bt[i] -= quantum;

                // Add newly arrived processes to the queue
                for (int j = 0; j < n; j++) {
                    if (at[j] <= t && rem_bt[j] > 0 && !inQueue[j]) {
                        q.push(j);
                        inQueue[j] = true;
                    }
                }
                q.push(i); // Re-add the current process to the queue for the next round
            } else {
                // Process finishes within the quantum
                t += rem_bt[i];
                wt[i] = t - bt[i] - at[i];
                rem_bt[i] = 0; // Mark process as completed
            }
        }

        // Add any newly arrived processes to the queue after processing
        for (int j = 0; j < n; j++) {
            if (at[j] <= t && rem_bt[j] > 0 && !inQueue[j]) {
                q.push(j);
                inQueue[j] = true;
            }
        }
    }
}

// Function to calculate turn around time
void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[])
{
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];
}

// Function to calculate average time
void findavgTime(int processes[], int n, int bt[], int at[], int quantum)
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    // Find waiting time of all processes
    findWaitingTime(processes, n, bt, wt, at, quantum);

    // Find turn around time for all processes
    findTurnAroundTime(processes, n, bt, wt, tat);

    // Display processes along with all details
    cout << "PN\tAT\tBT\tWT\tTAT\n";

    // Calculate total waiting time and total turn around time
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        cout << " " << i + 1 << "\t" << at[i] << "\t" << bt[i] << "\t" << wt[i] << "\t" << tat[i] << endl;
    }

}

// Driver code
int main()
{
    // Process IDs
    int processes[] = {1, 2, 3};
    int n = sizeof(processes) / sizeof(processes[0]);

    // Burst time of all processes
    int burst_time[] = {4, 1, 6};

    // Arrival time of all processes
    int arrival_time[] = {0, 1, 2};

    // Time quantum
    int quantum = 2;
    findavgTime(processes, n, burst_time, arrival_time, quantum);
    return 0;
}