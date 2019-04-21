//============================================================================
// Name        : STA4821_Random_Number.cpp
// Author      : Fanchen
// Date	       : 04/19/2019
// Description : Pseudo-Random Number Generator for uniform, Gaussian, and binary
//				 distribution. The generators are also used to perform a random
//				 walk simulation
//============================================================================

#define _USE_MATH_DEFINES

#include <iostream>
#include <utility> // std::pair<>
#include <random> // for comparison with C++ PRNG
#include <ctime> // std::time() for seeding purpose
#include <cmath> // std::log(), std::abs()
#include <iomanip>   // std::setw
#include <vector>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

class RandomNumber{
private:
	uint64_t curr;
public:
	RandomNumber(uint64_t seed = std::time(nullptr)) : curr(seed){} // constructor, set curr value. default seed is current time

	double ranUni(){ // return the next uniform-distributed random number between 0 (inclusive) and 1 (exclusive)
		curr *= curr;

		/* Inspired by "Middle Square Weyl Sequence PRNG" described in
		 * https://en.wikipedia.org/wiki/Middle-square_method
		 * The bit shifting (basically swapping the first and second half of the number)
		 * greatly reduces the likelihood of trailing zeros in curr.
		 * If trailing zeros occur, the sequence will very quickly converge to 0 and
		 * become unsuitable for generating pseudo-random number
		 */
		curr = (curr >> 32 | curr << 32);

		// Take the middle 10 digits to form the uniform-distributed random number.
		return ((curr % 1000000000000000) / 100000) / 10000000000.0;
	}

	std::pair<double, double> ranGau(){ // return the next pair of Gaussian-distributed random number
		double x1 = ranUni(); // get two uniformly distributed values
		double x2 = ranUni();
		// use Box-Muller transform
		return std::make_pair(std::sqrt(-2 * std::log(x1)) * std::cos(2 * M_PI * x2), std::sqrt(-2 * std::log(x1)) * std::sin(2 * M_PI * x2));
	}

	int ranBin(){ // return the next binary-distributed random number 0 or 1
		return ranUni() >= 0.5 ? 1 : 0;
	}

};


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


void outputRanUni(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	std::cout << "x" << std::endl;
	for (int i = 0; i < count; i++)
		std::cout << rn.ranUni() << std::endl;
}

void outputRanGau(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	std::cout << "\ty1\t\ty2" << std::endl;
	for (int i = 0; i < count / 2; i++){ // each iteration output two random Gaussian values
		std::pair<double, double> rg = rn.ranGau();
		std::cout << std::setw(10) << rg.first << "\t";
		std::cout << std::setw(10) << rg.second << std::endl;
	}
}

void outputRanBin(int count, uint64_t myseed = std::time(nullptr)){
	RandomNumber rn(myseed);
	std::cout << "Bin 0\tBin1" << std::endl;
	for (int i = 0; i < count; i++){
		int bin = rn.ranBin();
		if (bin)
			std::cout << "\t1" << std::endl;
		else
			std::cout << "0\t" << std::endl;
	}
}

class RandomWalk{
private:
	int mode; // mode = 0, Use uniform distribution; mode = 1, Use Gaussian distribution
	// horizontal reach
	double maxX;
	double minX;
	// vertical reach
	double maxY;
	double minY;

	double mapW; // map width
	double mapH; // map height

	// destination coordinates
	double desX;
	double desY;
	RandomNumber rn;

	bool inBoundary(double x, double y){ // check whether the location is within or out of bound
		return (x > maxX || x < minX || y > maxY || y < minY) ? false : true;
	}

	bool reachDest(double x, double y){ // check whether random walk has reached the destination
		double error = 0.00001; // arbitrary error margin
		return (std::abs(x - desX) >= error || std::abs(y - desY) >= error) ? false : true;
	}

	void randomPickStart(double &sx, double &sy){
		switch(mode){
		case 0: // use Uniform distribution
			sx = (rn.ranUni()) * mapW - maxX; // make sure minX <= sx <= maxX
			sy = (rn.ranUni()) * mapH - maxY; // make sure minY <= sy <= maxY
			break;
		case 1: // use Gaussain distribution
			while(true){
				std::pair<double, double> rv = rn.ranGau();
				if (std::abs(rv.first) < maxX && std::abs(rv.second) < maxY){ // choose the first Gaussian rv pair that are within the map boundary
					sx = rv.first;
					sy = rv.second;
					break;
				}
			}
			break;
		default:
			std::cerr << "Error in RandomWalk.mode" << std::endl;
			exit(1);
		}
	}

