#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>

namespace Utils{
    // Helper function to generate all partitions of a set
    template <typename T>
    void generatePartitionsHelper(const std::vector<T>& set_to_partition,
                                std::vector<std::vector<T>> currentPartition,
                                std::vector<std::vector<std::vector<T>>>& allPartitions,
                                int index) {
        if (index == set_to_partition.size()) {
            allPartitions.push_back(currentPartition);
            return;
        }

        const T element = set_to_partition[index];
        const int current_partition_size_before = currentPartition.size();
        // Try to add the current element to existing subsets
        for (int i = 0; i < current_partition_size_before; ++i) {
            auto& subset = currentPartition[i];
            subset.push_back(element);
            generatePartitionsHelper(set_to_partition, currentPartition, allPartitions, index + 1);
            subset.pop_back();
        }

        // Try to create a new subset with the current element
        currentPartition.push_back({element});
        generatePartitionsHelper(set_to_partition, currentPartition, allPartitions, index + 1);
        currentPartition.pop_back();
    }

    // Wrapper function to generate all partitions
    template <typename T>
    std::vector<std::vector<std::vector<T>>> generatePartitions(const std::vector<T>& set_to_partition) {
        std::vector<std::vector<std::vector<T>>> partitions;
        std::vector<std::vector<T>> currentPartition;
        generatePartitionsHelper(set_to_partition, currentPartition, partitions, 0);
        return partitions;
    }
}

#endif