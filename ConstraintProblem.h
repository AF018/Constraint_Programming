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
	// Parameters
	bool arc_consistency_activated;
	bool forward_check_activated;
	bool random_visit_order;
	bool visit_small_domains;
	bool visit_large_domains;
	// Elements used during the optimization
	bool inconsistent_instantiation;
	vector<bool> instantiated_vars;
public:
	// Default constructor
	ConstraintProblem();
	// Default destructor
	virtual ~ConstraintProblem();

	// Creates a queen problem
	void createQueenProblem(int const & n);
	// Creates a graph coloring problem with the graph contained in the file
	void createColorationProblem(string const & filename, int const & color_attempt_nb);
	// Applies parameters for the optimization
	void applyParameters(vector<int> const & parameters_vect);
	
	// Check if pair (a,b) is acceptable for variables (x,y)
	bool checkConstraint(int const & x, int const & y, int const & a, int const & b) const;
	// Add values to the domains
	void addValues(vector<pair<int, int> > const & values_to_add);
	// Removes a value from the domain of a variable
	void removeDomainValue(const  int & var_i, vector<int>::iterator const & value_i_it);

	// Performs forward check
	void forwardCheck(vector<pair<int, int> > & all_deleted_values, vector<int> part_inst, int const & var_idx);

	// Establishes arc consistency (AC4 algorithm)
	void initializationAC4(vector<pair<int, int> > & ac_deleted_values, vector<int> & support_count,
						   vector<vector<pair<int, int> > > & support_values);
	void AC4(vector<pair<int, int> > & ac_deleted_values);

	// Modify the visit order
	void initialVisitOrder(vector<int> & visit_order_vect);
	void alterVisitOrder(vector<int> & visit_order_vect, int const & current_idx);
	// Backtrack algorithm
	vector<int> backtrackSolve();
	// Backtrack recursive function
	bool recursiveBacktrack(vector<int> & visit_order_vect, int const & var_idx, vector<int> & partial_instantiation);
};

