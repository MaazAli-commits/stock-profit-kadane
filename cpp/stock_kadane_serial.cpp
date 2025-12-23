#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main() {

    ifstream file("../data/stock_data_new_2.csv");
    if (!file) {
        cout << "File not found!" << endl;
        return 1;
    }

    string line;
    vector<string> dates;
    vector<double> prices;

    getline(file, line);
    getline(file, line);
    getline(file, line);

    while (getline(file, line)) {
        string date = "", priceStr = "";
        bool commaFound = false;

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') { commaFound = true; continue; }
            if (!commaFound) date += line[i];
            else priceStr += line[i];
        }

        if (!commaFound || priceStr.empty()) continue;

        double price = stod(priceStr);
        dates.push_back(date);
        prices.push_back(price);
    }

    file.close();

    if (prices.size() < 2) {
        cout << "Not enough data." << endl;
        return 0;
    }

    vector<double> arr;
    for (int i = 1; i < prices.size(); i++)
        arr.push_back(prices[i] - prices[i - 1]);

    double start_time = omp_get_wtime();

    double sum = 0.0, maxi = -1e18;
    int start = 0, end = 0, tempStart = 0;

    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i];
        if (sum > maxi) {
            maxi = sum;
            start = tempStart;
            end = i;
        }
        if (sum < 0) {
            sum = 0.0;
            tempStart = i + 1;
        }
    }

    double end_time = omp_get_wtime();
    
    int buyIndex = start;
    int sellIndex = end + 1;
    if (sellIndex >= prices.size()) sellIndex = prices.size() - 1;

    cout << fixed << setprecision(3);

    cout << "Maximum Profit: " << maxi << endl;
    cout << "Buy Date: " << dates[buyIndex] << " (Price: " << prices[buyIndex] << ")\n";
    cout << "Sell Date: " << dates[sellIndex] << " (Price: " << prices[sellIndex] << ")\n";
    cout << fixed << setprecision(9);
    cout << "Execution Time: " << (end_time - start_time) << " s  (" 
     << (end_time - start_time) * 1000.0 << " ms)" << endl;

    return 0;
}





