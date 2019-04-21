/*
 * random_walk.h
 *
 *  Created on: Apr 21, 2019
 *      Author: fanchen
 */

#ifndef RANDOM_WALK_H_
#define RANDOM_WALK_H_

#include "random_number.h"

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

	// starting point coordinates
	double staX;
	double staY;

	RandomNumber rn;

	bool inBoundary(double x, double y){ // check whether the location is within or out of bound
		return (x > maxX || x < minX || y > maxY || y < minY) ? false : true;
	}

	bool reachDest(double x, double y){ // check whether random walk has reached the destination
		double error = 0.00001; // arbitrary error margin
		return (std::abs(x - desX) >= error || std::abs(y - desY) >= error) ? false : true;
	}

	void randomPickStart(){
		switch(mode){
		case 0: // use Uniform distribution
			staX = (rn.ranUni()) * mapW - maxX; // make sure minX <= sx <= maxX
			staY = (rn.ranUni()) * mapH - maxY; // make sure minY <= sy <= maxY
			break;
		case 1: // use Gaussain distribution
			while(true){
				std::pair<double, double> rv = rn.ranGau();
				if (std::abs(rv.first) < maxX && std::abs(rv.second) < maxY){ // choose the first Gaussian rv pair that are within the map boundary
					staX = rv.first;
					staY = rv.second;
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
	RandomWalk(int m, uint64_t seed = std::time(nullptr)) :
		mode(m),
		maxX(2.0), minX(-2.0),
		maxY(2.0), minY(-2.0),
		mapW(4.0), mapH(4.0),
		desX(0.0), desY(0.0), rn(seed){randomPickStart();} // constructor.
	//The map default will be centered on (0, 0) with width and height being 4.
	// Default destination is (0, 0); default starting point is randomly selected

	std::vector<std::vector<double> > walk(int steps = 100){ // default randwom walk 100 steps
		std::vector<std::vector<double> > res; // an array of two arrays, res[0] for x coordinates, res[1] for y coordinates.
		res.emplace_back(std::vector<double>());
		res.emplace_back(std::vector<double>());

		// push starting positions to res
		res[0].push_back(staX);
		res[1].push_back(staY);

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

	// getters
	std::pair<double, double> getXRange(){return std::make_pair(minX, maxX);}
	std::pair<double, double> getYRange(){return std::make_pair(minY, maxY);}
	std::pair<double, double> getDes(){return std::make_pair(desX, desY);}
	std::pair<double, double> getStart(){return std::make_pair(staX, staY);}

	// setters
	void setDim(double w, double h){ // set map's width and height.
		mapW = w; mapH = h;
		maxX = w / 2.0; minX = 0 - maxX;
		maxY = h / 2.0; minY = 0 - maxY;
	}
	void setStart(double x, double y) {staX = x; staY = y;} // set custom starting point
	void setDes(double x, double y) {desX = x; desY = y;} // set custom destination point
};



#endif /* RANDOM_WALK_H_ */
