#include <string>
#include "strand.cc"

int test1()
{
    std::string s1 = "abcdfghbc";
    std::vector<char> v1;
    for (char c : s1)
    {
        v1.push_back(c);
        std::cout << c << '\n';
    }

    std::string s2 = "ibcdegh";
    std::vector<char> v2;
    for (char c : s2)
    {
        v2.push_back(c);
        std::cout << c << '\n';
    }
    std::cout << v1.size() << " " << v2.size() << '\n';
    std::vector<int> result = longest_common_strand(v1, v2);
    if (!(result[0] == 1 & result[1] == 1 & result[2] == 3))
    {
        fprintf(stderr, "wrong values %d %d %d", result[0], result[1], result[2]);
    }

    s1 = "a21ffffffghi";
    v1.clear();
    for (char c : s1)
    {
        v1.push_back(c);
    }

    s2 = "ibcdefghi000000";
    v2.clear();
    for (char c : s2)
    {
        v2.push_back(c);
    }

    result = longest_common_strand(v1, v2);
    if (!(result[0] == 8 & result[1] == 5 & result[2] == 4))
    {
        fprintf(stderr, "wrong values %d %d %d", result[0], result[1], result[2]);
    }
}

int main()
{
    test1();
}