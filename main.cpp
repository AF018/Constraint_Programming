#include "ConstraintProblem.h"

#include <ctime>

int main()
{
	int queen_nb = 30;

	cout << "Starting ..." << endl;
	std::clock_t start;
	double duration;

	start = std::clock();
	ConstraintProblem pb_test(queen_nb);
	vector<int> solution = pb_test.backtrackSolve();
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Duration : " << duration << " seconds" << endl;;
	cout << endl;


	cout << "Solution : ";
	for (auto it : solution)
	{
		cout << it << "  ";
	}
	cout << endl;

	return 0;
}