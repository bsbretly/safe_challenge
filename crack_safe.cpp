#include "crack_safe.h"
#include "helpers.h"
using namespace helpers;

CrackSafe::CrackSafe(const std::string& input_file) {
    row_total_ = 0;
    col_total_ = 0;
    test_ = true;
    breach_ = false;
    buildMirrorMaps(input_file);
}

void CrackSafe::buildMirrorMaps (const std::string& file_name) {
    std::string file_path = "../" + file_name;
    std::ifstream input_file(file_path);
    std::string first_multiple_input_temp;
    std::getline(input_file, first_multiple_input_temp);
    std::vector<std::string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    row_total_ = std::stoi(first_multiple_input[0]) - 1; // num of rows, zero-indexed
    col_total_ = std::stoi(first_multiple_input[1]) - 1; // num of cols, zero-indexed

    int m = std::stoi(first_multiple_input[2]); // num of m-type / mirrors
    int n = std::stoi(first_multiple_input[3]); // num of n-type \ mirrors

    for (int i = 0; i < m + n; i++) {
        if (i < m) {
            std::string m_loc_row_temp_temp;
            std::getline(input_file, m_loc_row_temp_temp);
            std::vector<std::string> m_loc_row_temp = split(rtrim(m_loc_row_temp_temp));
            std::vector<int> m_loc;
            for (int j = 0; j < 2; j++) {
                int m_loc_row_item = std::stoi(m_loc_row_temp[j]);
                m_loc.push_back(m_loc_row_item - 1); // zero-index the mirror locations
            }

            Mirror temp_row_mirror;
            temp_row_mirror.location = m_loc[1];
            temp_row_mirror.type = "m";
            temp_row_mirror.state = "null";
            if (row_map_.count(m_loc[0]) == 0) {
                std::vector<Mirror> row_mirror_vector;
                row_mirror_vector.push_back(temp_row_mirror);
                row_map_.insert(std::pair<int, std::vector<Mirror>>(m_loc[0], row_mirror_vector));
            } else {
                auto it = row_map_.find(m_loc[0]);
                if (it != row_map_.end()) it->second.push_back(temp_row_mirror);
            }
            Mirror temp_col_mirror;
            temp_col_mirror.location = m_loc[0];
            temp_col_mirror.type = "m";
            temp_col_mirror.state = "null";
            if (col_map_.count(m_loc[1]) == 0) {
                std::vector<Mirror> col_mirror_vector;
                col_mirror_vector.push_back(temp_col_mirror);
                col_map_.insert(std::pair<int, std::vector<Mirror>>(m_loc[1], col_mirror_vector));
            } else {
                auto it = col_map_.find(m_loc[1]);
                if (it != col_map_.end()) it->second.push_back(temp_col_mirror);
            }
        } else {
            std::string n_loc_row_temp_temp;
            std::getline(input_file, n_loc_row_temp_temp);
            std::vector<std::string> n_loc_row_temp = split(rtrim(n_loc_row_temp_temp));
            std::vector<int> n_loc;
            for (int j = 0; j < 2; j++) {
                int n_loc_row_item = std::stoi(n_loc_row_temp[j]);
                n_loc.push_back(n_loc_row_item - 1); // zero-index the mirror locations
            }

            CrackSafe::Mirror temp_row_mirror;
            temp_row_mirror.location = n_loc[1];
            temp_row_mirror.type = "n";
            temp_row_mirror.state = "null";
            if (row_map_.count(n_loc[0]) == 0) {
                std::vector<Mirror> row_mirror_vector;
                row_mirror_vector.push_back(temp_row_mirror);
                row_map_.insert(std::pair<int, std::vector<Mirror>>(n_loc[0], row_mirror_vector));
            } else {
                auto it = row_map_.find(n_loc[0]);
                if (it != row_map_.end()) it->second.push_back(temp_row_mirror);
            }

            CrackSafe::Mirror temp_col_mirror;
            temp_col_mirror.location = n_loc[0];
            temp_col_mirror.type = "n";
            temp_col_mirror.state = "null";
            if (col_map_.count(n_loc[1]) == 0) {
                std::vector<Mirror> col_mirror_vector;
                col_mirror_vector.push_back(temp_col_mirror);
                col_map_.insert(std::pair<int, std::vector<Mirror>>(n_loc[1], col_mirror_vector));
            } else {
                auto it = col_map_.find(n_loc[1]);
                if (it != col_map_.end()) it->second.push_back(temp_col_mirror);
            }
        }
    }
    // sort the row map
    std::map<int, std::vector<Mirror>>::iterator map_it;
    for (map_it = row_map_.begin(); map_it != row_map_.end(); ++map_it) {
        std::sort(map_it->second.begin(), map_it->second.end(), Mirror::less_than());
    }

    // sort the column map
    for (map_it = col_map_.begin(); map_it != col_map_.end(); ++map_it) {
        std::sort(map_it->second.begin(), map_it->second.end(), Mirror::less_than());
    }
}

