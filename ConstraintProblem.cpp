#include "ConstraintProblem.h"


ConstraintProblem::ConstraintProblem()
{
	var_nb = 0;
	domain_bound = 0;
	var_domains = vector<vector<int> >();
	constraints = vector<vector<bool> >();
	constrained_vars = vector<bool>();
	// Parameters
	arc_consistency_activated = false;
	forward_check_activated = false;
	// Variables for the optimization
	inconsistent_instantiation = false;
	instantiated_vars = vector<bool>();
}

ConstraintProblem::~ConstraintProblem()
{
}

void ConstraintProblem::createQueenProblem(int const & n)
{
	var_nb = n;
	domain_bound = n;
	inconsistent_instantiation = false;
	instantiated_vars = vector<bool>(var_nb, false);

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
	constrained_vars = vector<bool>(var_nb*var_nb, false);
	// Constructing constraints
	bool accepted_values = false;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			constraints.push_back(vector<bool>(domain_bound*domain_bound, false));
			if (i != j)
			{
				constrained_vars[(i * n) + j] = true;
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

void ConstraintProblem::createColorationProblem(string const & filename, int const & color_attempt_nb)
{
	ifstream file_reader;
	file_reader.open(filename);

	if (!file_reader)
	{
		cout << "Unable to open : " << filename << endl;
		// call system to stop
		exit(1);   
	}

	int constraint_nb = 0;

	std::string line;
	bool found_data = false;
	while ((not found_data) && std::getline(file_reader, line))
	{
		if (line[0] == 'p')
		{
			found_data = true;
			size_t pos = 0;
			std::string delimiter = " ";
			std::string token;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + delimiter.length());
			}
			var_nb = std::stoi(token);
			constraint_nb = std::stoi(line);
		}
	}
	char readChar;
	int readInt;

	cout << "val : " << var_nb << " " << constraint_nb << endl;

	// Constructing constraints;
	constraints = vector<vector<bool> >(var_nb*var_nb, vector<bool>(color_attempt_nb*color_attempt_nb, false));
	constrained_vars = vector<bool>(var_nb*var_nb, false);
	int var_i;
	int var_j;
	for (int constraint_idx=0; constraint_idx<constraint_nb; constraint_idx++)
	{
		file_reader >> readChar >> var_i >> var_j;
		// Variable values are modified to fit in the interval [0, var_nb-1]
		var_i = var_i - 1;
		var_j = var_j - 1;
		constrained_vars[(var_i*var_nb) + var_j] = true;
		constrained_vars[(var_j*var_nb) + var_i] = true;
		for (int value_i = 0; value_i < color_attempt_nb; value_i++)
		{
			for (int value_j = 0; value_j < color_attempt_nb; value_j++)
			{
				if (value_i != value_j)
				{
					constraints[(var_i * var_nb) + var_j][(value_i*color_attempt_nb) + value_j] = true;
					constraints[(var_j * var_nb) + var_i][(value_j*color_attempt_nb) + value_i] = true;
				}
			}
		}
	}
	file_reader.close();
	
	domain_bound = color_attempt_nb;
	var_domains = vector<vector<int> >(var_nb, vector<int>());
	for (int var_i = 0; var_i < var_nb; var_i++)
	{
		for (int value_i = 0; value_i < color_attempt_nb; value_i++)
		{
			var_domains[var_i].push_back(value_i);
		}
	}

	inconsistent_instantiation = false;
	instantiated_vars = vector<bool>(var_nb, false);
}

void ConstraintProblem::applyParameters(vector<int> const & parameters_vect)
{
	if (parameters_vect[0] == 1)
	{ 
		arc_consistency_activated = true;
	}
	if (parameters_vect[1] == 1)
	{
		forward_check_activated = true;
	}
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

void ConstraintProblem::removeDomainValue(const int & var_i, vector<int>::iterator const & value_i_it)
{
	//var_domains[var_i].erase(value_i_it);
	vector<int>::iterator back_it = std::prev(var_domains[var_i].end());
	std::iter_swap(value_i_it, back_it);
	var_domains[var_i].pop_back();
}

void ConstraintProblem::forwardCheck(vector<pair<int, int>> & all_deleted_values, vector<int> part_inst)
{
	for (int var_i = 0; var_i < var_nb; var_i++)
	{
		if (instantiated_vars[var_i])
		{
			if (var_domains[var_i].size() != 1)
			{
				cout << "WARNING : the domain for " << var_i
					<< "does not have the right size : " << var_domains[var_i].size() << endl;
			}
			int value_i = var_domains[var_i][0];
			for (int var_j = 0; var_j < var_nb; var_j++)
			{
				if ((not instantiated_vars[var_j]) && constrained_vars[(var_i * var_nb) + var_j])
				{
					for (vector<int>::iterator value_j_it = var_domains[var_j].begin();
						value_j_it != var_domains[var_j].end(); value_j_it++)
					{
						//cout << " " << var_i << " " << var_j << " " << var_domains[var_j].size() << endl;
						if (not checkConstraint(var_i, var_j, value_i, *value_j_it))
						{
							//cout << "  " << var_i << " " << value_i << " " << var_j << " " << *value_j_it << endl;
							all_deleted_values.push_back(std::make_pair(var_j, *value_j_it));
							removeDomainValue(var_j, value_j_it);
							value_j_it--;
							if (var_domains[var_j].size() == 0)
							{

								//for (auto value : part_inst)
								//{
								//	cout << value << " ";
								//}
								//cout << "\t" << var_i << " " << var_j;
								//cout << endl;

								inconsistent_instantiation = true;
								return;
							}
						}
					}
				}
			}
		}
	}
	return;
}

void ConstraintProblem::initializationAC4(vector<pair<int, int> > & ac_deleted_values,
										  vector<int> & support_count, 
										  vector<vector<pair<int, int> > > & support_values)
{
	// Going through the constraints
	for (int var_i = 0; var_i < var_nb; var_i++)
	{
		for (int var_j = 0; var_j < var_nb; var_j++)
		{
			if (constrained_vars[(var_i * var_nb) + var_j])
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
							support_values[value_j + (domain_bound*var_j)].push_back(std::make_pair(var_i, *value_i_it));
						}
					}
					if (pairs_count == 0)
					{
						//cout << "Deleting val " << *value_i_it << " with var " << var_i << " because of var " << var_j << endl;
						ac_deleted_values.push_back(std::make_pair(var_i, *value_i_it));
						removeDomainValue(var_i, value_i_it);
						value_i_it--;
						if (var_domains[var_i].size() == 0)
						{
							inconsistent_instantiation = true;
							return;
						}
					}
					else
					{
						support_count[(*value_i_it) + (var_i*domain_bound) + (var_j*domain_bound*var_nb)] = pairs_count;
					}
				}
			}
		}
	}
}

