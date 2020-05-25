#include "helpers.h"

namespace helpers {

    std::string rtrim(const std::string &str) {
        std::string s(str);
        s.erase(
                find_if(s.rbegin(), s.rend(), not1(std::ptr_fun<int, int>(isspace))).base(),
                s.end()
        );
        return s;
    }

    std::vector<std::string> split(const std::string &str) {
        std::vector<std::string> tokens;
        std::string::size_type start = 0;
        std::string::size_type end = 0;
        while ((end = str.find(" ", start)) != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }
}
