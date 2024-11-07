// This code is an implementation of different memory allocation algorithms for managing memory in an operating system or any application that needs efficient memory utilization. Here’s a breakdown of the key components and functions
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class MemoryPlacement
{
    vector<int> blockSize; // A vector representing the sizes of available memory blocks.
    vector<int> procesSize;

public:
    void input()
    {
        int bs, ps;
        cout << "Enter block array size:";
        cin >> bs;
        cout << "Enter process array size:";
        cin >> ps;

        blockSize.resize(bs);
        procesSize.resize(ps);

        cout << "Enter block array:" << endl;
        for (int i = 0; i < bs; i++)
        {
            cout << "Block Size:";
            cin >> blockSize[i];
        }
        cout << "Enter process array:" << endl;
        for (int i = 0; i < ps; i++)
        {
            cout << "Process Size:";
            cin >> procesSize[i];
        }
    }

    void firstFit()
    {
        input();
        vector<int> allocation(procesSize.size(), -1); // Initializes an allocation vector to store the block index assigned to each process (-1 means unallocated)

        for (int i = 0; i < procesSize.size(); i++)
        {
            for (int j = 0; j < blockSize.size(); j++) // loops through each block and checks if it can fit the process
            {
                if (blockSize[j] >= procesSize[i]) // If a block is found with enough space, the process is allocated to that block, and the block size is reduced by the size of the process
                {
                    allocation[i] = j;
                    blockSize[j] -= procesSize[i];
                    break;
                }
            }
        }
        // Prints the process allocation table, showing the block number or "Not Allocated" for each process.
        cout << "\nProcess No.\tProcess Size\tBlock no." << endl;
        for (int i = 0; i < procesSize.size(); i++)
        {
            cout << i + 1 << "\t\t" << procesSize[i] << "\t\t";
            if (allocation[i] != -1)
                cout << allocation[i] + 1;
            else
                cout << "Not Allocated";
            cout << endl;
        }
    }
    // The Best Fit algorithm searches for the smallest memory block that can fit each process, which helps minimize fragmentation by reducing the remaining space left after allocation.
    void bestFit()
    {
        input();

        vector<int> allocation(procesSize.size(), -1);

        for (int i = 0; i < procesSize.size(); i++)
        {
            int bestIdx = -1;// stores the index of the best-fitting block (smallest block that can still fit the process). Setting it to -1 indicates that no suitable block has been found for the process yet
            for (int j = 0; j < blockSize.size(); j++) // Loops through each block to find the one with the smallest size that can still fit the process
            {
                if (blockSize[j] >= procesSize[i]) // checks if the current block (blockSize[j]) has enough space for the current process (procesSize[i])
                {
                    if (bestIdx == -1)
                        bestIdx = j;
                    else if (blockSize[bestIdx] > blockSize[j]) // checks if the current block (blockSize[j]) is a smaller fit than the previously assigned best block
                        bestIdx = j;                            // If so, bestIdx is updated to j to use the smaller block
                }
            }
            if (bestIdx != -1)
            {
                allocation[i] = bestIdx;
                blockSize[bestIdx] -= procesSize[i];
            }
        }

        cout << "\nProcess No.\tProcess Size\tBlock no." << endl;
        for (int i = 0; i < procesSize.size(); i++)
        {
            cout << i + 1 << "\t\t" << procesSize[i] << "\t\t";
            if (allocation[i] != -1)
                cout << allocation[i] + 1;
            else
                cout << "Not Allocated";
            cout << endl;
        }
    }

    void nextFit()
    {
        input();

        vector<int> allocation(procesSize.size(), -1);

        int j = 0, t = blockSize.size() - 1;
        // Sets j to start from the last allocated block and uses a t tracker to identify if the entire block list has been checked
        for (int i = 0; i < procesSize.size(); i++) // This loop iterates over each process in procesSize. For each process, it looks for a memory block that can fit the process, starting from the last allocated block.
        {
            while (j < blockSize.size()) // Inner Loop (while loop): This loop iterates over memory blocks starting from the j index. It checks each block in sequence and wraps around to the start if necessary
            {
                if (blockSize[j] >= procesSize[i])
                {
                    allocation[i] = j;
                    blockSize[j] -= procesSize[i];

                    t = (j - 1) % blockSize.size(); // setting the next position to search from on the next iteration.
                    break;
                }
                if (t == j) // checks if the allocation has returned to the initial block position (t), meaning no suitable blocks were found for the current process.
                // If this condition is met, it breaks the loop and moves on to the next process without allocating a block to the current process
                {
                    t = (j - 1 + blockSize.size()) % blockSize.size();
                    break;
                }
                j = (j + 1) % blockSize.size();//increments j to move to the next block
            }
        }

        cout << "\nProcess No.\tProcess Size\tBlock no." << endl;
        for (int i = 0; i < procesSize.size(); i++)
        {
            cout << i + 1 << "\t\t" << procesSize[i] << "\t\t";
            if (allocation[i] != -1)
                cout << allocation[i] + 1;
            else
                cout << "Not Allocated";
            cout << endl;
        }
    }

    // copy of best fit but instead of smallest block we use largest block
    void worstFit()
    {
        input();

        vector<int> allocation(procesSize.size(), -1);

        for (int i = 0; i < procesSize.size(); i++)
        {
            int wstIdx = -1;
            for (int j = 0; j < blockSize.size(); j++)
            {
                if (blockSize[j] >= procesSize[i])
                {
                    if (wstIdx == -1)
                        wstIdx = j;
                    else if (blockSize[wstIdx] < blockSize[j])//bas < sign kar
                        wstIdx = j;
                }
            }
            if (wstIdx != -1)
            {
                allocation[i] = wstIdx;
                blockSize[wstIdx] -= procesSize[i];
            }
        }

        cout << "\nProcess No.\tProcess Size\tBlock no." << endl;
        for (int i = 0; i < procesSize.size(); i++)
        {
            cout << i + 1 << "\t\t" << procesSize[i] << "\t\t";
            if (allocation[i] != -1)
                cout << allocation[i] + 1;
            else
                cout << "Not Allocated";
            cout << endl;
        }
    }
};

int main()
{
    MemoryPlacement mp;

    bool repeat = true;
    int choice;

    while (repeat)
    {
        cout << "### MENU ###" << endl;
        cout << "1. First Fit" << endl;
        cout << "2. Best Fit" << endl;
        cout << "3. Next Fit" << endl;
        cout << "4. Worst Fit" << endl;
        cout << "5. Exit" << endl;
        cout << endl;
        cout << "Enter your choice:";
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            cout << "First Fit Algorithm" << endl;
            mp.firstFit();
            break;

        case 2:
            cout << "Best Fit ALgorithm" << endl;
            mp.bestFit();
            break;

        case 3:
            cout << "Next Fit Algorithm" << endl;
            mp.nextFit();
            break;

        case 4:
            cout << "Worst Fit Algorithm" << endl;
            mp.worstFit();
            break;

        default:
            cout << "Exiting the code" << endl;
            repeat = false;
            break;
        }
    }

    return 0;
}
