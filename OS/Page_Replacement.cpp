#include <iostream>
#include <vector>
using namespace std;

class PageReplacementAlgorithm
{
private:
    int n;              // Length of page reference sequence
    int nf;             // Number of frames
    vector<int> in;     // Vector holding the page reference sequence.
    vector<int> p;      //  Vector representing frames currently in use.
    int pgfaultcnt = 0; // Counter for page faults (when a requested page is not found in any of the frames).

    void initialize()
    {
        pgfaultcnt = 0;
        p.assign(nf, 9999); // Initializes the frames p by setting each entry to 9999 (an arbitrary placeholder value meaning empty or unused).
    }

    bool isHit(int data) // Checks if a requested page data is already in one of the frames (p vector)
    {
        for (int j = 0; j < nf; j++)
        {
            if (p[j] == data)
                return true;
        }
        return false;
    }

    void dispPages()
    {
        for (int k = 0; k < nf; k++)
        {
            if (p[k] != 9999) // Displays the current pages in the frames (p vector), skipping any that still contain the placeholder 9999
                cout << " " << p[k];
        }
    }

    void dispPgFaultCnt() // Displays the total number of page faults that occurred during the execution of the algorithm.
    {
        cout << "Total no of page faults: " << pgfaultcnt << endl;
    }

public:
    void getData()
    {
        cout << "Enter length of page reference sequence: ";
        cin >> n;
        cout << "Enter the page reference sequence: ";
        in.resize(n);
        for (int i = 0; i < n; i++)
            cin >> in[i];
        cout << "Enter no of frames: ";
        cin >> nf;
        p.resize(nf);
        cout << endl;
    }

    void fifo()
    {
        initialize();
        for (int i = 0; i < n; i++)
        {
            cout << "For " << in[i] << " :";
            if (!isHit(in[i]))
            {
                for (int k = 0; k < nf - 1; k++)//shift all frames one position to the left, effectively removing the oldest page.
                    p[k] = p[k + 1];
                p[nf - 1] = in[i];//Place the new page at the end of the frames, increase the page fault count, and display the frames.  
                pgfaultcnt++;
                dispPages();
            }
            else
                cout << "No page fault";
            cout << endl;
        }
        dispPgFaultCnt();
        cout << endl;
    }

//Replace the page that will not be needed for the longest time in the future.
    void optimal()
    {
        initialize();
        vector<int> near(nf, 0);//used to store the index of the page in the future.
        for (int i = 0; i < n; i++)
        {
            cout << "For " << in[i] << " :";
            if (!isHit(in[i]))//If the page in[i] is not already in the frames
            {
                for (int j = 0; j < nf; j++)// Finding the Next Use of Each Page in Frames  
                {
                    int pg = p[j];
                    bool found = false;
                    for (int k = i; k < n; k++)
                    {
                        if (pg == in[k])//Checks if pg appears again in the page reference sequence in, starting from the current position i
                        {
                            near[j] = k;//If pg is found in the future (at position k), the index k is stored in near[j] to indicate when pg will next be used.
                            found = true;
                            break;
                        }
                    }
                    if (!found)//If pg does not appear again, near[j] is set to 9999, marking pg as not needed in the future.
                        near[j] = 9999;
                }
                //Choosing the Page to Replace
                int max = -9999;
                int repindex;
                //The code searches the near vector for the page with the highest next use index (max). The rationale is that the page with the farthest next use (or not needed at all) should be replaced
                for (int j = 0; j < nf; j++)
                {
                    if (near[j] > max)
                    {
                        max = near[j];
                        repindex = j;//repindex stores the index of the frame containing the page to replace
                    }
                }
                p[repindex] = in[i];//The page in the frame at repindex is replaced with the current page in[i]
                pgfaultcnt++;
                dispPages();
            }
            else
                cout << "No page fault";
            cout << endl;
        }
        dispPgFaultCnt();
        cout << endl;
    }
// Logic: Replace the page that was least recently used
// same as optimal but instead of max we use min
    void lru()
    {
        initialize();
        vector<int> least(nf, 0);
        for (int i = 0; i < n; i++)
        {
            cout << "For " << in[i] << " :";
            if (!isHit(in[i]))
            {
                for (int j = 0; j < nf; j++)
                {
                    int pg = p[j];
                    bool found = false;
                    for (int k = i - 1; k >= 0; k--)
                    {
                        if (pg == in[k])
                        {
                            least[j] = k;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        least[j] = -9999;
                }
                int min = 9999;
                int repindex;
                for (int j = 0; j < nf; j++)
                {
                    if (least[j] < min)
                    {
                        min = least[j];
                        repindex = j;
                    }
                }
                p[repindex] = in[i];
                pgfaultcnt++;
                dispPages();
            }
            else
                cout << "No page fault!";
            cout << endl;
        }
        dispPgFaultCnt();
        cout << endl;
    }
};

int main()
{
    PageReplacementAlgorithm prAlgo;
    int choice;
    while (true)
    {
        cout << " ***** Page Replacement Algorithms ***** " << endl;
        cout << "1. Enter data" << endl;
        cout << "2. FIFO" << endl;
        cout << "3. Optimal" << endl;
        cout << "4. LRU" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        switch (choice)
        {
        case 1:
            prAlgo.getData();
            break;
        case 2:
            cout << "#1# FIFO Algorithm" << endl;
            prAlgo.fifo();
            break;
        case 3:
            cout << "#2# Optimal Algorithm" << endl;
            prAlgo.optimal();
            break;
        case 4:
            cout << "#3# LRU Algorithm" << endl;
            prAlgo.lru();
            break;
        default:
            return 0;
            break;
        }
    }
}
