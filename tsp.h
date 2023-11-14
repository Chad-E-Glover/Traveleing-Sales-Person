#ifndef TSP_H
#define TSP_H

#include <vector>

typedef std::vector<std::vector<int>> MAP;
void backtracking_branch_bound_best_first_aux(
    MAP const &m,
    int const &N,
    int const &index,
    std::vector<int> &solution_so_far,
    int &cost_solution_so_far,
    std::vector<int> &best_solution_so_far,
    int &cost_best_solution_so_far,
    int parent_id);

std::vector<int> SolveTSP(char const *filename);
#endif
