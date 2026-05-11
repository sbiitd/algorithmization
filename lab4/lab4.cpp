#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <thread>
#include <future>

using namespace std;
using namespace chrono;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[(low + high) / 2]; 
    int i = low - 1;
    int j = high + 1;

    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}

void quick_sort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi);
        quick_sort(arr, pi + 1, high);
    }
}

void parallel_quick_sort(vector<int>& arr, int low, int high, int depth, int max_depth) {
    if (low >= high) return;

    if (depth >= max_depth || (high - low) < 10000) {
        quick_sort(arr, low, high);
        return;
    }

    int pi = partition(arr, low, high);

    future<void> left_future = async(launch::async, [&]() {
        parallel_quick_sort(arr, low, pi, depth + 1, max_depth);
        });
    future<void> right_future = async(launch::async, [&]() {
        parallel_quick_sort(arr, pi + 1, high, depth + 1, max_depth);
        });

    left_future.get();
    right_future.get();
}

void quick_sort_single(vector<int>& arr) {
    quick_sort(arr, 0, arr.size() - 1);
}

void quick_sort_parallel_2(vector<int>& arr) {
    parallel_quick_sort(arr, 0, arr.size() - 1, 0, 2);
}

void quick_sort_parallel_4(vector<int>& arr) {
    parallel_quick_sort(arr, 0, arr.size() - 1, 0, 4);
}

void quick_sort_parallel_8(vector<int>& arr) {
    parallel_quick_sort(arr, 0, arr.size() - 1, 0, 8);
}

double measure_time(void (*sort_func)(vector<int>&), vector<int> arr) {
    auto start = high_resolution_clock::now();
    sort_func(arr);
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    return elapsed.count();
}

vector<int> generate_random_array(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 100000);
    for (int i = 0; i < size; i++) {
        arr[i] = dist(gen);
    }
    return arr;
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<int> sizes = { 10000, 50000, 100000, 200000, 500000, 1000000 };

    cout << "Размер\tОбычная\t\t2 потока\t4 потока\t8 потоков" << endl;
    cout << "==========================================================================" << endl;

    for (int size : sizes) {
        cout << size << "\t";

        vector<int> arr = generate_random_array(size);

        double t1 = measure_time(quick_sort_single, arr);
        cout << t1 << "\t";

        double t2 = measure_time(quick_sort_parallel_2, arr);
        cout << t2 << "\t";

        double t4 = measure_time(quick_sort_parallel_4, arr);
        cout << t4 << "\t";

        double t8 = measure_time(quick_sort_parallel_8, arr);
        cout << t8 << endl;
    }

    return 0;
}