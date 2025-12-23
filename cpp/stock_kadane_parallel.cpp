#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

double parallelMaxSubarraySum(vector<double>& arr, int numThreads) {
    int n = arr.size();
    int chunkSize = (n + numThreads - 1) / numThreads;

    vector<double> prefix_max(numThreads);
    vector<double> suffix_max(numThreads);
    vector<double> total_sum(numThreads);
    double globalLocalMax = -1e18;

    #pragma omp parallel reduction(max:globalLocalMax)
    {
        #pragma omp for schedule(static)
        for (int t = 0; t < numThreads; t++) {
            int start = t * chunkSize;
            int end = min(n - 1, (t + 1) * chunkSize - 1);
            if (start >= n) continue;

            double sum = 0.0, localMax = -1e18;

            // Local max within chunk
            for (int i = start; i <= end; i++) {
                sum += arr[i];
                if (sum > localMax) localMax = sum;
                if (sum < 0) sum = 0.0;
            }
            globalLocalMax = max(globalLocalMax, localMax);

            // Prefix max
            sum = 0.0; double preMax = -1e18;
            for (int i = start; i <= end; i++) {
                sum += arr[i];
                if (sum > preMax) preMax = sum;
            }
            prefix_max[t] = preMax;

            // Suffix max
            sum = 0.0; double sufMax = -1e18;
            for (int i = end; i >= start; i--) {
                sum += arr[i];
                if (sum > sufMax) sufMax = sum;
            }
            suffix_max[t] = sufMax;

            // Total sum
            sum = 0.0;
            for (int i = start; i <= end; i++) sum += arr[i];
            total_sum[t] = sum;
        }
    }

    // Combine cross-chunk results (multi-chunk safe)
    double maxSum = globalLocalMax;

    for (int i = 0; i < numThreads; i++) {
        for (int j = i + 1; j < numThreads; j++) {
            double crossSum = suffix_max[i] + prefix_max[j];
            for (int k = i + 1; k < j; k++) crossSum += total_sum[k];
            maxSum = max(maxSum, crossSum);
        }
    }

    return maxSum;
}

int main() {
    ifstream file("../data/stock_data_new_2.csv");
    if (!file) {
        cout << "File not found!" << endl;
        return 1;
    }

    string line;
    vector<string> dates;
    vector<double> prices;

    // Skip first 3 header lines
    getline(file, line);
    getline(file, line);
    getline(file, line);

    // Read data line by line
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

    cout << "Number of data rows: " << prices.size() << endl;

    // Compute daily differences
    vector<double> arr;
    for (int i = 1; i < prices.size(); i++)
        arr.push_back(prices[i] - prices[i - 1]);

    int numThreads = 6; // adjust as needed

    // --- Start timing using omp_get_wtime() ---
    double start_time = omp_get_wtime();

    double maxi = parallelMaxSubarraySum(arr, numThreads);

    double end_time = omp_get_wtime();
    double execTimeMs = (end_time - start_time) * 1000.0; // convert seconds → ms

    double sum = 0.0, best = -1e18;
    int start = 0, end = 0, tempStart = 0;

    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i];
        if (sum > best) {
            best = sum;
            start = tempStart;
            end = i;
        }
        if (sum < 0) {
            sum = 0.0;
            tempStart = i + 1;
        }
    }

    int buyIndex = start;
    int sellIndex = end + 1;
    if (sellIndex >= prices.size()) sellIndex = prices.size() - 1;

    cout << fixed << setprecision(3);
    cout << "Maximum Profit (Parallel): " << maxi << endl;
    cout << "Buy Date: " << dates[buyIndex] << " (Price: " << prices[buyIndex] << ")" << endl;
    cout << "Sell Date: " << dates[sellIndex] << " (Price: " << prices[sellIndex] << ")" << endl;
    cout << setprecision(9);
    cout << "Execution Time: " << (end_time - start_time) << " s  (" 
     << (end_time - start_time) * 1000.0 << " ms)" << endl;


    return 0;
}
