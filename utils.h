#pragma once

#include "ConstraintProblem.h"

// Reads the parameters off the parameters.txt file
vector<int> readParameters(string const & filename);

// Function used to check solutions to the queen problem
bool checkSolution(vector<int> const & solution);