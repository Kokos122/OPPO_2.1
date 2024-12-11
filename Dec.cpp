#include <iostream>
#include <clocale>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>
using namespace std;

struct FileInfo {
    string name;
    string date;
    int sum;
};

std::ostream& operator<<(std::ostream& stream, const FileInfo& fileInfo);
vector<string> extractData(const string& data, const string& regex);
string getFileName(string s);
string getFileDate(string s);
int getFileSum(string s);

int main() {
    ifstream ist("file.txt");
    if (!ist.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    vector<FileInfo> files;
    string s;
    while (getline(ist, s)) {
        files.push_back({ getFileName(s), getFileDate(s), getFileSum(s) });
    }
    for (const auto& item : files) {
        cout << item << "\n";
    }
    return 0;
}

string getFileName(string s) {
    auto name = extractData(s, R"("[a-zA-Z0-9._-]+")");
    if (name.empty()) {
        throw runtime_error("File name not found in the input line.");
    }
    return name.at(0);
}

string getFileDate(string s) {
    auto date = extractData(s, R"(\d{2}\.\d{2}\.\d{4})");
    if (date.size() != 1) {
        throw runtime_error("File date not found or invalid in the input line.");
    }
    return date.at(0);
}

int getFileSum(string s) {
    auto sum = extractData(s, R"(\b\d+\b)");
    if (sum.size() < 1) {
        throw runtime_error("File sum not found in the input line.");
    }
    return stoi(sum.back());
}

vector<string> extractData(const string& data, const string& regex) {
    std::regex words_regex(regex);
    auto words_begin = sregex_iterator(data.begin(), data.end(), words_regex);
    auto words_end = sregex_iterator();
    vector<string> out;
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        out.push_back(i->str());
    }
    return out;
}

std::ostream& operator<<(std::ostream& stream, const FileInfo& fileInfo) {
    stream << "Name: " << fileInfo.name;
    stream << ", Date: " << fileInfo.date;
    stream << ", Sum: " << fileInfo.sum;
    return stream;
}