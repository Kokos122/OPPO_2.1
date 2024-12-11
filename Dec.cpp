#include <iostream> 
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
string getFileName(const string& s);
string getFileDate(const string& s);
int getFileSum(const string& s);

int main() {
    ifstream ist("file.txt");
    if (!ist) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<FileInfo> files;
    string line;
    while (getline(ist, line)) {
        try {
            files.push_back({ getFileName(line), getFileDate(line), getFileSum(line) });
        }
        catch (const exception& e) {
            cerr << "Error processing line: " << line << "\n" << e.what() << endl;
        }
    }

    // Вывод исходного списка 
    cout << "Original list:\n";
    for (const auto& file : files) {
        cout << file << "\n";
    }

    // Сортировка по дате (альтернативная реализация через компаратор) 
    auto dateComparator = [](const FileInfo& a, const FileInfo& b) {
        return a.date < b.date;
        };
    sort(files.begin(), files.end(), dateComparator);

    cout << "\nSorted by date:\n";
    for (const auto& file : files) {
        cout << file << "\n";
    }

    // Суммирование доходов (альтернативная реализация через явный цикл) 
    map<string, int> incomeSummary;
    for (const auto& file : files) {
        if (incomeSummary.find(file.name) == incomeSummary.end()) {
            incomeSummary[file.name] = 0;
        }
        incomeSummary[file.name] += file.sum;
    }

    // Нахождение топ-3 доходов (без промежуточного вектора) 
    multimap<int, string, greater<int>> sortedIncome;
    for (const auto& entry : incomeSummary) {
        sortedIncome.insert({ entry.second, entry.first });
    }

    cout << "\nTop-3 incomes:\n";
    size_t count = 0;
    for (const auto& entry : sortedIncome) {
        if (count++ == 3) break;
        cout << entry.second << ": " << entry.first << "\n";
    }

    return 0;
}

string getFileName(const string& s) {
    auto names = extractData(s, R"("[a-zA-Z0-9._-]+")");
    if (names.empty()) throw runtime_error("File name not found");
    return names[0];
}

string getFileDate(const string& s) {
    auto dates = extractData(s, R"(\d{2}\.\d{2}\.\d{4})");
    if (dates.size() != 1) throw runtime_error("Invalid or missing date");
    return dates[0];
}

int getFileSum(const string& s) {
    auto sums = extractData(s, R"(\b\d+\b)");
    if (sums.empty()) throw runtime_error("Sum not found");
    return stoi(sums.back());
}

vector<string> extractData(const string& data, const string& regex) {
    std::regex pattern(regex);
    sregex_iterator begin(data.begin(), data.end(), pattern), end;
    vector<string> result;
    for (auto it = begin; it != end; ++it) {
        result.push_back(it->str());
    }
    return result;
}

std::ostream& operator<<(std::ostream& stream, const FileInfo& fileInfo) {
    return stream << "Name: " << fileInfo.name
        << ", Date: " << fileInfo.date
        << ", Sum: " << fileInfo.sum;
}