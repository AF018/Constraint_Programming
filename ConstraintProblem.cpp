#include "ConstraintProblem.h"


ConstraintProblem::ConstraintProblem()
{
	var_nb = 0;
	var_domains = vector<vector<int> >();
	constraints = vector<vector<pair<int, int> > >();
}

ConstraintProblem::ConstraintProblem(int const & n)
{
	var_nb = n;
	// Constructing domains
	for (int i = 0; i < n; i++)
	{
		var_domains.push_back(vector<int>());
		for (int j = 0; j < n; j++)
		{
			var_domains[i].push_back(j);
		}
	}
	constraints = vector<vector<pair<int, int> > >();
	// Constructing constraints
	bool accepted_values = false;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			constraints.push_back(vector<pair<int, int> >());
			if (i != j)
			{
				//cout << "size : " << constraints.size() << "  i,j  " << i << " " << j << endl;
				
				// Filling constraints
				for (int i_value = 0; i_value < n; i_value++)
				{
					for (int j_value = 0; j_value < n; j_value++)
					{
						accepted_values = (i_value != j_value) && (i + i_value != j + j_value) && (i - i_value != j - j_value);
						if (accepted_values)
						{
							constraints[j + (i * n)].push_back(std::make_pair(i_value, j_value));
						}
					}
				}
				/*
				std::cout << "    ";
				for (int k = 0; k < constraints[j + i * n].size(); k++)
				{
					std::cout << "(" << constraints[j + (i * n)][k].first << "," << constraints[j + (i * n)][k].second << ")    ";
				}
				cout << endl;
				*/
			}
		}
	}
}

ConstraintProblem::~ConstraintProblem()
{
}

vector<int> ConstraintProblem::backtrackSolve()
{
	vector<int> visit_order_vect;
	for (int idx = 0; idx < var_nb; idx++)
	{
		visit_order_vect.push_back(idx);
	}
	for (vector<int>::iterator value_it = var_domains[visit_order_vect[0]].begin();
		value_it != var_domains[visit_order_vect[0]].end(); value_it++)
	{
		vector<int> instantiation(1, *value_it);
		if (recursiveBacktrack(visit_order_vect, 0, instantiation))
		{
			// A solution has been found
			return instantiation;
		}
	}
	cout << "No solution found" << endl;
}

bool ConstraintProblem::recursiveBacktrack(vector<int> const & visit_order_vect, int const & var_idx, vector<int> & partial_instantiation)
{
	// WARNING : pay attention to the var indices, different from the vect indices

	// Checks if the partial instantiation is correct, more precisely if the last added value does not create conflicts
	vector<pair<int, int> >::iterator pair_it;
	for (int vect_idx = 0; vect_idx < var_idx; vect_idx++)
	{
		// The elements in the pair are not commutative, pay attention to that
		pair_it = find(constraints[visit_order_vect[vect_idx] + (visit_order_vect[var_idx] * var_nb)].begin(),
					  constraints[visit_order_vect[vect_idx] + (visit_order_vect[var_idx] * var_nb)].end(),
					  make_pair(partial_instantiation[var_idx], partial_instantiation[vect_idx]));
		if (pair_it == constraints[visit_order_vect[vect_idx] + (visit_order_vect[var_idx] * var_nb)].end())
		{
			// Did not find the right pair in the constraints, the partial solution is not legit
			return false;
		}
	}

	/*
	cout << "values : ";
	for (auto it : partial_instantiation)
	{
		cout << it << "  ";
	}
	cout << endl;
	*/

	// If the constraints are all satisfied and all variables have been assigned values
	if (var_idx + 1 == var_nb)
	{
		return true;
	}

	// Trying to assign the next variable
	int explored_idx = var_idx + 1;
	for (auto domain_value : var_domains[explored_idx])
	{
		partial_instantiation.push_back(domain_value);
		if (recursiveBacktrack(visit_order_vect, explored_idx, partial_instantiation))
		{
			return true;
		}
		else
		{
			partial_instantiation.pop_back();
		}
	}

	return false;
}