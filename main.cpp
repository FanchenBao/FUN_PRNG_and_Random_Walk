//============================================================================
// Name        : STA4821_Random_Number.cpp
// Author      : Fanchen
// Date	       : 04/19/2019
// Description : Pseudo-Random Number Generator for uniform, Gaussian, and binary
//				 distribution. The generators are also used to perform a random
//				 walk simulation
//============================================================================


#include <string>
#include <random> // for comparison with C++ PRNG
#include <vector>
#include "matplotlibcpp.h"
#include "random_number.h"
#include "random_walk.h"
#include "output.h"


namespace plt = matplotlibcpp;

void comparePRNG(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	int myDis[10] = {0};
	int cppDis[10] = {0};
	double rv;

	// C++ standard random number generator
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> d(0.0, 1.0);

	for (int i = 0; i < count; i++){
		rv = rn.ranUni();
		myDis[static_cast<int>(rv*10)]++;
		rv = d(gen);
		cppDis[static_cast<int>(rv*10)]++;
	}
	std::cout << "My PRNG distribution from 0.0 to 0.9" << std::endl;
	for (int i = 0; i < 10; i++)
		std::cout << "." << i << "\t";
	std::cout << "\n";
	for (int i = 0; i < 10; i++)
		std::cout << myDis[i] << "\t";
	std::cout << "\n\n";

	std::cout << "C++ PRNG distribution from 0.0 to 0.9" << std::endl;
	for (int i = 0; i < 10; i++)
		std::cout << "." << i << "\t";
	std::cout << "\n";
	for (int i = 0; i < 10; i++)
		std::cout << cppDis[i] << "\t";
	std::cout << "\n";
}


void plotRandomWalk(std::vector<std::vector<double> > coord, RandomWalk &rw, int disMode){
	std::pair<double, double> xrange = rw.getXRange();
	std::pair<double, double> yrange = rw.getYRange();
	std::pair<double, double> des = rw.getDes();
	std::pair<double, double> sta = rw.getStart();

	std::vector<std::string> colors = {"g", "r", "b", "c", "m", "y"}; // color choices for each step
	std::map<std::string, std::string> kw; // plot param for the actual walk
	kw["markersize"] = "5";
	kw["linestyle"] = "-";
	kw["linewidth"] = "0.5";

	for (size_t i = 1; i < coord[0].size(); i++){
		kw["color"] = colors[i % 6]; // make sure each step has a different color.
		if (coord[0][i] > coord[0][i - 1]) {kw["marker"] = ">";} // go right
		else if (coord[0][i] < coord[0][i - 1]) {kw["marker"] = "<";} // go left
		else{
			if (coord[1][i] > coord[1][i - 1]) {kw["marker"] = "^";} // go up
			else if (coord[1][i] < coord[1][i - 1]) {kw["marker"] = "v";} // go down
		}
		plt::plot({coord[0][i]},{coord[1][i]}, kw); // plot the arrow
		kw["marker"] = "";
		plt::plot({coord[0][i-1], coord[0][i]},{coord[1][i-1], coord[1][i]}, kw); // plot the line

	}

	plt::plot({xrange.first, xrange.second}, {0, 0}, "k-"); // x axis
	plt::plot({0, 0}, {yrange.first, yrange.second}, "k-"); // y axis

	std::map<std::string, std::string> keywords; // plot param for the legends
	keywords["marker"] = "D";
	keywords["markerfacecolor"] = "g";
	keywords["markeredgecolor"] = "g";
	keywords["markersize"] = "8";
	keywords["label"] = "Destination";
	keywords["linestyle"] = "";
	plt::plot({des.first},{des.second}, keywords); // destination point

	keywords["marker"] = "o";
	keywords["markerfacecolor"] = "r";
	keywords["markeredgecolor"] = "r";
	keywords["label"] = "Start";
	plt::plot({sta.first}, {sta.second}, keywords); // starting point

	keywords["marker"] = "X";
	keywords["markerfacecolor"] = "k";
	keywords["markeredgecolor"] = "k";
	keywords["label"] = "End";
	plt::plot({*coord[0].rbegin()}, {*coord[1].rbegin()}, keywords); // last step point
	plt::legend();

	// set x and y axis limit
	plt::xlim(xrange.first, xrange.second);
	plt::ylim(yrange.first, yrange.second);

	// set plot title
	if (disMode)
		plt::title("Random Walk With Gaussian-distributed Random Step Distance");
	else
		plt::title("Random Walk With Uniform-distributed Random Step Distance");

	plt::show();
}


int main() {
//	comparePRNG(10000);
//	outputRanUni(20);
//	std::cout << "\n";
//	outputRanGau(20);
//	std::cout << "\n";
//	outputRanBin(20);
//	std::cout << "\n";
	int disMode = 1; // 1 = Gaussian, 0 = uniform
	RandomWalk rw(disMode);
	std::vector<std::vector<double> > coord = rw.walk();
	plotRandomWalk(coord, rw, disMode);
	outputRandomWalk(coord);


	return 0;
}
