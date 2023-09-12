#include <iostream>
#include <vector>
#include <unordered_set>

int main()
{
    int n, k;
    std::cin >> n >> k;
    std::vector<int> v(n, 0);
    std::unordered_set<int> items;
    std::vector<size_t> last_idxes(k, 0);

    for (size_t i = 0; i < static_cast<size_t>(n); ++i)
    {
        std::cin >> v[i];
    }

    long long cur_sum = 0;
    long long ans = 250000000000;

    size_t l = 0, r = 0;

    while (l < v.size() && r < v.size())
    {
        while (r < v.size())
        {
            if (v[r] <= k)
            {
                items.insert(v[r]);
                last_idxes[v[r] - 1] = r;
            }
            cur_sum += v[r];
            ++r;
            if (items.size() == k)
            {
                break;
            }
        }

        while (l < v.size() && items.size() == k)
        {
            ans = std::min(ans, cur_sum);
            cur_sum -= v[l];
            if (v[l] <= k && l == last_idxes[v[l] - 1]) {
                items.erase(v[l]);
            }
            ++l;
        }
    }

    std::cout<<ans;
}