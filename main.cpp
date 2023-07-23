#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class grid {
public:
    friend std::ostream& operator<<(std::ostream& o, grid const& g);

    grid(int rows, int columns)
        : rows_ {rows},
          columns_ {columns} {

        data_.resize(rows_ * columns_, '#');
    }

    void change(int row, int column, char value) {
        data_[row * columns_ + column] = value;

        if(value == 'E') {
            end_.first = row;
            end_.second = column;
        }
    }

    std::vector<std::pair<int, int>> solve_dfs() {
        std::vector<std::pair<int, int>> paths;
        paths.resize(rows_ * columns_);

        std::vector<bool> visited;
        visited.resize(rows_ * columns_);

        dfs(0, 0, visited, paths);

        std::vector<std::pair<int, int>> results;

        if(visited[end_.first * columns_ + end_.second]) {
            auto v = end_;
            auto s = std::make_pair(0, 0);

            while(v != s) {
                results.push_back(v);
                v = paths[v.first * columns_ + v.second];
            }
            results.push_back(s);
            std::reverse(results.begin(), results.end());
        }

        return results;
    }

    std::vector<std::pair<int, int>> solve_bfs() {
        std::vector<std::pair<int, int>> paths;
        paths.resize(rows_ * columns_);

        std::vector<bool> visited;
        visited.resize(rows_ * columns_);

        bfs(0, 0, visited, paths);

        std::vector<std::pair<int, int>> results;

        if(visited[end_.first * columns_ + end_.second]) {
            auto v = end_;
            auto s = std::make_pair(0, 0);

            while(v != s) {
                results.push_back(v);
                v = paths[v.first * columns_ + v.second];
            }
            results.push_back(s);
            std::reverse(results.begin(), results.end());
        }

        return results;
    }

private:
    bool valid(int row, int col) const noexcept {
        return (row >= 0 && row < rows_ && col >= 0 && col < columns_);
    }

    void dfs(int row, int column, std::vector<bool>& visited,
             std::vector<std::pair<int, int>>& path) {

        auto row_stride_offset = row * columns_;
        auto row_stride_column_offset = row_stride_offset + column;

        visited[row_stride_column_offset] = true;

        if(data_[row_stride_column_offset] == '#') { return; }

        for(auto i = 0; i < 4; ++i) {
            auto adj_row = (row + row_adj_[i]);
            auto adj_col = (column + col_adj_[i]);

            auto adj_row_stride_offset = adj_row * columns_;
            auto adj_row_stride_column_offset = adj_row_stride_offset + adj_col;

            if(valid(adj_row, adj_col) && !visited[adj_row_stride_column_offset]) {
                dfs(adj_row, adj_col, visited, path);
                path[adj_row_stride_column_offset] = std::make_pair(row, column);
            }
        }
    }

    void bfs(int row, int column, std::vector<bool>& visited,
             std::vector<std::pair<int, int>>& path) {

        auto row_stride_offset = row * columns_;
        auto row_stride_column_offset = row_stride_offset + column;

        visited[row_stride_column_offset] = true;

        std::queue<std::pair<int, int>> q {};
        q.push(std::make_pair(row, column));

        while(!q.empty()) {

            auto v = q.front();
            q.pop();

            if(data_[v.first * columns_ + v.second] == '#') { continue; }

            for(auto i = 0; i < 4; ++i) {
                auto adj_row = (v.first + row_adj_[i]);
                auto adj_col = (v.second + col_adj_[i]);

                auto adj_row_stride_offset = adj_row * columns_;
                auto adj_row_stride_column_offset = adj_row_stride_offset + adj_col;

                if(valid(adj_row, adj_col) && !visited[adj_row_stride_column_offset]) {
                    q.push(std::make_pair(adj_row, adj_col));
                    visited[adj_row_stride_column_offset] = true;
                    path[adj_row_stride_column_offset] = std::make_pair(v.first, v.second);
                }
            }
        }
    }

    int rows_ {};
    int columns_ {};
    std::pair<int, int> end_ {};
    std::vector<char> data_;

    // left , up, right, down
    static constexpr std::array<int, 4> row_adj_ {0, -1, 0, 1};
    static constexpr std::array<int, 4> col_adj_ {-1, 0, 1, 0};
};

std::ostream& operator<<(std::ostream& o, grid const& g) {
    for(auto r = 0; r < g.rows_; ++r) {
        for(auto c = 0; c < g.columns_; ++c) { o << g.data_[r * g.columns_ + c] << " "; }
        o << "\n";
    }
    return o;
}

int main(int argc, char** argv) {
    grid g {5, 7};
    g.change(0, 0, 'S');
    g.change(1, 0, '.');
    g.change(2, 0, '.');
    g.change(3, 0, '.');

    g.change(0, 1, '.');
    g.change(3, 1, '.');
    g.change(4, 1, '.');

    g.change(0, 2, '.');
    g.change(1, 2, '.');
    g.change(2, 2, '.');

    g.change(1, 3, '.');
    g.change(2, 3, '.');
    g.change(4, 3, 'E');

    g.change(0, 4, '.');
    g.change(2, 4, '.');
    g.change(3, 4, '.');
    g.change(4, 4, '.');

    g.change(0, 5, '.');
    g.change(1, 5, '.');
    g.change(2, 5, '.');
    g.change(3, 5, '.');

    g.change(0, 6, '.');
    g.change(1, 6, '.');
    g.change(2, 6, '.');
    g.change(3, 6, '.');
    g.change(4, 6, '.');

    std::cout << g << std::endl;

    std::cout << "Path to solve using dfs: ";
    auto dfs_path = g.solve_dfs();

    for(auto const& [f, s] : dfs_path) { std::cout << "[" << f << ", " << s << "]\n"; }
    std::cout << std::endl;

    std::cout << "Path to solve using bfs: ";
    auto bfs_path = g.solve_bfs();

    for(auto const& [f, s] : bfs_path) { std::cout << "[" << f << ", " << s << "]\n"; }
    std::cout << std::endl;
}
