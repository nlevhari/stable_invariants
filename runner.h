#ifndef runner_h
#define runner_h

#include <vector>

enum class PROBLEM_TYPE{
    SPI = 0,
    SPI_M = 1,
    SPI_NO_ORIGAMI = 2
};

double calculate_stable_invariant(const std::vector<int>& word, PROBLEM_TYPE problem_type, int r, int m = -1, bool display=false);
std::vector<int> generate_random_reduced_word(int length, int r);

#endif