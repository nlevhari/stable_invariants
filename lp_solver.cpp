#include <iostream>
#include <vector>
#include <glpk.h>
#include "linear_program_construction.h"


#include <glpk.h>
#include <vector>
#include <iostream>

void print_glp_matrix(glp_prob *lp, int numVars, int numConstraints) {
    std::vector<std::vector<double>> A(numConstraints, std::vector<double>(numVars, 0.0));
    std::vector<double> b(numConstraints);
    std::vector<double> c(numVars);

    // Get the coefficient matrix A
    for (int i = 1; i <= numConstraints; ++i) {
        int len = glp_get_mat_row(lp, i, nullptr, nullptr);
        std::vector<int> indices(len + 1); // Store indices of non-zero entries
        std::vector<double> values(len + 1); // Store non-zero values
        glp_get_mat_row(lp, i, indices.data(), values.data());

        for (int j = 1; j <= len; ++j) {
            A[i - 1][indices[j] - 1] = values[j]; // Fill the matrix A
        }

        // Get the right-hand side vector b
        int row_type = glp_get_row_type(lp, i);
        if (row_type == GLP_FX) {
            // Fixed constraint (equality): lb == ub
            b[i - 1] = glp_get_row_lb(lp, i);
        } else if (row_type == GLP_UP) {
            // Upper bound constraint
            b[i - 1] = glp_get_row_ub(lp, i);
        } else if (row_type == GLP_LO) {
            // Lower bound constraint
            b[i - 1] = glp_get_row_lb(lp, i);
        } else if (row_type == GLP_DB) {
            // Double bounded constraint
            b[i - 1] = glp_get_row_ub(lp, i); // Or use lb based on your interpretation
        } else {
            std::cerr << "Unknown row type for constraint " << i << std::endl;
        }
    }

    // Get the objective function coefficients (vector c)
    for (int i = 1; i <= numVars; ++i) {
        c[i - 1] = glp_get_obj_coef(lp, i);
    }

    // Output the matrix A
    std::cout << "Matrix A:" << std::endl;
    for (const auto& row : A) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    // Output the RHS vector b
    std::cout << "Vector b:" << std::endl;
    for (double value : b) {
        std::cout << value << std::endl;
    }

    // Output the objective coefficients vector c
    std::cout << "Vector c (Objective function coefficients):" << std::endl;
    for (double value : c) {
        std::cout << value << std::endl;
    }
}

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
        ia.insert(ia.end(), pos.size()+neg.size(), constraints_used);
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

    glp_term_out(GLP_OFF);

    // Solve the linear program
    int status = glp_simplex(lp, nullptr);
    int solution_status = glp_get_status(lp);

    
    // Output results
    std::vector<double> solution;
    double obj_value;
    if (status == 0 && solution_status == GLP_OPT){
        obj_value = glp_get_obj_val(lp);
        const bool verbose = false;
        if (verbose) std::cout << "Objective value: " << obj_value << std::endl;
        for (int i = 1; i <= numVars; ++i) {
            solution.push_back(glp_get_col_prim(lp, i));
            if (verbose) std::cout << "Graph x" << i << " coefficient: " << solution[i-1] << std::endl;
        }
    } else {
        obj_value = -1;
    }

    // Cleanup
    glp_delete_prob(lp);
    return std::make_pair(obj_value, solution);
}

