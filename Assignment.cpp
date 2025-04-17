#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <cmath>

using json = nlohmann::json;
using namespace std;

// Function to read JSON file
json readJsonFile(const string& filePath) {
    ifstream file(filePath);
    json data;
    file >> data;
    return data;
}

// Function to decode Y values
vector<pair<int, int>> decodeYValues(const json& data) {
    vector<pair<int, int>> decodedPoints;
    for (auto& [key, value] : data.items()) {
        if (isdigit(key[0])) {
            int x = stoi(key);
            int base = stoi(value["base"].get<string>());
            int y = stoi(value["value"].get<string>(), nullptr, base);
            decodedPoints.emplace_back(x, y);
        }
    }
    return decodedPoints;
}

// Function to perform Lagrange interpolation
double lagrangeInterpolation(const vector<pair<int, int>>& points, int xValue = 0) {
    int n = points.size();
    double result = 0.0;

    for (int j = 0; j < n; ++j) {
        double term = points[j].second;
        for (int m = 0; m < n; ++m) {
            if (m != j) {
                term *= (xValue - points[m].first) / static_cast<double>(points[j].first - points[m].first);
            }
        }
        result += term;
    }

    return result;
}

// Function to process test cases
vector<double> processTestCases(const json& data) {
    vector<double> results;
    for (auto& [testCase, value] : data.items()) {
        if (testCase == "keys") continue;
        auto decodedPoints = decodeYValues(value);
        double secretC = lagrangeInterpolation(decodedPoints);
        results.push_back(secretC);
    }
    return results;
}

int main() {
    string filePath = "test_cases.json"; // Replace with your JSON file path
    json data = readJsonFile(filePath);

    auto results = processTestCases(data);

    for (size_t i = 0; i < results.size(); ++i) {
        cout << "Test Case " << i + 1 << ": The constant term (secret c) is: " << results[i] << endl;
    }

    return 0;
}