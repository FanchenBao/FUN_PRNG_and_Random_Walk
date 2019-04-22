/*
 * output.h
 *
 *  Created on: Apr 21, 2019
 *      Author: fanchen
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <iostream>
#include <iomanip>   // std::setw
#include <string>
#include <vector>
#include "random_number.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

void outputRanUni(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	std::cout << "Uniform-distributed Random Numbers\nx" << std::endl;
	for (int i = 0; i < count; i++)
		std::cout << rn.ranUni() << std::endl;
}

void outputRanGau(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	std::cout << "Gaussian-distributed Random Numbers\n\ty1\t\ty2" << std::endl;
	for (int i = 0; i < count / 2; i++){ // each iteration output two random Gaussian values
		std::pair<double, double> rg = rn.ranGau();
		std::cout << std::setw(10) << rg.first << "\t";
		std::cout << std::setw(10) << rg.second << std::endl;
	}
}

void outputRanBin(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	std::cout << "Binomial-distributed Random Numbers\nBin0\tBin1" << std::endl;
	for (int i = 0; i < count; i++){
		int bin = rn.ranBin();
		if (bin)
			std::cout << "\t1" << std::endl;
		else
			std::cout << "0\t" << std::endl;
	}
}

void outputRandomWalk(std::vector<std::vector<double> > &coord){
	std::cout << "x\ty" << std::endl;
	std::cout << std::fixed;
	std::cout << std::setprecision(3);
	for (size_t i = 0; i < coord[0].size(); i++){
		std::cout << coord[0][i] << "\t" << coord[1][i] << std::endl;
	}
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


#endif /* OUTPUT_H_ */
