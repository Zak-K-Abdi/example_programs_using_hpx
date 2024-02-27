#include "hpx/hpx_init.hpp"
#include "hpx/hpx_finalize.hpp"
#include "hpx/parallel/algorithms/for_each.hpp"
#include "hpx/execution.hpp"
#include "hpx/parallel/algorithm.hpp"

#include <vector>
#include <iostream>

std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b) {
    std::size_t m = a.size(), p = b[0].size();
    std::vector<std::vector<int>> c(m, std::vector<int>(p, 0));
    auto calculate = [&c, a, b] (const std::size_t& i) {
        for (int j = 0; j < b[0].size(); j++) {
            for (int k = 0; k < a[0].size(); k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    };

    hpx::for_each(hpx::execution::par, hpx::util::counting_iterator<std::size_t>(0),
                  hpx::util::counting_iterator<std::size_t>(a.size()), calculate);

    return c;
}

int hpx_main() {
    std::vector<std::vector<int>> a = {{1, 2, 3},
                                       {4, 5, 6},
                                       {7, 8, 9}};
    std::vector<std::vector<int>> b = {{9, 8, 7},
                                       {6, 5, 4},
                                       {3, 2, 1}};

    std::vector<std::vector<int>> c = multiply(a, b);
    for (const auto& row : c) {
        for (const auto& i : row) {
            std::cout << i << ' ';
        }

        std::cout << '\n';
    }

    return hpx::finalize();
}

int main(int argv, char* argc[]) {
    return hpx::init(argv, argc);
}
