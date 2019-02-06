#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;

class ConstraintProblem
{
	int var_nb;
	vector<vector<int> > var_domains;
	vector<vector<pair<int, int> > > constraints;
	vector<bool> instantiated_vars;
	// Containers for the arc consistency
	vector<pair<int, int> > deleted_values;
	map<tuple<int, int, int>, int> support_count;
	map < pair<int, int>, vector<pair<int, int> > > support_values;
public:
	// Default constructor
	ConstraintProblem();
	// Constructor for the queens problem
	ConstraintProblem(int const & n);
	// Default destructor
	virtual ~ConstraintProblem();
	
	// Add values to the domains
	void addValues(vector<pair<int, int> > const & values_to_add);

	// Establishes arc consistency (AC4 algorithm)
	void initializationAC4();
	vector<pair<int, int> > AC4();

	// Backtrack algorithm
	vector<int> backtrackSolve();
	// Backtrack recursive function
	bool recursiveBacktrack(vector<int> const & visit_order_vect, int const & var_idx, vector<int> & partial_instantiation);
};

