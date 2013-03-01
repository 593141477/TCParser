#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

typedef vector<string> vstr;

string DATA_START_MARKER("<!-- System Testing -->");
string DATA_END_MARKER("<!-- End System Testing -->");

void error(string v)
{
    cerr<<v<<endl;
    exit(-1);
} 

vstr readLines()
{
    string str = "";
    vstr v;
    while(getline(cin, str))
        v.push_back(str);
    return v;
}

vstr getDataTable(const vstr& lines)
{
    vstr v;
    for(int i=0, seen=0; i<lines.size(); i++){
        if(lines[i] == DATA_START_MARKER){
            seen = 1;
            continue;
        }else if(lines[i] == DATA_END_MARKER)
            return v;

        if(seen)
            v.push_back(lines[i]);
    }
    error("Invalid format!");
}

vector<vstr> mergeRows(const vstr& dt)
{
    vector<vstr> ret;
    vstr row;

    for(int i=0, inrow=0; i<dt.size(); i++){
        if(dt[i].find("<TR valign=\"top\">") != string::npos
                || dt[i].find("<tr valign=\"top\">") != string::npos){
            if(inrow)
                error("Invalid format! Nested <tr> tags");
            inrow = 1;
            row.clear();
            continue;
        }else if(inrow && dt[i].find("</TR>") != string::npos
                    || inrow && dt[i].find("</tr>") != string::npos){
            inrow = 0;
            ret.push_back(row);
            continue;
        }

        if(inrow)
            row.push_back(dt[i]);
    }

    return ret;
}

template<typename T> 
vector<T> slice(const vector<T>& vec, int s, int e)
{
    return vec;
    if(s < 0) 
        s += vec.size();
    if(e < 0)
        e += vec.size();
    vector<T> ret;
    for(int i=s; i<=e; i++)
        ret.push_back(vec[i]);
    return ret;
}

string removeHTMLTags(const string& str)
{
    string s;
    int tags = 0;
    for(int i=0; i<str.length(); i++){
        if(str[i] == '<')
            tags++;
        else if(str[i] == '>')
            tags--;
        else if(tags == 0)
            s += str[i];
    }
    return s;
}

void removeHTMLTagsVec(vstr& lines)
{
    vstr ret;
    for(int i=0; i<lines.size(); i++){
        string inner = removeHTMLTags(lines[i]);
        if(inner != "")
            ret.push_back(inner);
    }
    lines = ret;
}

const char* filename(const char*fmt, int d)
{
    char buf[1024];
    sprintf(buf, fmt, d);
    return string(buf).c_str();
}

bool whiteString(const string& s)
{
    for(int i=0; i<s.length(); i++)
        if(!isspace(s[i]))
            return false;
    return true;
}

string trim(const string& s)
{
    string ret = "";
    for(int i = 0, seen = 0; i<s.length(); i++){
        if(!isspace(s[i]))
            seen = 1;
        if(seen)
            ret += s[i];
    }
    return ret;
}

void writeToFile(const vstr& lines, ofstream& in, ofstream& out)
{
    int i = 0;
    while(whiteString(lines[i]))
        i++;//Skip leading white strings
    do
        in<<trim(lines[i++])<<endl;
    while(!whiteString(lines[i]));


    while(whiteString(lines[i]))
        i++;//Skip white strings between inputs and outputs
    do
        out<<trim(lines[i++]);
    while(!whiteString(lines[i]));
    out<<endl;
}


int main()
{
    vector<vstr> dtrs = mergeRows(getDataTable(readLines()));
    cout<<dtrs.size()<<" Groups of data found"<<endl;
    for_each(dtrs.begin(), dtrs.end(), removeHTMLTagsVec);

    for(int i=0; i<dtrs.size(); i++){
        ofstream in(filename("%d.in", i)), out(filename("%d.out", i));
        writeToFile(dtrs[i], in, out);
        in.close();
        out.close();
    }
    return 0;
}
