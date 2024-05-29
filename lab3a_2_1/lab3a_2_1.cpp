#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>

struct Record {
    int id;
    int priority;
    std::string messageType;
};

std::vector<Record> g_arr;
int statemenu;

void menu() {
    std::cout << "Choose mode:" << std::endl
        << "0. Exit from the program" << std::endl
        << "1. Demonstration mode" << std::endl
        << "2. Benchmark mode" << std::endl;

    std::cin >> statemenu;
}

void showArray(const std::vector<Record>& arr) {
    for (const auto& record : arr) {
        std::cout << "ID: " << record.id << ", Priority: " << record.priority << ", Message Type: " << record.messageType << std::endl;
    }
    std::cout << "\n";
}

void fillArrayWithRandomRecords(std::vector<Record>& arr, int size) {
    arr.clear();
    std::vector<std::string> messageTypes = { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };
    for (int i = 0; i < size; ++i) {
        arr.push_back({ rand() % 1000, rand() % 101, messageTypes[rand() % messageTypes.size()] });
    }
}

void bubbleSort(std::vector<Record>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        for (size_t j = arr.size() - 1; j > i; j--) {
            if (arr[j].id < arr[j - 1].id)
                std::swap(arr[j], arr[j - 1]);
        }
    }
}

int partition(std::vector<Record>& arr, int low, int high) {
    Record pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].id <= pivot.id) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(std::vector<Record>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(std::vector<Record>& arr, int const left, int const mid, int const right) {
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    std::vector<Record> leftArray(subArrayOne);
    std::vector<Record> rightArray(subArrayTwo);

    for (int i = 0; i < subArrayOne; i++)
        leftArray[i] = arr[left + i];
    for (int j = 0; j < subArrayTwo; j++)
        rightArray[j] = arr[mid + 1 + j];

    int indexOfSubArrayOne = 0;
    int indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne].id <= rightArray[indexOfSubArrayTwo].id) {
            arr[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            arr[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        arr[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        arr[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(std::vector<Record>& arr, int const begin, int const end) {
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    mergeSort(arr, begin, mid);
    mergeSort(arr, mid + 1, end);
    merge(arr, begin, mid, end);
}

void combSort(std::vector<Record>& arr, int low, int high, int threshold) {
    if (high - low + 1 >= threshold)
        quickSort(arr, low, high);
    else
        bubbleSort(arr);
}

void librSort(std::vector<Record>& arr) {
    std::sort(arr.begin(), arr.end(), [](const Record& a, const Record& b) { return a.id < b.id; });
}

void countingSortByMessageType(std::vector<Record>& arr) {
    std::vector<std::string> messageTypes = { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };
    std::unordered_map<std::string, int> messageTypeIndex;
    for (size_t i = 0; i < messageTypes.size(); ++i) {
        messageTypeIndex[messageTypes[i]] = static_cast<int>(i);
    }

    int k = static_cast<int>(messageTypes.size());
    std::vector<int> count(k, 0);
    std::vector<Record> output(arr.size());

    for (auto& record : arr) {
        count[messageTypeIndex[record.messageType]]++;
    }

    for (int i = 1; i < k; ++i) {
        count[i] += count[i - 1];
    }

    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; i--) {
        output[count[messageTypeIndex[arr[i].messageType]] - 1] = arr[i];
        count[messageTypeIndex[arr[i].messageType]]--;
    }

    arr = output;
}

int getMaxPriority(const std::vector<Record>& arr) {
    int max = arr[0].priority;
    for (const auto& record : arr) {
        if (record.priority > max)
            max = record.priority;
    }
    return max;
}

void countingSortForRadix(std::vector<Record>& arr, int exp) {
    int n = static_cast<int>(arr.size());
    std::vector<Record> output(n);
    int count[10] = { 0 };

    for (int i = 0; i < n; i++) {
        count[(arr[i].priority / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i].priority / exp) % 10] - 1] = arr[i];
        count[(arr[i].priority / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radixSortByPriority(std::vector<Record>& arr) {
    int m = getMaxPriority(arr);
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countingSortForRadix(arr, exp);
    }
}

// Сортування за довільним полем або комбінацією полів
void sortByFields(std::vector<Record>& arr) {
    std::sort(arr.begin(), arr.end(), [](const Record& a, const Record& b) {
        if (a.messageType == b.messageType) {
            return a.priority < b.priority;
        }
        return a.messageType < b.messageType;
        });
}

void demonstrationMode() {
    std::cout << "---Demonstration mode---" << std::endl;

    std::cout << "Enter array size: ";
    int size;
    std::cin >> size;

    fillArrayWithRandomRecords(g_arr, size);

    std::cout << "Original array:" << std::endl;
    showArray(g_arr);
    std::cout << std::endl;

    auto arr_copy = g_arr;
    bubbleSort(arr_copy);
    std::cout << "Array after bubble sorting:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    quickSort(arr_copy, 0, static_cast<int>(arr_copy.size()) - 1);
    std::cout << "Array after quick sorting:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    mergeSort(arr_copy, 0, static_cast<int>(arr_copy.size()) - 1);
    std::cout << "Array after merge sorting:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    combSort(arr_copy, 0, static_cast<int>(arr_copy.size()) - 1, 10); // default threshold = 10
    std::cout << "Array after combining sorting:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    librSort(arr_copy);
    std::cout << "Array after library sorting:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    countingSortByMessageType(arr_copy);
    std::cout << "Array after counting sort by message type:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    radixSortByPriority(arr_copy);
    std::cout << "Array after radix sort by priority:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;

    arr_copy = g_arr;
    sortByFields(arr_copy);
    std::cout << "Array after sorting by messageType and priority:" << std::endl;
    showArray(arr_copy);
    std::cout << std::endl;
}

void benchmarkMode() {
    std::cout << "---Benchmark mode---" << std::endl;

    const int sizes[] = { 100, 1000, 10000 };
    const int thresholds[] = { 10, 20, 50 };

    for (int size : sizes) {
        std::cout << "Array size: " << size << std::endl;

        // Random array
        fillArrayWithRandomRecords(g_arr, size);

        auto arrCopy = g_arr;
        auto start = std::chrono::high_resolution_clock::now();
        bubbleSort(arrCopy);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Bubble Sort took: " << elapsed.count() << " seconds.\n";

        arrCopy = g_arr;
        start = std::chrono::high_resolution_clock::now();
        quickSort(arrCopy, 0, static_cast<int>(arrCopy.size()) - 1);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Quick Sort took: " << elapsed.count() << " seconds.\n";

        arrCopy = g_arr;
        start = std::chrono::high_resolution_clock::now();
        mergeSort(arrCopy, 0, static_cast<int>(arrCopy.size()) - 1);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Merge Sort took: " << elapsed.count() << " seconds.\n";

        for (int threshold : thresholds) {
            arrCopy = g_arr;
            start = std::chrono::high_resolution_clock::now();
            combSort(arrCopy, 0, static_cast<int>(arrCopy.size()) - 1, threshold);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "Comb Sort with threshold " << threshold << " took: " << elapsed.count() << " seconds.\n";
        }

        arrCopy = g_arr;
        start = std::chrono::high_resolution_clock::now();
        librSort(arrCopy);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "std::sort took: " << elapsed.count() << " seconds.\n";

        arrCopy = g_arr;
        start = std::chrono::high_resolution_clock::now();
        countingSortByMessageType(arrCopy);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Counting Sort by message type took: " << elapsed.count() << " seconds.\n";

        arrCopy = g_arr;
        start = std::chrono::high_resolution_clock::now();
        radixSortByPriority(arrCopy);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Radix Sort by priority took: " << elapsed.count() << " seconds.\n";

        arrCopy = g_arr;
        start = std::chrono::high_resolution_clock::now();
        sortByFields(arrCopy);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Sort by messageType and priority took: " << elapsed.count() << " seconds.\n";
    }
}

int main() {
    menu();
    switch (statemenu) {
    case 0:
        std::cout << "Exiting";
        break;
    case 1:
        demonstrationMode();
        break;
    case 2:
        benchmarkMode();
        break;
    default:
        std::cout << "Invalid option selected. Try again" << std::endl;
    }

    return 0;
}
