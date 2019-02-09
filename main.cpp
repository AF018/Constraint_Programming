#include "ConstraintProblem.h"

#include <ctime>

int main()
{
	int queen_nb = 38;

	cout << "Starting ..." << endl;
	std::clock_t start;
	double duration;

	start = std::clock();
	ConstraintProblem pb_test(queen_nb);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Construction duration : " << duration << " seconds" << endl;;
	cout << endl;

	start = std::clock();
	pb_test.initializationAC4();
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Arc consistency duration : " << duration << " seconds" << endl;;
	cout << endl;

	start = std::clock();
	vector<int> solution = pb_test.backtrackSolve();
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Backtrack duration : " << duration << " seconds" << endl;;
	cout << endl;

	// Displaying the solution
	cout << "Solution : ";
	for (auto it : solution)
	{
		cout << it << "  ";
	}
	cout << endl;

	return 0;
}