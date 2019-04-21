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
#include "random_number.h"

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


#endif /* OUTPUT_H_ */
