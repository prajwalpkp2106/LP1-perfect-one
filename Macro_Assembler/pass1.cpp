#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <algorithm> // For std::remove_if

using namespace std;

int main() {
    // Opening input and output files
    ifstream br("input.txt");  // input file for macro definitions and code
    ofstream mnt("mnt.txt");   // output file for Macro Name Table
    ofstream mdt("mdt.txt");   // output file for Macro Definition Table
    ofstream kpdt("kpdt.txt"); // output file for Keyword Parameter Default Table
    ofstream pnt("pntab.txt"); // output file for Parameter Name Table
    ofstream ir("intermediate.txt"); // output file for intermediate code

    // Check if all files opened successfully
    if (!br.is_open() || !mnt.is_open() || !mdt.is_open() || !kpdt.is_open() || !pnt.is_open() || !ir.is_open()) {
        cerr << "Error opening one or more files!" << endl;
        return 1;
    }

    // Variable initialization
    unordered_map<string, int> pntab; // stores parameters with their indices
    string line;                       // current line being processed
    string Macroname;                  // name of the current macro
    int mdtp = 1, kpdtp = 0, paramNo = 1, pp = 0, kp = 0, flag = 0;

    // Process each line in the input file
    while (getline(br, line)) {
        istringstream iss(line);
        vector<string> parts((istream_iterator<string>(iss)), istream_iterator<string>());

        // Check if the line starts with "MACRO"
        if (parts[0] == "MACRO" || parts[0] == "macro") {
            flag = 1;  // set flag indicating that we are inside a macro definition
            if (!getline(br, line)) break;  // read next line containing macro name and parameters

            istringstream iss2(line);
            vector<string> parts2((istream_iterator<string>(iss2)), istream_iterator<string>());
            Macroname = parts2[0];  // first token is the macro name

            // If there are no parameters, write directly to MNT
            if (parts2.size() <= 1) {
                mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
                continue;
            }

            // Process each parameter for the macro
            for (size_t i = 1; i < parts2.size(); ++i) {
                string param = parts2[i];
                // Remove '&' and ',' characters from parameter
                param.erase(remove_if(param.begin(), param.end(), [](char c) { return c == '&' || c == ','; }), param.end());

                // Check if the parameter is a keyword parameter (contains '=')
                if (param.find('=') != string::npos) {
                    ++kp;  // Increment keyword parameter count
                    size_t pos = param.find('=');  // find position of '=' in the parameter
                    string keywordParam = param.substr(0, pos);  // extract the keyword parameter name
                    string value = param.substr(pos + 1);  // extract default value
                    pntab[keywordParam] = paramNo++;  // assign index to parameter in PNTAB
                    kpdt << keywordParam << "\t" << value << "\n";  // write keyword parameter to KPDT
                } else {
                    pntab[param] = paramNo++;  // assign index to positional parameter in PNTAB
                    ++pp;  // Increment positional parameter count
                }
            }

            // Write macro information to MNT
            mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
            kpdtp += kp;  // Update KPDT pointer
            kp = 0;       // Reset keyword parameter count
            pp = 0;       // Reset positional parameter count

        } else if (parts[0] == "MEND" || parts[0] == "mend") {
            mdt << line << "\n";  // Write MEND to MDT, ending the macro definition
            flag = kp = pp = 0;   // Reset all flags and counters for the next macro
            ++mdtp;               // Move MDT pointer to next line
            paramNo = 1;          // Reset parameter number for PNTAB

            // Write parameter names to PNTAB file
            pnt << Macroname << ":\t";
            for (const auto& pair : pntab) {
                pnt << pair.first << "\t";
            }
            pnt << "\n";
            pntab.clear();  // Clear PNTAB for next macro

        } else if (flag == 1) { // Inside the macro definition, process macro statements
            for (const auto& part : parts) {
                // Check if part is a parameter (contains '&')
                if (part.find('&') != string::npos) {
                    string param = part;
                    param.erase(remove_if(param.begin(), param.end(), [](char c) { return c == '&' || c == ','; }), param.end());
                    mdt << "(P," << pntab[param] << ")\t";  // Replace parameter with its index in MDT
                } else {
                    mdt << part << "\t";  // Write other tokens as they are in MDT
                }
            }
            mdt << "\n";  // New line in MDT
            ++mdtp;  // Increment MDT pointer
        } else {
            // Not inside a macro; write the line to intermediate file
            ir << line << "\n";
        }
    }

    // Close all file streams
    br.close();
    mnt.close();
    mdt.close();
    kpdt.close();
    pnt.close();
    ir.close();

    cout << "Macro Pass1 Processing done. :)" << endl;
    return 0;
}
