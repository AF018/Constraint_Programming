#include "utils.h"

vector<int> readParameters(string const & filename)
{
	vector<int> parameters;
	ifstream file_reader;
	file_reader.open(filename);

	if (!file_reader)
	{
		cout << "Unable to open : " << filename << endl;
		// call system to stop
		exit(1);
	}
	char readChar;
	int readInt;

	// Arc consistency : AC
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	// Forward check : FC
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	// Random visit order : RV
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	// Visit small domains first : SD
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	// Visit large domains first : LD
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	// Arc consistency every two iterations during the backtrack : HA
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	// Values constrained with more values from incoming variables : VA
	file_reader >> readChar >> readChar >> readInt;
	parameters.push_back(readInt);
	return parameters;
}

bool checkQueenSolution(vector<int> const & solution)
{
	int value_i, value_j;
	bool accepted_values;
	for (int var_i = 0; var_i < solution.size(); var_i++)
	{
		value_i = solution[var_i];
		for (int var_j = var_i+1; var_j < solution.size(); var_j++)
		{
			value_j = solution[var_j];
			accepted_values = (value_i != value_j) 
				&& (var_i + value_i != var_j + value_j) 
				&& (var_i - value_i != var_j - value_j);
			if (not accepted_values)
			{
				return false;
			}
		}
	}
	return true;
}