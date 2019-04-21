/*
 * random_number.h
 *
 *  Created on: Apr 21, 2019
 *      Author: fanchen
 */

#ifndef RANDOM_NUMBER_H_
#define RANDOM_NUMBER_H_

#define _USE_MATH_DEFINES

#include <iostream>
#include <ctime> // std::time() for seeding purpose
#include <utility> // std::pair<>
#include <cmath> // std::log(), std::abs()

class RandomNumber{
private:
	uint64_t curr;
public:
	RandomNumber(uint64_t seed = std::time(nullptr)) : curr(seed){} // constructor, set curr value. default seed is current time

	double ranUni(){ // return the next uniform-distributed random number between 0 (exclusive) and 1 (exclusive)
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

	int ranBin() {return ranUni() >= 0.5 ? 1 : 0;} // return the next binomial-distributed random number 0 or 1
};



#endif /* RANDOM_NUMBER_H_ */
