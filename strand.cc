#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

#define vec std::vector<int>

template <class T>
void printVec(std::vector<T> v) //debugging
{
    for (T a : v)
        std::cout << a << " ";
    std::cout << '\n';
}

/*
Get the longest common byte strand between two individual files represented as char vectors.
out[0] = offset in file 1, out[1] = offset in file 2, out[2] = length of common strand
*/
vec longest_common_strand(std::vector<char> &f1, std::vector<char> &f2)
{
    /*
    A DP approach works well here. 

    A naive solution is to check all pairs of substrands, and we can arrive at the DP solution intuitively from the naive solution by noticing that there are lots of redundancies in the naive computation. For example, if we have two files (representing bytes as chars) 'abcde' and 'abcdf' and we find that they both contain the common substrand 'abc', it wouldn't make sense for us to then verify the common substrand 'abcd' in its entirety since this would mean re-checking the first three bytes 'abc' again. Instead it is better to cache the knowledge that the length of the longest common substrand ending at the 'c' in both files is 3, and then we can check from there if the 'd's without having to parse the entire substrand again. 

    Since we also want to get the starting indices, it makes more sense to check the substrands in reverse order. 
    */

    std::vector<vec> table(2); //the DP subproblem table. Table[i%2][j] = length of longest common substrand that begins at index i in f1 and index j in f2. For one file, we can forget all but the last two indices since the only subproblem we rely on for computing the (i,j)th subproblem is the (i+1,j+1)th subproblem. If we want to be more memory frugal we could put the larger file as the first argument. The mod operator allows us to imagine we have a full size table by pretending Table[i%2][j] ~ Table[i][j].

    table[0].resize(f2.size(), 0);
    table[1].resize(f2.size(), 0);

    int longest(0), index1(0), index2(0);

    for (int i = f1.size() - 1; i >= 0; i--) //filling the subproblem table backwards
    {
        for (int j = f2.size() - 1; j >= 0; j--)
        {
            if (f1[i] != f2[j])
            {
                table[i % 2][j] = 0; //Substrings don't start the same, therefore they cannot match.
            }
            else if (i == f1.size() - 1 || j == f2.size() - 1)
            {
                table[i % 2][j] = 1; // Boundary case.
            }
            else
            {
                table[i % 2][j] = table[(i + 1) % 2][j + 1] + 1; //Adding an extra byte to the front of a previously checked case.
            }
            if (table[i % 2][j] > longest)
            {
                longest = table[i % 2][j];
                index1 = i;
                index2 = j;
            }
        }
    }

    vec result = {index1, index2, longest};
    return result;
}

/*
Given a directory, find the longest common pairwise strand between at least two files in the given directory. 

Input name = directory to search. 
Output 'out' is of the form
out[0] = file #1, out[1] = byte offset in file #1
out[2] = file #2, out[3] = byte offset in file #2
out[4] = length of longest common substrand. 
*/

std::vector<std::string> longest_pw_strand(std::string dir)
{
    std::vector<std::string> result(5, "0");

    std::vector<std::filesystem::directory_entry> files;
    for (auto &f : std::filesystem::directory_iterator(dir))
        files.push_back(f);

    //Put largest files first since this will use less memory in the DP table.
    std::sort(files.begin(), files.end(), [](auto f1, auto f2) { return f1.file_size() > f2.file_size(); });

    for (int i = 0; i < files.size(); i++)
    {
        std::ifstream file(files[i].path(), std::ifstream::binary);
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        std::vector<char> f1(length);
        file.read(f1.data(), length);

        for (int j = i + 1; j < files.size(); j++)
        {
            std::ifstream file(files[j].path(), std::ifstream::binary);
            file.seekg(0, file.end);
            int length = file.tellg();
            file.seekg(0, file.beg);

            std::vector<char> f2(length);
            file.read(f2.data(), length);
            std::vector<int> pw_result = longest_common_strand(f1, f2); //Find longest strand betweenf1, f2
            if (pw_result[2] > std::stoi(result[4]))                    //check if it's the longest so far.
            {

                result[4] = std::to_string(pw_result[2]);
                result[0] = files[i].path().filename().string();
                result[1] = std::to_string(pw_result[0]);
                result[2] = files[j].path().filename().string();
                result[3] = std::to_string(pw_result[1]);
            }
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Please enter a directory\n";
        return 0;
    }

    auto start = std::chrono::system_clock::now();
    std::vector<std::string> result = longest_pw_strand(argv[1]);
    auto stop = std::chrono::system_clock::now();

    std::cout << result[0] << " " << result[1] << " " << result[2] << " " << result[3] << " " << result[4] << '\n';
    std::cout << "Found result in " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds" << '\n';
    return 0;
}