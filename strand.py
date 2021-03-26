
"""Python Scratchpaper. Strand.cc has full implementation."""


# First try solving this simpler problem. A DP approach happens to the trick nicely. Perhaps we could even find a library to solve this sub-problem more efficiently.

def shared_substring(s1, s2):
    table = [[0 for _ in range(len(s2))] for _ in range(len(s1))]
    # In our fast c++ implementation we can save table memory by cachine only relevant subproblems.

    for i in range(len(s1)):
        for j in range(len(s2)):
            if s1[i] != s2[j]:
                table[i][j] = 0
            elif i == 0 or j == 0:
                table[i][j] = 1
            else:
                table[i][j] = table[i-1][j-1] + 1

    return max([max(sub) for sub in table])


def test():

    # Test shared_substring
    s1 = "abcdfghbc"
    s2 = "ibcdegh"
    solution = shared_substring(s1, s2)
    assert solution == 3, solution

    s1 = "a21ffffffghi"
    s2 = "ibcdefghi000000"
    solution = shared_substring(s1, s2)
    assert solution == 4, solution

    print('all good')


def main():
    test()


if __name__ == "__main__":
    main()