	double getDistance(){
		// determine step size for x and y direction
		double d;
		switch(mode){
		case 0: // use Uniform distribution
			d = rn.ranUni();
			break;
		case 1: // use Gaussain distribution
			while(true){
				std::pair<double, double> rv = rn.ranGau();
				// choose one Gaussian rv that has abs value between 0 and 1
				if (std::abs(rv.first) <= 1){
					d = std::abs(rv.first);
					break;
				}
				else if (std::abs(rv.second) <= 1){
					d = std::abs(rv.second);
					break;
				}
				// else, neither rv values meet the requirement, try again
			}
			break;
		default:
			std::cerr << "Error in RandomWalk.mode" << std::endl;
			exit(1);
		}
		return d;
	}


public:
	RandomWalk(int m, uint64_t seed = std::time(nullptr), double width = 4.0, double height = 4.0, double dx = 0.0, double dy = 0.0) :
		mode(m),
		maxX(width / 2.0), minX(0 - width / 2.0),
		maxY(height / 2.0), minY(0 - height / 2.0),
		mapW(width), mapH(height),
		desX(dx), desY(dy), rn(seed){} // constructor. The map default will be centered on (0, 0) with width and height being 4.

	std::vector<std::vector<double> > walk(int steps = 100, double sx = 0, double sy = 0){ // default randwom walk 100 steps, with random starting position
		std::vector<std::vector<double> > res; // an array of two arrays, res[0] for x coordinates, res[1] for y coordinates.
		res.emplace_back(std::vector<double>());
		res.emplace_back(std::vector<double>());

		if (sx == 0 && sy == 0) // determine starting position
			randomPickStart(sx, sy);
		// push starting positions to res
		res[0].push_back(sx);
		res[1].push_back(sy);

		for (int i = 0; i < steps; i++){
			double newX, newY;

			while (true){ // find next position
				newX = *res[0].rbegin();
				newY = *res[1].rbegin();
				double d = getDistance(); // get next step's distance

				// determine walking direction
				int xORy = rn.ranBin(); // 1 = in x direction, 0 = in y direction
				int dir = rn.ranBin() ? 1 : -1; // 1 = go right or up, -1 = go left or down

				// find next coordinates of next position
				if (xORy) // in x direction
					newX += d * dir;
				else // in y direction
					newY += d * dir;

				if (inBoundary(newX, newY)){ // new position within boundary, push them to res
					res[0].push_back(newX);
					res[1].push_back(newY);
					break;
				}
				// else if new positions out of boundary, try again
			}
			if (reachDest(newX, newY)) // if the new position reaches destination, end walk
				break;
		}
		return res;
	}

	std::pair<double, double> getXRange(){return std::make_pair(minX, maxX);}
	std::pair<double, double> getYRange(){return std::make_pair(minY, maxY);}
	std::pair<double, double> getDes(){return std::make_pair(desX, desY);}
};


void plotRandomWalk(std::vector<std::vector<double> > coord, RandomWalk &rw){
	std::pair<double, double> xrange = rw.getXRange();
	std::pair<double, double> yrange = rw.getYRange();
	std::pair<double, double> des = rw.getDes();

	plt::figure_size(780, 780);

	std::map<std::string, std::string> keywords;
	keywords["linewidth"] = "0.5";
	keywords["linestyle"] = "-";
	keywords["marker"] = ".";
	keywords["markerfacecolor"] = "b";
	plt::plot(coord[0], coord[1], keywords);

	plt::plot({xrange.first, xrange.second}, {0, 0}, "k-"); // x axis
	plt::plot({0, 0}, {yrange.first, yrange.second}, "k-"); // y axis

	plt::plot({des.first},{des.second}, "g*"); // destination point
	plt::plot({}, {}, "ro"); // starting point
	plt::plot({}, {}, "kX"); // last step point

	// set x and y axis limit
	plt::xlim(xrange.first, xrange.second);
	plt::ylim(yrange.first, yrange.second);

	plt::show();
}


int main() {
//	comparePRNG(10000);
//	outputRanUni(20);
//	outputRanGau(20);
//	outputRanBin(20);
	RandomWalk rw(0);
	std::vector<std::vector<double> > coord = rw.walk();
	plotRandomWalk(coord, rw);



	return 0;
}
