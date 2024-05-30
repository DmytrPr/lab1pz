
#include <tuple>
#include <string>
#include <vector>
#include <cassert>


#include <iostream>
#include <fstream>
#include <sstream>

struct TwoBitValue {
    uint8_t value : 2;
};

auto read_input_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "can't open file at " << file_path << std::endl;
        throw new std::exception();
    }

    std::vector<std::vector<std::vector<TwoBitValue>>> boards;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        
        if(line == "\n" || line == "\r\n" || line.empty()) continue;

        std::vector<std::vector<TwoBitValue>> board(19, std::vector<TwoBitValue>(19));
        for (int i = 0; i < 19; ++i) {
            std::getline(file, line);
            
            if(line == "\n" || line == "\r\n" || line.empty()) continue;

            for (int j = 0; j < 19; ++j) {
                board[i][j].value = line[j] - '0';
            }
        }
        
        boards.push_back(board);
    }

    file.close();
    return boards;
}

std::tuple<int, int, int> check_victory_condition(const std::vector<std::vector<TwoBitValue>>& board) {
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};
    int n = board.size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j].value != 0) {
                int color = board[i][j].value;
                for (auto& [dx, dy] : directions) {
                    int count = 1;

                    // Check in the positive direction
                    int x = i + dx, y = j + dy;
                    while (x >= 0 && x < n && y >= 0 && y < n && board[x][y].value == color) {
                        count++;
                        x += dx;
                        y += dy;
                    }

                    // Check in the negative direction
                    x = i - dx;
                    y = j - dy;
                    while (x >= 0 && x < n && y >= 0 && y < n && board[x][y].value == color) {
                        count++;
                        x -= dx;
                        y -= dy;
                    }

                    if (count >= 5) {
                        return {color, i + 1, j + 1}; // return 1-based index
                    }
                }
            }
        }
    }

    return {0, 0, 0};
}

void write_result(const std::string& file_path, const std::vector<std::tuple<int, int, int>>& results) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }
    for(const auto& result : results)
        file << std::get<0>(result) << "\n" << std::get<1>(result) << " " << std::get<2>(result) << "\n\n";

    file.close();
}

int main() {
    auto boards = read_input_file("input.txt");
    
    std::vector<std::tuple<int, int, int>> res;
    res.reserve(boards.size());

    for (const auto& board : boards) {
        res.push_back(check_victory_condition(board));
    }

    write_result("output.txt", res);
    return 0;
}