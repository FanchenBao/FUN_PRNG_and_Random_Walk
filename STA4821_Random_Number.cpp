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


int main() {
	RandomNumber rn(19890929);
	int dis[10] = {0};
	double rv;


	for (int i = 0; i < 100; i++){
		rv = rn.ranUni();
		dis[static_cast<int>(rv*10)]++;
//		std::cout << rn.ranUni() << std::endl;
	}
	for (int d : dis)
		std::cout << d << ", ";
	std::cout << "\n";
	return 0;
}
