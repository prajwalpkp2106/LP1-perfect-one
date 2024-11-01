#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class MemoryPlacement
{
    vector<int> blockSize;
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

    // Allocation functions (firstFit, bestFit, nextFit, worstFit) without calling input()

    void firstFit()
    {
        input();
        vector<int> allocation(procesSize.size(), -1);

        for (int i = 0; i < procesSize.size(); i++)
        {
            for (int j = 0; j < blockSize.size(); j++)
            {
                if (blockSize[j] >= procesSize[i])
                {
                    allocation[i] = j;
                    blockSize[j] -= procesSize[i];
                    break;
                }
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

    void bestFit()
    {
        input();

        vector<int> allocation(procesSize.size(), -1);

        for (int i = 0; i < procesSize.size(); i++)
        {
            int bestIdx = -1;
            for (int j = 0; j < blockSize.size(); j++)
            {
                if (blockSize[j] >= procesSize[i])
                {
                    if (bestIdx == -1)
                        bestIdx = j;
                    else if (blockSize[bestIdx] > blockSize[j])
                        bestIdx = j;
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

        for (int i = 0; i < procesSize.size(); i++)
        {
            while (j < blockSize.size())
            {
                if (blockSize[j] >= procesSize[i])
                {
                    allocation[i] = j;
                    blockSize[j] -= procesSize[i];

                    t = (j - 1) % blockSize.size();
                    break;
                }
                if (t == j)
                {
                    t = (j - 1) % blockSize.size();
                    break;
                }
                j = (j + 1) % blockSize.size();
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
                    else if (blockSize[wstIdx] < blockSize[j])
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
