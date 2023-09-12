#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>

// Решение через AND и OR битовых масок почему-то падает на 5 тесте. 
// Пришлось написать другое решение, по логике делающее то же самое
// int get_mask_by_str(std::string mask_str)
// {
//     int num = 0;
//     for (size_t i = 0; i < mask_str.length(); ++i)
//     {
//         if (mask_str[i] == '.')
//         {
//             num += std::pow(2, i);
//         }
//     }

//     return num;
// }

// int main()
// {
//     int c;
//     std::cin >> c;
//     std::unordered_map<std::string, std::pair<int, std::unordered_map<std::string, int>>> data;
//     for (int i = 0; i < c; ++i)
//     {
//         std::string name;
//         int n;
//         std::cin >> name >> n;
//         for (int j = 0; j < n; ++j)
//         {
//             std::string mask_str, name_room;
//             std::cin >> mask_str >> name_room;
//             int mask = get_mask_by_str(mask_str);
//             data[name].second[name_room] = mask;
//         }

//         int union_num = 0;

//         for (auto key : data[name].second)
//         {
//             union_num |= key.second;
//         }
//         data[name].first = union_num;
//     }

//     int m;
//     std::cin >> m;
//     std::vector<std::string> ans(m);
//     for (int i = 0; i < m; ++i)
//     {
//         int n;
//         std::cin >> n;
//         std::vector<std::string> towns(n);
//         for (int j = 0; j < n; ++j)
//         {
//             std::cin >> towns[j];
//         }

//         int can = std::pow(2, 25) - 1;
//         for (int j = 0; j < n; ++j)
//         {
//         	if (data.find(towns[j]) == data.end()) {
//             	// can = 0;
//             	// break;
//                 continue;
//             }

//             can &= data[towns[j]].first;
//         }

//         if (can)
//         {
//             std::string cur_ans = "Yes";
//             for (int j = 0; j < n; ++j)
//             {

//                 for (auto it : data[towns[j]].second)
//                 {
//                     if (it.second & can)
//                     {
//                         cur_ans += " " + it.first;
//                         break;
//                     }
//                 }
//             }

//             ans[i] = cur_ans;
//         }
//         else
//         {
//             ans[i] = "No";
//         }
//     }

//     for (size_t i = 0; i < m; ++i)
//     {
//         std::cout << ans[i] << std::endl;
//     }

//     return 0;
// }

int main()
{
    int c;
    std::cin >> c;
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> data;
    for (int i = 0; i < c; ++i)
    {
        std::string name;
        int n;
        std::cin >> name >> n;
        for (int j = 0; j < n; ++j)
        {
            std::string mask_str, name_room;
            std::cin >> mask_str >> name_room;
            data[name].push_back({mask_str, name_room});
        }
    }

    int m;
    std::cin >> m;
    std::vector<std::string> ans(m);
    for (int i = 0; i < m; ++i)
    {
        int n;
        std::cin >> n;
        std::vector<std::string> towns(n);
        for (int j = 0; j < n; ++j)
        {
            std::cin >> towns[j];
        }
        bool ans_ok = false;
        std::vector<std::string> goods_global;
        for (int k = 0; k < 24; ++k)
        {
            bool local_ok = true;
            std::vector<std::string> goods;

            for (auto city : towns)
            {
                bool found_one = false;
                for (auto room : data[city])
                {
                    if (room.first[k] == '.')
                    {
                        found_one = true;
                        goods.push_back(room.second);
                        break;
                    }
                }

                if (!found_one)
                {
                    local_ok = false;
                    break;
                }
            }

            if (local_ok)
            {
                ans_ok = true;
                goods_global = goods;
                break;
            }
        }

        if (ans_ok)
        {
            std::string cur_ans = "Yes";
            for (int j = 0; j < goods_global.size(); ++j)
            {
                cur_ans += " " + goods_global[j];
            }
            ans[i] = cur_ans;
        }
        else
        {
            ans[i] = "No";
        }
    }

    for (size_t i = 0; i < m; ++i)
    {
        std::cout << ans[i] << std::endl;
    }

    return 0;
}