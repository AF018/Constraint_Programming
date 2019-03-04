#include "utils.h"

#include <ctime>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//srand(0);
	srand(unsigned(time(NULL)));

	ConstraintProblem pb_test;
	std::clock_t start;
	double duration;

	start = std::clock();
	if (argc > 1)
	{
		if (string(argv[1]) == "queen")
		{
			int queen_nb = std::atoi(argv[2]);
			pb_test.createQueenProblem(queen_nb);
		}
		else if (string(argv[1]) == "color")
		{
			string filename(argv[2]);
			int color_attempt_nb = (std::atoi(argv[3]));
			pb_test.createColorationProblem(filename, color_attempt_nb);
		}
		else
		{
			cout << "Wrong input : " << argv[1] << endl;
		}
	}
	else
	{
		int queen_nb = 25;
		pb_test.createQueenProblem(queen_nb);
		//pb_test.createColorationProblem("Instances\\myciel5.col", 5);
		//pb_test.createColorationProblem("Instances\\queen10_10.col", 9);
		//pb_test.createColorationProblem("Instances\\jean.col", 9);
		//pb_test.createColorationProblem("Instances\\le450_15b.col", 15);
	}

	// Applying parameters found in parameters.txt for the optimization
	vector<int> parameters_vect = readParameters("parameters.txt");
	pb_test.applyParameters(parameters_vect);

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Construction duration : " << duration << " seconds" << endl;;
	cout << endl;

	start = std::clock();
	vector<int> solution = pb_test.backtrackSolve();
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Backtrack duration : " << duration << " seconds" << endl;;
	cout << endl;

	cout << "Visited nodes number : " << pb_test.getVisitedNodesNb() << endl;
	cout << endl;

	// Displaying the solution
	if (solution.size() != 0)
	{
		cout << "Solution : ";
		for (auto it : solution)
		{
			cout << it << "  ";
		}
		cout << endl;
		cout << "Correct solution (only works if it is a queen problem) : ";
		if (checkQueenSolution(solution))
		{
			cout << "Yes" << endl;
		}
		else
		{
			cout << "No" << endl;
		}
	}

	return 0;
}