void ConstraintProblem::AC4(vector<pair<int, int> > & all_deleted_values)
{
	vector<int> support_count = vector<int>(var_nb*var_nb*domain_bound, 0);
	vector<vector<pair<int, int> > > support_values = vector<vector<pair<int, int> > >(var_nb*domain_bound);
	initializationAC4(all_deleted_values, support_count, support_values);
	if (inconsistent_instantiation)
	{
		return;
	}

	vector<pair<int, int> > deleted_values;
	for (auto deleted_value : all_deleted_values)
	{
		deleted_values.push_back(deleted_value);
	}
	//cout << "SIZE : " << deleted_values.size() << endl;
	while (deleted_values.size() > 0)
	{
		// Popping the last deleted value out of the container
		pair<int, int> deleted_pair = deleted_values.back();
		deleted_values.pop_back();

		if (support_values[deleted_pair.second + (domain_bound*deleted_pair.first)].size()!=0)
		{
			for (auto paired_value : support_values[deleted_pair.second + (domain_bound*deleted_pair.first)])
			{
				// Making sure the paired variable is not instantiated yet, it is useless otherwise
				support_count[(paired_value.second) + (paired_value.first*domain_bound) + (deleted_pair.first*domain_bound*var_nb)] += -1;
					
				// If the paired value has no support value left : propagate
				if (support_count[(paired_value.second) + (paired_value.first*domain_bound) + (deleted_pair.first*domain_bound*var_nb)] == 0)
				{
					vector<int>::iterator value_it = std::find(var_domains[paired_value.first].begin(),
						var_domains[paired_value.first].end(),
						paired_value.second);
					if (value_it != var_domains[paired_value.first].end())
					{
						removeDomainValue(paired_value.first, value_it);
						deleted_values.push_back(paired_value);
						// Keeping all the deleted values to add them back in case the backtrack goes back
						all_deleted_values.push_back(paired_value);
						if (var_domains[paired_value.first].size() == 0)
						{
							inconsistent_instantiation = true;
							return;
						}

						//cout << "Deleted value with propagation " << paired_value.first << "," << paired_value.second << endl;
					}
				}
			}
		}
		else
		{
			//cout << deleted_pair.first << "," << deleted_pair.second << "n'a aucune valeur support" << endl;
		}
	}

	return;
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
		
		//cout << "Affichage" << endl;
		//int isz = 0;
		//for (auto dma : var_domains)
		//{
		//	cout << isz << ": ";
		//	for (auto val : dma)
		//	{
		//		cout << " " << val;
		//	}
		//	cout << endl;
		//	isz++;
		//}
		//cout << "FINs" << endl;

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

	// Checks if the partial instantiation is correct, more precisely if the last added value does not create conflicts
	vector<pair<int, int> >::iterator pair_it;
	for (int vect_idx = 0; vect_idx < var_idx; vect_idx++)
	{
		if (constrained_vars[(visit_order_vect[var_idx]*var_nb) + visit_order_vect[vect_idx]])
		{
			bool constraint_respected = checkConstraint(visit_order_vect[var_idx], visit_order_vect[vect_idx],
				partial_instantiation[var_idx], partial_instantiation[vect_idx]);
			if (not constraint_respected)
			{
				// The partial solution is not legit
				return false;
			}
		}
	}
	//cout << "Exploring var idx " << var_idx << " with value " << partial_instantiation.back() << endl;

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
	//cout << endl;

	vector<pair<int, int> > ac_deleted_values;

	if (forward_check_activated)
	{
		forwardCheck(ac_deleted_values, partial_instantiation);
		if (inconsistent_instantiation)
		{
			inconsistent_instantiation = false;
			addValues(ac_deleted_values);
			return false;
		}
	}
	if (arc_consistency_activated)
	{
		AC4(ac_deleted_values);
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

	//cout << "pairs ";
	//for (auto pair : ac_deleted_values)
	//{
	//	cout << "(" << pair.first << ',' << pair.second << ") ";
	//}
	//cout << endl;

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
	//cout << "Var " << explored_idx << " not working, going back" << endl;
	//cout << "Adding : ";
	//for (auto pair : ac_deleted_values)
	//{
	//	cout << "(" << pair.first << ',' << pair.second << ") ";
	//}
	//cout << endl;
	var_domains[visit_order_vect[explored_idx]] = former_var_domain;
	addValues(ac_deleted_values);
	instantiated_vars[visit_order_vect[explored_idx]] = false;
	return false;
}