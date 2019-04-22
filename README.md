# FUN_PRNG_and_Random_Walk
Custom PRNG for Uniform, Gaussian, and Binomial Distribution, and Random Walk with plot in matplotlib

## Introduction
This project includes a ```RandomNumber``` class, which, upon seeding, can produce pseudo-random numbers in uniform, Gaussian, or binomial distributions. It also includes a ```RandomWalk``` class, which can execute an unguided random walk within a map (default width and height 4 units, centered on point (0, 0)), using either uniform- or Gaussian-distributed random numbers generated from the ```RandomNumber``` class. The result of random walk is plotted using [matplotlibcpp](https://github.com/lava/matplotlib-cpp).
## Interfaces
### ```RandomNumber``` (random_number.h)
1. ```RandomNumber(uint64_t seed)```

   Constructor of ```RandomNumber``` class. Seed should be a larger integer number. Default seed value is the number of seconds since the Epoch (i.e. ```std::time(nullptr)```).

2. ```ranUni()```

   **Return one uniform-distributed pseudo-random number in double form.** The algorithm is borrowed from the “Middle-square method”. One draw back of this method is that if the number to be squared has trailing zeros, the number of zeros in the following iterations would grow and eventually causing the whole pseudo-random number sequence to quickly converge to 0. To deal with this issue, the method is modified following “Middle Square Weyl Sequence PRNG” (https://en.wikipedia.org/wiki/Middle-square_method), in which before the number is squared, its higher 32-bits and lower 32-bits are swapped. This way, we can guarantee that there will be no growth in trailing zeros after the square-and-swap step. The actual random number comes from the number after square-and-swap, but turned into a value between 0 and 1 (exclusive on both ends). 

3. ```ranGau()```

   **Return a pair of Gaussian-distributed pseudo-random number in the form of std::pair<double, double>.** The algorithm follows the Box-Muller transformation, in which a pair of uniform-distributed random numbers (generated from ```ranUni()```) were plugged into the Box-Muller transformation formula to generate two Gaussian-distributed random numbers.

4. ```ranBin()```

   **Return one binomial-distributed pseudo-random number in int form.** The algorithm first generates a uniform-distributed random number (from ```ranUni()```), and compared it to 0.5. If it is larger or equal to 0.5, ```ranBin()``` returns 1, otherwise 0. 

### ```RandomWalk``` (random_walk.h)
1. ```RandomWalk(int m, uint64_t seed = std::time(nullptr))```

   Constructor of ```RandomWalk``` class. ```m``` determines which random number distribution to use for random walk: m = 1, use Gaussian; m = 0, use uniform.

2. Setters
   * ```void setDim(double w, double h)``` Set map's width and height. Default width = height = 4 units.
   * ```void setStart(double x, double y)``` Set custom starting point. Default starting point is chosen randomly from any position on the map.
   * ```void setDes(double x, double y)``` Set custom destination point. Default destination point at (0, 0).

3. Getters
   * ```std::pair<double, double> getXRange()``` Get the range of x axis on the map.
   * ```std::pair<double, double> getYRange()``` Get the range of y axis on the map.
   * ```std::pair<double, double> getDes()``` Get the coordinates of the destination point.
   * ```std::pair<double, double> getStart()``` Get the coordinates fo the starting point.

4. ```std::vector<std::vector<double> > walk(int steps)```

   **Return a vector of two vectors, the first one containing all x coordinates of the points in the random walk, and the second one y coordinates.** User can input any number of steps to take; the default number of steps is 100. The rule of random walk is that at each step, the point only has four directions to go. Once a direction is randomly selected, the step distance the point can take is generated from either uniform- or Gaussian-distributed random number. The requirement for the step distance is that it must be between 0 and 1, inclusive, and that the new point must be within the boundary of the map.

### Output (output.h)
1. ```void outputRanUni(int count, uint64_t myseed)```

   Print out ```count``` number of uniform-distributed random numbers seeded by ```myseed```. Default value for ```myseed``` is ```std::time(nullptr)```, which is the same for the other interfaces shown below.

2. ```void outputRanGau(int count, uint64_t myseed)```

   Print out ```count``` number of Gaussian-distributed random numbers.

3. ```void outputRanBin(int count, uint64_t myseed)```

   Print out ```count``` number of binomial-distributed random numbers.

4. ```void outputRandomWalk(std::vector<std::vector<double> > &coord)```

   Print out the coordinates of all points sequentially passed through in random walk. ```coord``` is the returned value from calling ```RandomWalk::walk()```.

5. ```void plotRandomWalk(std::vector<std::vector<double> > coord, RandomWalk &rw, int disMode)```

   Plot the random walk using matplotlibcpp.h and display the graph. ```disMode``` indicates which distribution of random number is to use (disMode = 0, uniform; disMode = 1, Gaussian). ```rw``` is an instance of ```RandomWalk```. ```coord``` is the returned value of ```rw.walk()```.

## Usage and Examples
### Compilation
Follow the usage guidelines described in [matplotlibcpp](https://github.com/lava/matplotlib-cpp) to include and link to Python for plotting purpose. If the steps described there don't work for your system, you can use Homebrew to install (or reinstall) Python, use ```pip``` to install matplotlib and numpy, and run the following bash script to compile the C++ code (`$(python3-config --cflags)` handles the inclusion of header files in Python; `$(python3-config --ldflags)` handles linker to Python).
```bash
g++ main.cpp -o main.out $(python3-config --cflags) $(python3-config --ldflags) -std=c++11
```
### Header files
In main.cpp, include all the header files.
```cpp
#include "random_number.h"
#include "random_walk.h"
#include "output.h"
```
### Output random numbers
```cpp
int main{
  outputRanUni(10); // output 10 uniform-distributed random numbers
  std::cout << "\n";
  outputRanGau(10); // output 10 Gaussian-distributed random numbers
  std::cout << "\n";
  outputRanBin(10); // output 10 binomial-distributed random numbers
  std::cout << "\n";
  
  return 0;
}
/* output result
Uniform-distributed Random Numbers
x
0.858251
0.495672
0.751231
0.696892
0.987067
0.357003
0.837429
0.662025
0.208294
0.776459

Gaussian-distributed Random Numbers
	y1		y2
 -0.552713	 0.0150341
 -0.247729	 -0.714644
  -0.10049	  0.126238
 -0.312758	 -0.506972
  0.293126	  -1.74691

Binomial-distributed Random Numbers
Bin0	Bin1
	1
0	
	1
	1
	1
0	
	1
	1
0	
	1
*/
```
### Output random walk coordinates
```cpp
int main{
  int disMode = 1; // 1 = Gaussian, 0 = uniform
  RandomWalk rw(disMode);
  std::vector<std::vector<double> > coord = rw.walk();
  outputRandomWalk(coord); // output all coordinates of the positions in the random walk
  
  return 0;
}

/* output
x	y
-0.553	0.015
-0.800	0.015
-0.800	0.328
...
...
...
-0.165	-0.959
-0.142	-0.959
-0.440	-0.959
```
### Plot random walk
```cpp
int main{
  int disMode = 0; // 1 = Gaussian, 0 = uniform
  RandomWalk rw(disMode);
  std::vector<std::vector<double> > coord = rw.walk();
  plotRandomWalk(coord, rw, disMode); // plot random walk
  
  return 0;
}
```
![Random walk based on uniform-distributed random numbers](https://github.com/FanchenBao/FUN_PRNG_and_Random_Walk/blob/master/images/random_walk_demo_uniform.png)
