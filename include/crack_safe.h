#ifndef RAPYUTACHALLENGE_CRACK_SAFE_H
#define RAPYUTACHALLENGE_CRACK_SAFE_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>


class CrackSafe {

private:
    struct Position {
        int row;
        int col;
    };

    struct Mirror {
        int location;
        std::string type;
        std::string state;

        struct less_than {
            bool operator()(const Mirror &a, const Mirror &b) {
                return a.location < b.location;
            }
        };
    };

    // class variables
    int row_total_;
    int col_total_;
    bool breach_;
    std::vector<Position> intersections_;
    std::map<int,std::vector<Mirror>> row_map_;
    std::map<int,std::vector<Mirror>> col_map_;
    bool test_;

public:
    explicit CrackSafe(const std::string& input_file);
    void buildMirrorMaps (const std::string& file_name);
    void findIntersection(int row, int col, std::string& laser_state);
    void populateMapState(const std::string& movement, int row, int col, const std::string& state);
    void rowSearch(int& row_current, int& col_current, std::string& laser_state);
    void colSearch(int& row_current, int& col_current, std::string& laser_state);
    void openSafe(int& test_row_current, int& test_col_current, std::string& test_laser_state);
    static std::string updateState(const std::string& movement, const std::string& mirror_type, const std::string& laser_state);
    static bool compareLexicographically(Position pos1, Position pos2);
};

#endif //RAPYUTACHALLENGE_CRACK_SAFE_H
