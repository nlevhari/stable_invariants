// main.cpp
#include "runner.h"
#include "whitehead_automorphisms.h"
#include <iostream>
#include <numeric>
#include <chrono>
#include <fstream>  // Include for file operations

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " mode [additional arguments]" << std::endl;
        std::cerr << "Modes:" << std::endl;
        std::cerr << "  single [problem_type] [r] [m (if required)] [word_as_integers]" << std::endl;
        std::cerr << "  stats [problem_type] [r] [m (if required)] [min_length] [max_length] [num_samples_per_length]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "single") {
        // Handle single word calculation
        if (argc < 5 || (PROBLEM_TYPE(std::stoi(argv[2])) == PROBLEM_TYPE::SPI_M && argc < 6)) {
            std::cerr << "Usage: " << argv[0] << " single [problem_type] [r] [m (if required)] [word_as_integers]" << std::endl;
            return 1;
        }

        const PROBLEM_TYPE problem_type = PROBLEM_TYPE(std::stoi(argv[2]));
        int arg_index = 3;

        // Parse the rank of the free group
        int r = std::stoi(argv[arg_index++]);

        int m = -1;
        if (problem_type == PROBLEM_TYPE::SPI_M) {
            m = std::stoi(argv[arg_index++]);
        }

        // Parse the word from command line arguments
        std::vector<int> word;
        for (int i = arg_index; i < argc; ++i) {
            word.push_back(std::stoi(argv[i]));
        }

        double result = calculate_stable_invariant(word, problem_type, r, m);
        if (result >= 0) {
            std::cout << "The stable primitivity rank is: " << result << std::endl;
        } else {
            std::cout << "Calculation failed or is not supported." << std::endl;
        }

    } else if (mode == "stats") {
        // Handle statistics calculation
        if (argc < 7 || (PROBLEM_TYPE(std::stoi(argv[2])) == PROBLEM_TYPE::SPI_M && argc < 8)) {
            std::cerr << "Usage: " << argv[0] << " stats [problem_type] [r] [m (if required)] [min_length] [max_length] [num_samples_per_length]" << std::endl;
            return 1;
        }

        const PROBLEM_TYPE problem_type = PROBLEM_TYPE(std::stoi(argv[2]));
        int arg_index = 3;

        // Parse the rank of the free group
        int r = std::stoi(argv[arg_index++]);

        int m = -1;
        if (problem_type == PROBLEM_TYPE::SPI_M) {
            m = std::stoi(argv[arg_index++]);
        }

        int min_length = std::stoi(argv[arg_index++]);
        int max_length = std::stoi(argv[arg_index++]);
        int num_samples = std::stoi(argv[arg_index++]);

        // Open the output file
        std::ofstream outfile("stats_output.txt");
        if (!outfile.is_open()) {
            std::cerr << "Error: Could not open 'stats_output.txt' for writing." << std::endl;
            return 1;
        }

        // Write a header for clarity
        outfile << "Statistics Output\n";
        outfile << "=================\n\n";

        // Pre-generate all Type II automorphisms once
        std::cout << "Generating Type II Whitehead automorphisms..." << std::endl;
        std::vector<Automorphism> automorphisms = generate_typeII(r);
        std::cout << "Generated " << automorphisms.size() << " Type II automorphisms.\n" << std::endl;

        // For each length in the interval, generate words and compute statistics
        for (int length = min_length; length <= max_length; ++length) {
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<double> results;
            int infty_counter = 0;
            std::vector<Word> infty_words; // To store words resulting in infinity

            for (int sample = 0; sample < num_samples; ++sample) {
                // Generate a random reduced word of the given length
                std::vector<int> word = generate_random_reduced_word(length, r);
                double result = calculate_stable_invariant(word, problem_type, r, m);
                if (result >= 0) {
                    results.push_back(result);
                } else {
                    infty_counter++;
                    infty_words.push_back(word);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            // Write statistics for this length to the file
            outfile << "Length " << length << ":\n";
            outfile << "--------------\n";

            if (!results.empty()) {
                double sum = std::accumulate(results.begin(), results.end(), 0.0);
                double mean = sum / results.size();
                double sq_sum = std::inner_product(results.begin(), results.end(), results.begin(), 0.0);
                double stdev = std::sqrt(sq_sum / results.size() - mean * mean);
                outfile << "Mean = " << mean << ", StdDev = " << stdev << ", Finite = " << results.size() << ", Infinite = " << infty_counter << "\n";
            } else {
                outfile << "No finite samples. All samples resulted in infinity.\n";
            }

            // Process and write 'infty' words
            if (!infty_words.empty()) {
                outfile << "\nWords Resulting in Infinity:\n";
                outfile << "----------------------------\n";
                for(const auto& word : infty_words){
                    // Convert word to a string representation
                    std::string word_str = "";
                    for(auto c : word){
                        word_str += std::to_string(c) + " ";
                    }

                    // Check if the word is minimal
                    bool minimal = is_minimal(word, r, automorphisms);

                    // If not minimal, find a minimal word
                    Word minimal_word;
                    if(!minimal){
                        minimal_word = get_minimal_word(word, r);
                    }

                    // Write the information to the file
                    outfile << "Word: " << word_str << "\n";
                    outfile << "Is Minimal: " << (minimal ? "Yes" : "No") << "\n";
                    if(!minimal){
                        // Convert minimal_word to string
                        std::string minimal_word_str = "";
                        for(auto c : minimal_word){
                            minimal_word_str += std::to_string(c) + " ";
                        }
                        outfile << "Minimal Word: " << minimal_word_str << "\n";
                    }
                    outfile << "\n";
                }
            }

            outfile << "Elapsed time for length " << length << ": " << elapsed.count() << " seconds\n";
            outfile << "========================================\n\n";

            // Optional: Print progress to console
            std::cout << "Processed length " << length << " in " << elapsed.count() << " seconds." << std::endl;
        }

        outfile.close();
        std::cout << "\nStatistics have been written to 'stats_output.txt'." << std::endl;

    } else {
        std::cerr << "Invalid mode. Use 'single' or 'stats'." << std::endl;
        return 1;
    }

    return 0;
}
