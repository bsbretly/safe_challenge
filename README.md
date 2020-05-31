# Safe Challenge

The problem proposed asks the user to unlock a laser reflecting safe. The challenge is to find the grid location(s) on which to place a mirror(s) in order to successfully reflect the laser out of the bottom right of the grid (see "rr_take_home_challenge_mirrors_and_lasers.pdf" for challenge details. 

The algorithm developed here follows a two phase approach: phase 1 tracks the state of a laser that is directed rightwards from the top, left corner of the grid, storing the laser state as it progresses through the grid in a map. If mirror(s) must be added to unlock the safe, phase 2 of the algorithm tracks a laser directed leftwards from the bottom, right of the grid and searches for intersections relative to the saved laser states from part 1. The intersection locations are the grid locations where mirrors must be added to unlock the safe (if possible).

Clone/copy this directory to your location of choice and run the following commands in your terminal from the top of the directory:

1. mkdir build && cd build
2. cmake .. && make
3. ./safe_challenge ```<your appropiately formatted grid specification text file>```

The terminal command to run the included input file from your build directory would be: ```./safe_challenge "input.txt" ```

The algorithm outputs statements according to the challenge requirements along with the execution time required to open the safe. 

## Platform Notes

The code provided in this directory was tested and compiled with gcc 9.3.0 in Ubuntu 20.04.
