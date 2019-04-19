//============================================================================
// Name        : STA4821_Random_Number.cpp
// Author      : Fanchen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <utility>
#include <random>

class RandomNumber{
private:
	uint64_t curr;
public:
	RandomNumber(uint64_t seed) : curr(seed){} // constructor, set curr value

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

//	std::pair<double, double> ranGau(){ // return the next Gaussian-distributed random number
//
//	}
//
//	int ranBin(){ // return the next binary-distributed random number 0 or 1
//
//	}

};


void comparePRNG(uint64_t myseed, int count){
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

int main() {
//	comparePRNG(19890929, 10000);

	return 0;
}