void CrackSafe::findIntersection(int row, int col, std::string& state) {
    Mirror temp_mirror;
    if (state == "up") {
        auto it = row_map_.lower_bound(row);
        if (it != col_map_.begin()) it--;
        temp_mirror.location = col;
        auto ptr = std::lower_bound(it->second.begin(), it->second.end(), temp_mirror, Mirror::less_than());
        int idx = ptr - it->second.begin();
        if (idx - 1 >= 0 && it->second[idx - 1].state == "right" ||
            idx + 1 < it->second.size() && it->second[idx + 1].state == "left") {
            intersections_.push_back({it->first, col});
        }
    }
    else if (state == "down") {
        auto it = row_map_.upper_bound(row);
        temp_mirror.location = col;
        auto ptr = std::lower_bound(it->second.begin(), it->second.end(), temp_mirror, Mirror::less_than());
        int idx = ptr - it->second.begin();
        if (idx - 1 >= 0 && it->second[idx - 1].state == "right" ||
            idx + 1 < it->second.size() && it->second[idx + 1].state == "left") {
            intersections_.push_back({it->first, col});
        }
    }
    else if (state == "right") {
        auto it = col_map_.upper_bound(col);
        temp_mirror.location = row;
        auto ptr = std::lower_bound(it->second.begin(), it->second.end(), temp_mirror, Mirror::less_than());
        int idx = ptr - it->second.begin();
        if (idx - 1 >= 0 && it->second[idx - 1].state == "down" ||
            idx + 1 < it->second.size() && it->second[idx + 1].state == "up") {
            intersections_.push_back({row, it->first});
        }
    }
    else if (state == "left") {
        auto it = col_map_.lower_bound(col);
        // catch edge case if a mirror is to be added in the bottom, right position
        if (it != col_map_.end() && row == row_total_ && col == col_total_) {}
        else if (it != col_map_.begin()) it--; //&& it == col_map_.end()
        temp_mirror.location = row;
        auto ptr = std::lower_bound(it->second.begin(), it->second.end(), temp_mirror, Mirror::less_than());
        int idx = ptr - it->second.begin();
        if (idx - 1 >= 0 && it->second[idx - 1].state == "down" ||
            idx + 1 < it->second.size() && it->second[idx + 1].state == "up") {
            intersections_.push_back({row, it->first});
        }
    }
}

void CrackSafe::populateMapState(const std::string& movement, int row, int col, const std::string& state) {
    Mirror temp_mirror;
    if (movement == "row") {
        auto col_it = col_map_.find(col);
        temp_mirror.location = row;
        auto col_ptr = std::lower_bound(col_it->second.begin(), col_it->second.end(), temp_mirror, Mirror::less_than());
        int idx = col_ptr - col_it->second.begin();
        col_it->second[idx].state = state;
    }
    else {
        auto row_it = row_map_.find(row);
        temp_mirror.location = col;
        auto row_ptr = std::lower_bound(row_it->second.begin(), row_it->second.end(), temp_mirror, Mirror::less_than());
        int idx = row_ptr - row_it->second.begin();
        row_it->second[idx].state = state;
    }
}

void CrackSafe::rowSearch(int& row_current, int& col_current, std::string& state) {
    auto row_it = row_map_.find(row_current);
    if (row_it != row_map_.end()) {
        Mirror temp_mirror; temp_mirror.location = col_current;
        if (state == "right") {
            auto upper = std::upper_bound(row_it->second.begin(), row_it->second.end(), temp_mirror,
                                          Mirror::less_than());
            // temp_index is the index of the next mirror in col_current. If there are no more mirrors, temp_index = size of vector
            int temp_index = upper - row_it->second.begin();
            if (temp_index == row_it->second.size()) {
                breach_ = true;
                return;
            }
            col_current = row_it->second[temp_index].location;
            state = updateState("row", row_it->second[temp_index].type, state);
            if (test_) {
                row_it->second[temp_index].state = state;
                populateMapState("row", row_current, col_current, state);
            }
            return;
        }
        else {// state == "left"
            auto lower = std::lower_bound(row_it->second.begin(), row_it->second.end(), temp_mirror, Mirror::less_than());
            // temp_index is the index of the next upward mirror in col_current. If there are no more mirrors below, temp_index = 0
            int temp_index = lower - row_it->second.begin();
            if ((temp_index - 1) == -1) {
                breach_ = true;
                return;
            }
            col_current = row_it->second[temp_index - 1].location;
            state = updateState("row", row_it->second[temp_index - 1].type, state);
            if (test_) {
                row_it->second[temp_index - 1].state = state;
                populateMapState("row", row_current, col_current, state);
            }
            return;
        }
    }
    else breach_ = true;
}

