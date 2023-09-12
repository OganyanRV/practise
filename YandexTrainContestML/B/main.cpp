#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <cmath>
#include <set>
#include <cassert>
#include <random>
#include <iomanip>
#include <cstdlib>
#include <numeric>

const int MAX_K_TO_MODEL = 9;
const int SAMPLES_COUNT = 50;
const int SAMPLE_SIZE = 5000;
std::vector<std::vector<int>> cartesian(std::vector<std::vector<int>> &v)
{
    auto product = [](long long a, std::vector<int> &b)
    { return a * b.size(); };
    const long long N = accumulate(v.begin(), v.end(), 1LL, product);
    std::vector<int> u(v.size());
    std::vector<std::vector<int>> res;
    for (long long n = 0; n < N; ++n)
    {
        lldiv_t q{n, 0};
        for (long long i = v.size() - 1; 0 <= i; --i)
        {
            q = std::div(q.quot, v[i].size());
            u[i] = v[i][q.rem];
        }
        // Do what you want here with u.
        res.push_back(u);
        // for( int x : u ) cout << x << ' ';
        // cout << '\n';
    }

    return res;
}
std::vector<std::vector<int>> cartesian_product(const std::vector<int> &uniq_a, int k)
{
    std::vector<std::vector<int>> sets(k);
    for (int i = 0; i < k; ++i)
    {
        sets[i] = uniq_a;
    }

    return cartesian(sets);
}

std::vector<std::vector<int>> generate_sample(const std::unordered_map<int, double> &probs_edge, int k, int count)
{
    std::vector<std::vector<int>> samples(count, std::vector<int>(k, 0));
    std::vector<int> edges(probs_edge.size());
    std::vector<double> probs(probs_edge.size());
    int i = 0;
    for (auto it : probs_edge)
    {
        edges[i] = it.first;
        probs[i++] = it.second;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(probs.begin(), probs.end());

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < k; ++j)
        {
            samples[i][j] = edges[d(gen)];
        }
    }

    return samples;
}

int main()
{

    std::array<int, 6> a;
    int k;
    for (int i = 0; i < 6; ++i)
    {
        std::cin >> a[i];
    }
    std::cin >> k;
    std::unordered_map<int, double> probs_edge;
    for (auto val : a)
    {
        ++probs_edge[val];
    }

    for (auto it : probs_edge)
    {
        probs_edge[it.first] /= a.size();
    }

    std::set<int> uniq_a_set{a.begin(), a.end()};
    std::vector<int> uniq_a;
    std::copy(uniq_a_set.begin(), uniq_a_set.end(), std::back_inserter(uniq_a));
    if (k <= MAX_K_TO_MODEL)
    {
        std::vector<std::vector<int>> all_combinations;
        all_combinations = cartesian_product(uniq_a, k);
        std::vector<double> comb_probs(all_combinations.size());
        for (size_t i = 0; i < all_combinations.size(); ++i)
        {
            double prob = 1;
            for (auto val : all_combinations[i])
            {
                prob *= probs_edge[val];
            }
            comb_probs[i] = prob;
        }

        std::vector<int> comb_score(all_combinations.size());
        for (size_t i = 0; i < all_combinations.size(); ++i)
        {
            int cur_score = all_combinations[i][0];
            for (int j = 1; j < k; ++j)
            {
                if (all_combinations[i][j] != all_combinations[i][j - 1])
                {
                    cur_score += all_combinations[i][j];
                }
            }
            comb_score[i] = cur_score;
        }

        double expected_value = 0;
        for (int i = 0; i < comb_score.size(); ++i)
        {
            expected_value += comb_probs[i] * comb_score[i];
        }
        std::cout << std::setprecision(10) << expected_value << std::endl;
    }
    else
    {
        std::vector<double> means_of_samples(SAMPLES_COUNT);

        for (int j = 0; j < SAMPLES_COUNT; ++j)
        {
            std::vector<std::vector<int>> all_combinations;
            all_combinations = generate_sample(probs_edge, k, SAMPLE_SIZE);
            std::vector<double> comb_probs(all_combinations.size());

            double prob_sum = 0.0;
            for (size_t i = 0; i < all_combinations.size(); ++i)
            {
                double prob = 1;
                for (auto val : all_combinations[i])
                {
                    prob *= probs_edge[val] * 5;
                }
                comb_probs[i] = prob;
                prob_sum += prob;
            }

            for (size_t i = 0; i < all_combinations.size(); ++i)
            {
                comb_probs[i] /= prob_sum;
            }

            std::vector<int> comb_score(all_combinations.size());
            for (size_t i = 0; i < all_combinations.size(); ++i)
            {
                int cur_score = all_combinations[i][0];
                for (int j = 1; j < k; ++j)
                {
                    if (all_combinations[i][j] != all_combinations[i][j - 1])
                    {
                        cur_score += all_combinations[i][j];
                    }
                }
                comb_score[i] = cur_score;
            }

            double expected_value = 0;
            for (int i = 0; i < comb_score.size(); ++i)
            {
                expected_value += comb_probs[i] * comb_score[i];
            }
            means_of_samples[j] = expected_value;
        }
        double sum_expected_value = 0;
        for (int i = 0; i < SAMPLES_COUNT; ++i)
        {
            sum_expected_value += means_of_samples[i];
        }

        std::cout << std::setprecision(10) << sum_expected_value / SAMPLES_COUNT << std::endl;
    }
}