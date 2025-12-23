<h1>Stock Profit Analysis using Kadane’s Algorithm</h1>

<p>
This project analyzes historical stock price data to determine the maximum profit
that could be achieved from a single buy–sell transaction. The implementation combines
financial data extraction using Python with algorithmic optimization and parallelization
in C++.
</p>

<hr>

<h2>Data Collection</h2>

<p>
Stock price data is fetched using the <b>yfinance</b> Python library, which provides
easy access to historical market data from Yahoo Finance.
</p>

<p>
A Python script is used to:
</p>

<ul>
  <li>Download historical stock prices for a given ticker (e.g., AAPL)</li>
  <li>Extract the closing prices</li>
  <li>Store the data as a CSV file for further processing</li>
</ul>

<p>
This separation allows the data acquisition step to remain independent of the
performance-critical C++ implementation.
</p>

<hr>

<h2>Problem Formulation</h2>

<p>
Given a time series of daily closing prices, the goal is to compute the maximum profit
possible using exactly one buy operation followed by one sell operation, where the
buy date must occur before the sell date.
</p>

<p>
To simplify the problem, daily price differences are computed. This transforms the
problem into finding a contiguous subarray with the maximum sum.
</p>

<hr>

<h2>Serial Implementation (Kadane’s Algorithm)</h2>

<p>
The serial solution is based on Kadane’s Algorithm, which efficiently finds the maximum
subarray sum in linear time.
</p>

<p>
The algorithm iterates over the array of daily price differences while maintaining:
</p>

<ul>
  <li>The current running sum</li>
  <li>The maximum sum observed so far</li>
  <li>The corresponding buy and sell indices</li>
</ul>

<p>
If the running sum becomes negative, it is reset, as extending a negative-sum subarray
cannot improve the result.
</p>

<p>
This approach runs in <b>O(n)</b> time and serves as the baseline for performance comparison.
</p>

<hr>

<h2>Parallel Implementation using OpenMP</h2>

<p>
The serial Kadane’s Algorithm is extended to a parallel implementation using
<b>OpenMP</b>, enabling the computation to utilize multiple CPU cores.
</p>

<p>
The array of daily price differences is divided into fixed-size chunks, with
each chunk processed independently by a separate OpenMP thread.
</p>

<p>
For each chunk, the following values are computed in parallel:
</p>

<ul>
  <li>Maximum subarray sum within the chunk</li>
  <li>Prefix maximum sum</li>
  <li>Suffix maximum sum</li>
  <li>Total sum of the chunk</li>
</ul>

<p>
OpenMP pragmas are used to parallelize the computation across threads while
safely reducing local maximum values.
</p>

<p>
After the parallel region, the chunk-level results are combined to correctly
handle subarrays that span multiple chunks. This ensures correctness while
achieving parallel speedup.
</p>

<p>
This approach demonstrates how a traditionally sequential algorithm can be
adapted for parallel execution using OpenMP.
</p>


<hr>

<h2>Project Structure</h2>

<pre>
stock-profit-kadane/
├── cpp/        C++ source files (serial and parallel implementations)
├── data/       CSV file containing stock price data
├── python/     Python script for data extraction using yfinance
├── .gitignore
└── README.md
</pre>

<hr>

<h2>Notes</h2>

<ul>
  <li>
    On Windows, the serial execution time may appear as 0 ms due to coarse timer
    resolution for very fast executions.
  </li>
  <li>
    On Linux, finer-grained timing results in small non-zero execution times.
  </li>
  <li>
    Execution times are measured using <code>omp_get_wtime()</code>.
  </li>
</ul>

<hr>

<h2>Technologies Used</h2>

<ul>
  <li>C++ with OpenMP</li>
  <li>Python with yfinance</li>
</ul>

<hr>

<p>
This project demonstrates the integration of financial data analysis with algorithmic
optimization and parallel programming concepts.
</p>



