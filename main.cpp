#include "crack_safe.h"
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Enter the input file name as the first and only argument" << '\n';
        return 0;
    }
    CrackSafe crackSafe(argv[1]);
    int test_row = 0;
    int test_col = 0;
    std::string state = "right";

    // print execution time
    auto start = std::chrono::high_resolution_clock::now();
    crackSafe.openSafe(test_row, test_col, state);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution time: "
              << duration.count() * 1e-6 << " seconds" << std::endl;

    return 0;
}
