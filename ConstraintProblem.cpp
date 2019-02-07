#include "ConstraintProblem.h"


ConstraintProblem::ConstraintProblem()
{
	var_nb = 0;
	domain_bound = 0;
	var_domains = vector<vector<int> >();
	constraints = vector<vector<bool> >();
	inconsistent_instantiation = false;
	instantiated_vars = vector<bool>();

	deleted_values = vector<pair<int, int> >();
	support_count = map<tuple<int, int, int>, int>();
	support_values = map < pair<int, int>, vector<pair<int, int> > >();
}

ConstraintProblem::ConstraintProblem(int const & n)
{
	var_nb = n;
	domain_bound = n;
	inconsistent_instantiation = false;
	instantiated_vars = vector<bool>(var_nb, false);
	deleted_values = vector<pair<int, int> >();
	support_count = map<tuple<int, int, int>, int>();
	support_values = map < pair<int, int>, vector<pair<int, int> > >();
	// Constructing domains
	for (int i = 0; i < n; i++)
	{
		var_domains.push_back(vector<int>());
		for (int j = 0; j < n; j++)
		{
			var_domains[i].push_back(j);
		}
	}
	constraints = vector<vector<bool> >();
	// Constructing constraints
	bool accepted_values = false;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			constraints.push_back(vector<bool>(domain_bound*domain_bound, false));
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
							constraints[(i * n)+j][(i_value*domain_bound)+j_value] = true;
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

bool ConstraintProblem::checkConstraint(int const & x, int const & y, int const & a, int const & b) const
{
	if (a > domain_bound || b > domain_bound)
	{
		return false;
	}
	return constraints[(x*var_nb)+y][(a*domain_bound)+b];
}

void ConstraintProblem::addValues(vector<pair<int, int>> const & values_to_add)
{
	// This assumes the values are not in the domains yet
	for (auto value_pair : values_to_add)
	{
		var_domains[value_pair.first].push_back(value_pair.second);
	}
}

void ConstraintProblem::initializationAC4()
{
	deleted_values = vector<pair<int, int> >();
	support_count = map<tuple<int, int, int>, int>();
	support_values = map < pair<int, int>, vector<pair<int, int> > >();
	
	// Going through the constraints
	for (int var_i = 0; var_i < var_nb; var_i++)
	{
		for (int var_j = 0; var_j < var_nb; var_j++)
		{
			if (var_i != var_j)
			{
				int pairs_count = 0;
				for (vector<int>::iterator value_i_it = var_domains[var_i].begin(); 
					value_i_it != var_domains[var_i].end(); value_i_it++)
				{
					pairs_count = 0;
					for (auto value_j : var_domains[var_j])
					{
						if (checkConstraint(var_i, var_j, *value_i_it, value_j))
						{
							// The pair (value_i, value_j) is acceptable for the variables (var_i, var_j)
							pairs_count++;
							// Adding a value to the support of (var_j, value_j)
							auto support_it = support_values.find(std::make_pair(var_j, value_j));
							if (support_it != support_values.end())
							{
								support_it->second.push_back(std::make_pair(var_i, *value_i_it));
							}
							else
							{
								support_values[std::make_pair(var_j, value_j)]
									= vector<pair<int, int> >(1, std::make_pair(var_i, *value_i_it));
							}
						}
					}
					if (pairs_count == 0)
					{
						//cout << "Deleting val " << *value_i_it << " with var " << var_i << " because of var " << var_j << endl;
						deleted_values.push_back(std::make_pair(var_i, *value_i_it));
						var_domains[var_i].erase(value_i_it);
						value_i_it--;
						if (var_domains[var_i].size() == 0)
						{
							inconsistent_instantiation = true;
							return;
						}
					}
					else
					{
						support_count[std::make_tuple(var_i, var_j, *value_i_it)] = pairs_count;
					}
				}
			}
		}
	}
}

vector<pair<int, int> > ConstraintProblem::AC4()
{
	vector<pair<int, int> > all_deleted_values = deleted_values;
	//cout << "SIZE : " << deleted_values.size() << endl;
	while (deleted_values.size() > 0)
	{
		// Popping the last deleted value out of the container
		pair<int, int> deleted_pair = deleted_values.back();
		deleted_values.pop_back();

		map < pair<int, int>, vector<pair<int, int> > >::iterator paired_value_vect_it = support_values.find(deleted_pair);
		if (paired_value_vect_it != support_values.end())
		{
			for (auto paired_value : paired_value_vect_it->second)
			{
				// Making sure the paired variable is not instantiated yet, it is useless otherwise
				if (true)//not instantiated_vars[paired_value.first])
				{
					support_count[std::make_tuple(paired_value.first, deleted_pair.first, paired_value.second)] += -1;
					// If the paired value has no support value left : propagate
					if (support_count[std::make_tuple(paired_value.first, deleted_pair.first, paired_value.second)] == 0)
					{
						vector<int>::iterator value_it = std::find(var_domains[paired_value.first].begin(),
							var_domains[paired_value.first].end(),
							paired_value.second);
						if (value_it != var_domains[paired_value.first].end())
						{
							var_domains[paired_value.first].erase(value_it);
							deleted_values.push_back(paired_value);
							// Keeping all the deleted values to add them back in case the backtrack goes back
							all_deleted_values.push_back(paired_value);
							if (var_domains[paired_value.first].size() == 0)
							{
								inconsistent_instantiation = true;
								return all_deleted_values;
							}

							//cout << "Deleted value with propagation " << paired_value.first << "," << paired_value.second << endl;
						}
					}
				}
			}

		}
		else
		{
			//cout << deleted_pair.first << "," << deleted_pair.second << "n'a aucune valeur support" << endl;
		}
	}

	return all_deleted_values;
}

vector<int> ConstraintProblem::backtrackSolve()
{
	vector<int> visit_order_vect;
	for (int idx = 0; idx < var_nb; idx++)
	{
		visit_order_vect.push_back(idx);
	}

	vector<int> former_var_domain = var_domains[visit_order_vect[0]];
	for (auto domain_value : former_var_domain)
	{
		var_domains[visit_order_vect[0]] = vector<int>(1, domain_value);
		instantiated_vars[visit_order_vect[0]] = true;
		vector<int> instantiation(1, domain_value);
		if (recursiveBacktrack(visit_order_vect, 0, instantiation))
		{
			// A solution has been found
			return instantiation;
		}
	}
	var_domains[visit_order_vect[0]] = former_var_domain;
	instantiated_vars[visit_order_vect[0]] = false;
	cout << "No solution found" << endl;
}

bool ConstraintProblem::recursiveBacktrack(vector<int> const & visit_order_vect, int const & var_idx, vector<int> & partial_instantiation)
{
	// WARNING : pay attention to the var indices, different from the vect indices
	
	//cout << "Exploring var idx " << var_idx << " with value " << partial_instantiation.back() << endl;

	// Checks if the partial instantiation is correct, more precisely if the last added value does not create conflicts
	vector<pair<int, int> >::iterator pair_it;
	for (int vect_idx = 0; vect_idx < var_idx; vect_idx++)
	{
		bool constraint_respected = checkConstraint(visit_order_vect[var_idx], visit_order_vect[vect_idx],
			partial_instantiation[var_idx], partial_instantiation[vect_idx]);
		if (not constraint_respected)
		{
			// The partial solution is not legit
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

	//for (int idx_1 = 0; idx_1 < var_domains.size(); idx_1++)
	//{
	//	cout << "befor domain var " << idx_1 << " : ";
	//	for (auto it_2 : var_domains[idx_1])
	//	{
	//		cout << it_2 << "  ";
	//	}
	//	cout << endl;
	//}

	vector<pair<int, int> > ac_deleted_values;
	if (true)
	{
		initializationAC4();
		if (inconsistent_instantiation)
		{
			inconsistent_instantiation = false;
			addValues(deleted_values);
			return false;
		}
		ac_deleted_values = AC4();
		if (inconsistent_instantiation)
		{
			inconsistent_instantiation = false;
			addValues(ac_deleted_values);
			return false;
		}

		// Verifying each variable not instantiated has at least one value left in its domain
		//for (int left_var_idx = var_idx + 1; left_var_idx < var_nb; left_var_idx++)
		//{
		//	if (var_domains[visit_order_vect[left_var_idx]].size() == 0)
		//	{
		//		//cout << "no domain for var : " << visit_order_vect[left_var_idx] << endl;
		//		addValues(ac_deleted_values);
		//		return false;
		//	}
		//}

	}

	//for (int idx_1=0; idx_1<var_domains.size(); idx_1++)
	//{
	//	cout << "after domain var " << idx_1 << " : ";
	//	for (auto it_2 : var_domains[idx_1])
	//	{
	//		cout << it_2 << "  ";
	//	}
	//	cout << endl;
	//}

	// Trying to assign the next variable
	int explored_idx = var_idx + 1;
	instantiated_vars[visit_order_vect[explored_idx]] = true;
	vector<int> former_var_domain = var_domains[visit_order_vect[explored_idx]];
	for (auto domain_value : former_var_domain)
	{
		var_domains[visit_order_vect[explored_idx]] = vector<int>(1, domain_value);
		partial_instantiation.push_back(domain_value);
		bool found_solution = recursiveBacktrack(visit_order_vect, explored_idx, partial_instantiation);
		if (found_solution)
		{
			addValues(ac_deleted_values);
			return true;
		}
		else
		{
			partial_instantiation.pop_back();
		}
	}
	addValues(ac_deleted_values);
	var_domains[visit_order_vect[explored_idx]] = former_var_domain;
	instantiated_vars[visit_order_vect[explored_idx]] = false;
	return false;
}