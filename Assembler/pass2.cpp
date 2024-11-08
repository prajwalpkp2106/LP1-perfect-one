#include <bits/stdc++.h>
using namespace std;

string table(ifstream &inFile, const string x)
{
    inFile.clear();
    inFile.seekg(0,ios::beg);

    string ind,name,addr;
    while(inFile>>ind>>name>>addr)
    {
        if(ind==x)
        {
            return addr;
        }
    }
    return "NAN";
}

int main()
{
    ifstream ic,st,lt;

    ic.open("ic.txt");
    st.open("symtable.txt");
    lt.open("littable.txt");

    ofstream of;
    of.open("Output.txt");

    string lc;
    string s1,s2,s3;

    while(ic>>lc>>s1>>s2>>s3)
    {

        string s;

        if(s1.substr(1,2)=="AD" || s1=="(DL,02)")
        {
            s="---NO CODE---";
        }
        else if(s1=="(DL,01)")
        {
            s="00\t0\00"+s2.substr(3,1);       //00 0 001
        }
        else
        {
            //IS
            if(s1.substr(1,2)=="IS")
            {
                 s=s1.substr(4,2) + "\t0\t000";         //07 0 000
            }
            //Symbols
            else if(s2.substr(1,1)=="S")
            {
                s=s1.substr(4,2) + "\t0"+table(st,s2.substr(4,1));          //04 0 000 
            }
            else
            {
                //symbol 
                if(s3.substr(1,1)=="S")
                {
                    s=s1.substr(4,2) + s2.substr(1,1)+table(st,s2.substr(4,1));
                }
                //Literal
                else
                {
                    s=s1.substr(4,2) + s2.substr(1,1)+table(lt,s2.substr(4,1));
                }
            }
        }

        of<<lc<<"\t"<<s<<endl;
    }

    return 0;
}