void CrackSafe::colSearch(int& row_current, int& col_current, std::string& state) {
    auto col_it = col_map_.find(col_current);
    if (col_it != col_map_.end()) {
        Mirror temp_mirror; temp_mirror.location = row_current;
        if (state == "down") {
            auto upper = std::upper_bound(col_it->second.begin(), col_it->second.end(), temp_mirror, Mirror::less_than());
            int temp_index = upper - col_it->second.begin();
            if (temp_index == col_it->second.size()) {
                breach_ = true;
                return;
            }
            row_current = col_it->second[temp_index].location;
            state = updateState("col", col_it->second[temp_index].type, state);
            // update the laser state info
            if (test_) {
                col_it->second[temp_index].state = state;
                populateMapState("col", row_current, col_current, state);
            }
            return;
        }
        else {//state == "up"
            auto lower = std::lower_bound(col_it->second.begin(), col_it->second.end(), temp_mirror, Mirror::less_than());
            int temp_index = lower - col_it->second.begin();
            // temp_index returns index of the next upward mirror in col_current. If there are no more mirrors below, temp_index = 0
            if ((temp_index - 1) == -1) {
                breach_ = true;
                return;
            }
            row_current = col_it->second[temp_index - 1].location;
            state = updateState("col", col_it->second[temp_index - 1].type, state);
            if (test_) {
                col_it->second[temp_index - 1].state = state;
                populateMapState("col", row_current, col_current, state);
            }
            return;
        }
    }
    else breach_ = true;
}

void CrackSafe::openSafe(int& test_row, int& test_col, std::string& state) {
    // check if the grid is empty
    if (row_map_.empty() && col_map_.empty()) {
        std::cout << "impossible" << '\n';
        return;
    }
    // initialize from the top right
    auto row_it = row_map_.find(test_row);
    test_col = row_it->second[0].location;
    state = updateState("row", row_it->second[0].type, state);
    row_it->second[0].state = state;
    populateMapState("row", test_row, test_col, state);

    while (true) {
        colSearch(test_row, test_col, state);
        if (breach_) break;
        rowSearch(test_row, test_col, state);
        if (breach_) break;
    }

    // check if the safe is cracked without additional mirrors
    if (state == "right" && test_row == row_total_) {
        std::cout << 0 << '\n';
        return;
    }

    // make sure there is an n-type mirror in the last row. If not, add one
    if (row_map_.find(row_total_) == row_map_.end()) {
        if (state == "down") {
            std::cout << 1 << " " << row_total_ + 1<< " " << test_col + 1 << '\n';
            return;
        }
        else {
            std::cout << "impossible" << '\n';
            return;
        }
    }
    else if (row_map_.find(row_total_)->second.back().type != "n") {
        if (state == "down" && row_map_.find(row_total_)->second.back().location < test_col) {
        std::cout << 1 << " " << row_total_ + 1 << " " << test_col + 1 << '\n';
        return;
        }
        else {
            std::cout << "impossible" << '\n';
            return;
        }
    }

    else {
        // see if we can crack the safe by adding a mirror(s)
        breach_ = false;
        test_ = false;
        int row = row_total_;
        int col = row_total_;
        state = "left";

        findIntersection(row, col, state);
        row_it = row_map_.find(row);
        col = row_it->second.back().location;
        state = updateState("row", row_it->second.back().type, state);
        findIntersection(row, col, state);

        while (true) {
            colSearch(row, col, state);
            if (breach_) break;
            findIntersection(row, col, state);

            rowSearch(row, col, state);
            if (breach_) break;
            findIntersection(row, col, state);
        }

        // print out results
        if (intersections_.empty()) std::cout << "impossible" << '\n';
        else {
            sort(intersections_.begin(), intersections_.end(), compareLexicographically);
            std::cout << intersections_.size() << " " << intersections_[0].row + 1 << " " << intersections_[0].col + 1
                      << '\n';
        }
    }
}

std::string CrackSafe::updateState(const std::string& movement, const std::string& mirror_type, const std::string& state) {
    if (movement == "row") {
        if (mirror_type == "m") {
            if (state == "right") return "up";
            else return "down";
        }
        else {// mirror_type = "n"
            if (state == "right") return "down";
            else return "up";
        }
    }
    else {// movement == "col"
        if (mirror_type == "m") {
            if (state == "down") return "left";
            else return "right";
        }
        else {// mirror_type == "n"
            if (state == "down") return "right";
            else return "left";
        }
    }
}

bool CrackSafe::compareLexicographically(CrackSafe::Position pos1, CrackSafe::Position pos2) {
    if (pos1.row == pos2.row) return pos1.col < pos2.col;
    else return pos1.row < pos2.row;
}
