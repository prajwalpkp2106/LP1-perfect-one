#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class MNTEntry
{
public:
    string name;
    int pp, kp, mdtp, kpdtp;
    MNTEntry() : name(""), pp(0), kp(0), mdtp(0), kpdtp(0) {}
    MNTEntry(const string &name, int pp, int kp, int mdtp, int kpdtp)
        : name(name), pp(pp), kp(kp), mdtp(mdtp), kpdtp(kpdtp) {}
    // name: Macro name.
    // pp: Positional parameter count.
    // kp: Keyword parameter count.
    // mdtp: Macro definition table pointer.
    // kpdtp: Keyword parameter default table pointer.
    int getPp() const { return pp; }
    int getKp() const { return kp; }
    int getMdtp() const { return mdtp; }
    int getKpdtp() const { return kpdtp; }
};

int main()
{

    ifstream irb("intermediate.txt");
    ifstream mdtb("mdt.txt");
    ifstream kpdtb("kpdt.txt");
    ifstream mntb("mnt.txt");
    ofstream fr("pass2.txt");

    unordered_map<string, MNTEntry> mnt; // map storing each macro's metadata (MNTEntry object) keyed by the macro name.

    // Expanding Macros from intermediate.txt Argument Parameter Table
    unordered_map<int, string> aptab;        // aptab: Stores parameters by their position number.
    unordered_map<string, int> aptabInverse; // aptabInverse: Maps parameter names to their respective position numbers in aptab.

    vector<string> mdt;
    vector<string> kpdt;

    string line;

    // Reading MDT Tables
    while (getline(mdtb, line))
    {
        mdt.push_back(line); // lines are read and stored in the mdt vector
    }

    // Read KPDT file
    while (getline(kpdtb, line))
    {
        kpdt.push_back(line);
    }

    // Read MNT file
    while (getline(mntb, line)) // map storing each macro's metadata (MNTEntry object) keyed by the macro name.
    {
        istringstream iss(line);
        string parts[5];
        for (int i = 0; i < 5; ++i)
        {
            iss >> parts[i]; // all the element are individual stored in parts array
        }
        mnt[parts[0]] = MNTEntry(parts[0], stoi(parts[1]), stoi(parts[2]), stoi(parts[3]), stoi(parts[4]));
    }

    // Process intermedidate file
    // traversing Each Line in intermediate.txt:
    while (getline(irb, line))
    {
        istringstream iss(line);
        vector<string> parts;
        string part;
        while (iss >> part)
        {
            parts.push_back(part);
        }

        if (mnt.find(parts[0]) != mnt.end()) // parts[0] is the macro name. If the macro exists in mnt, the program proceeds with expansion.
        {
            const MNTEntry &entry = mnt[parts[0]]; // entry is the MNTEntry object from mnt table.
            int pp = entry.getPp();                // extracting the positional parameter count from the MNTEntry object.
            int kp = entry.getKp();
            int mdtp = entry.getMdtp();
            int kpdtp = entry.getKpdtp();
            int paramNo = 1;//for each paramaeter 

            // Process Parameters Eg-MACRO1 ARG1, ARG2, &OPTION=NEW_VAL
            for (int i = 0; i < pp; ++i)
            {
                if (paramNo < parts.size())//if parameter is present
                {
                    parts[paramNo] = parts[paramNo].substr(0, parts[paramNo].find(',')); // Removing any trailing commas.
                    aptab[paramNo] = parts[paramNo];                                     // Storing the parameter in aptab using paramNo as the key and the parameter name as the value.
                    aptabInverse[parts[paramNo]] = paramNo;                              // Storing the parameter name in aptabInverse using the parameter name as the key and paramNo as the value.
                    paramNo++;
                }
            }

            // mapping each keyword parameter to its default value if no explicit value is provided in the macro call.
            // Eg- in kpdt table
            // &OPTION1    DEFAULT1
            // &OPTION2    DEFAULT2
            // kpdt[0] corresponds to &OPTION1 DEFAULT1
            int j = kpdtp - 1;           // KPDT pointer starts at kpdtp - 1 (0-based indexing)
            for (int i = 0; i < kp; ++i) // Iterate over each keyword parameter
            {
                if (j >= 0 && j < kpdt.size())
                {                                // Ensure j is within the bounds of kpdt
                    istringstream kpss(kpdt[j]); // Create a stream for the line at kpdt[j] Eg-&OPTION1 DEFAULT1
                    vector<string> kpParts;
                    string kpPart;
                    while (getline(kpss, kpPart, '\t'))
                    {
                        kpParts.push_back(kpPart); // line is split by tabs into kpParts: kpParts = ["&OPTION1", "DEFAULT1"];
                    }
                    if (kpParts.size() > 1)
                    {
                        aptab[paramNo] = kpParts[1];        // Sets aptab[3] = "DEFAULT1".
                        aptabInverse[kpParts[0]] = paramNo; // Adds aptabInverse["OPTION1"] = 3.
                        j++;
                        paramNo++;
                    }
                }
            }

            // Processes any additional arguments for the macro, typically keyword parameters:
            for (int i = pp + 1; i < parts.size(); ++i)
            {
                parts[i] = parts[i].substr(0, parts[i].find(','));
                size_t eqPos = parts[i].find('='); // Finds the position of '=' in the argument
                if (eqPos != string::npos)
                {                                                                  // If '=' is found, split into name and value
                    string name = parts[i].substr(0, eqPos);                       // Extracts the name part (before '=')
                    string value = parts[i].substr(eqPos + 1);                     // Extracts the value part (after '=')
                    name.erase(remove(name.begin(), name.end(), '&'), name.end()); // Removes '&' prefix from name

                    if (aptabInverse.find(name) != aptabInverse.end())
                    {                                      // Checks if name exists in aptabInverse
                        aptab[aptabInverse[name]] = value; // Updates aptab with provided value, overwriting defaults if necessary
                    }
                }
            }

            // APTAB is fully populated with all parameter values, ready for macro expansion.

            // Process MDT-Expanding Macro Definition from MDT
            int i = mdtp - 1; // pointer to the start of the macro definition in MDT
            while (i < mdt.size() && mdt[i] != "MEND")
            {
                istringstream mdiss(mdt[i]); // for each line in MDT
                vector<string> splits;
                string split;
                while (mdiss >> split)
                {
                    splits.push_back(split);
                }

                fr << "+"; // The + sign is printed at the start of each expanded macro instruction to indicate that this is an expanded instruction line.

                // For each line, it checks if tokens match the parameter pattern (P,num).
                // If a match is found, retrieves the corresponding parameter from APTAB for replacement.
                for (const string &s : splits)
                {
                    if (s.find("(P,") != string::npos) // Each token s in splits is checked to see if it contains the pattern "(P,number)", which indicates a parameter reference.
                    {
                        string numStr;
                        for (char c : s) // It extracts the parameter number from the token and stores it in numStr.
                        {
                            if (isdigit(c))
                                numStr += c;
                        }
                        int num = stoi(numStr);
                        if (aptab.find(num) != aptab.end()) // It then looks up APTAB[num] to find the actual argument value and writes this value to fr (the output stream)
                        {
                            fr << aptab[num] << "\t";
                        }
                    }
                    else
                    {
                        fr << s << "\t"; // If s is not a parameter placeholder (e.g., a regular instruction or symbol), it is written directly to fr
                    }
                }
                fr << "\n"; // next line
                i++;        // Advance to Next MDT Line:
            }
            // Resetting APTAB for Next Macro Call
            aptab.clear();
            aptabInverse.clear();
        }
        else
        {
            fr << line << "\n";
        }
    }

    // Close files
    fr.close();
    mntb.close();
    mdtb.close();
    kpdtb.close();
    irb.close();

    cout << "Pass 2 processing done!" << endl;

    return 0;
}
