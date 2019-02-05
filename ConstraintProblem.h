#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class ConstraintProblem
{
	int var_nb;
	vector<vector<int> > var_domains;
	vector<vector<pair<int, int> > > constraints;
public:
	// Default constructor
	ConstraintProblem();
	// Constructor for the queens problem
	ConstraintProblem(int const & n);
	// Default destructor
	virtual ~ConstraintProblem();

	// Backtrack algorithm
	vector<int> backtrackSolve();
	// Backtrack recursive function
	bool recursiveBacktrack(vector<int> const & visit_order_vect, int const & var_idx, vector<int> & partial_instantiation);
};

