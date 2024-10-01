#include <iostream>
#include <vector>
#include <glpk.h>
#include "linear_program_construction.h"

// Function to remove intersection elements from both vectors
void remove_intersection_from_both(std::vector<int>& v1, std::vector<int>& v2) {
    // Step 1: Create a lambda to remove elements from a vector based on the other vector
    auto remove_elements = [](std::vector<int>& vec, const std::vector<int>& other_vec) {
        vec.erase(std::remove_if(vec.begin(), vec.end(),
                                 [&other_vec](int element) {
                                     return std::find(other_vec.begin(), other_vec.end(), element) != other_vec.end();
                                 }),
                  vec.end());
    };

    // Step 2: Find and remove elements that are common in both vectors
    std::vector<int> intersection;
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(intersection));

    // Step 3: Remove the intersection from both vectors
    remove_elements(v1, intersection);
    remove_elements(v2, intersection);
}

std::pair<double, std::vector<double>> getLinearProgramSolutionAndMinimizer(std::vector<double> objective_coeffs, std::vector<std::pair<std::vector<int>, std::vector<int>>> constraint_pairs, std::vector<int> degree_equation) {
    // Create the GLPK problem object
    glp_prob *lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MAX); // Set the objective to maximize

    // Dynamic vectors for coefficients and constraints
    std::vector<double> rhs;
    rhs.reserve(constraint_pairs.size());
    // Load the constraint matrix
    std::vector<int> ia = {0}; // Row indices
    std::vector<int> ja = {0}; // Column indices
    std::vector<double> ar = {0.0}; // Values
    int constraints_used = 0;
    for (int i=0; i<constraint_pairs.size(); i++){
        auto& pos = constraint_pairs[i].first;
        auto& neg = constraint_pairs[i].second;
        remove_intersection_from_both(pos, neg);
        for (int i=0; i<pos.size(); i++){
            pos[i]++;
        }
        for (int i=0; i<neg.size(); i++){
            neg[i]++;
        }
        if (neg.size() + pos.size() == 0) { continue; }
        constraints_used++;
        ar.insert(ar.end(), pos.size(), 1.0);
        ar.insert(ar.end(), neg.size(), -1.0);
        rhs.push_back(0.0);
        ia.insert(ia.end(), pos.size()+neg.size(), i + 1);
        ja.insert(ja.end(), pos.begin(), pos.end());
        ja.insert(ja.end(), neg.begin(), neg.end());
    }
    for (int i=0; i<degree_equation.size(); i++){
        degree_equation[i]++;
    }
    ia.insert(ia.end(), degree_equation.size(), constraints_used + 1);
    ja.insert(ja.end(), degree_equation.begin(), degree_equation.end());
    ar.insert(ar.end(), degree_equation.size(), 1.0);
    // Define the number of variables and constraints
    int numVars = objective_coeffs.size();
    int numConstraints = constraints_used;

    // Add variables
    glp_add_cols(lp, numVars);
    for (int i = 1; i <= numVars; ++i) {
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0); // Lower bound: 0
        glp_set_obj_coef(lp, i, objective_coeffs[i - 1]); // Set objective coefficients
    }

    // Add constraints
    glp_add_rows(lp, numConstraints + 1); // +1 for degree equation
    for (int i = 1; i <= numConstraints; ++i) {
        glp_set_row_bnds(lp, i, GLP_FX, rhs[i - 1], rhs[i - 1]); // Equality constraints
    }
    glp_set_row_bnds(lp, numConstraints + 1, GLP_FX, 1.0, 1.0); // Equality constraint for dgree

    // Load the matrix into GLPK
    glp_load_matrix(lp, ia.size() - 1, ia.data(), ja.data(), ar.data());

    // Solve the linear program
    glp_simplex(lp, nullptr);

    // Output results
    const double obj_value = glp_get_obj_val(lp);
    std::cout << "Objective value: " << obj_value << std::endl;
    std::vector<double> solution;
    const bool verbose = false;
    for (int i = 1; i <= numVars; ++i) {
        solution.push_back(glp_get_col_prim(lp, i));
        if (verbose) std::cout << "Graph x" << i << " coefficient: " << solution[i-1] << std::endl;
    }

    // Cleanup
    glp_delete_prob(lp);
    return std::make_pair(obj_value, solution);
}

