#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class ConstraintProblem
{
	int var_nb;
	int domain_bound;
	vector<vector<int> > var_domains;
	vector<vector<bool> > constraints;
	vector<vector<int> > constrained_vars;
	// Elements used during the optimization
	bool inconsistent_instantiation;
	vector<bool> instantiated_vars;
	// Containers for the arc consistency
	vector<pair<int, int> > deleted_values;
	vector<int> support_count;
	vector<vector<pair<int, int> > > support_values;
public:
	// Default constructor
	ConstraintProblem();
	// Default destructor
	virtual ~ConstraintProblem();

	// Creates a queen problem
	void createQueenProblem(int const & n);
	// Creates a graph coloring problem with the graph contained in the file
	void createColorationProblem(string const & filename, int const & color_attempt_nb);
	
	// Check if pair (a,b) is acceptable for variables (x,y)
	bool checkConstraint(int const & x, int const & y, int const & a, int const & b) const;
	// Add values to the domains
	void addValues(vector<pair<int, int> > const & values_to_add);
	// Removes a value from the domain of a variable
	void removeDomainValue(const  int & var_i, vector<int>::iterator value_i_it);

	// Establishes arc consistency (AC4 algorithm)
	void initializationAC4();
	void AC4(vector<pair<int, int> > & ac_deleted_values);

	// Backtrack algorithm
	vector<int> backtrackSolve();
	// Backtrack recursive function
	bool recursiveBacktrack(vector<int> const & visit_order_vect, int const & var_idx, vector<int> & partial_instantiation);
};

