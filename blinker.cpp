#include <iostream>
#include <array>
#include <chrono>
#include <thread>
// #include <Array.cpp>
// #include <stdlib.h>
constexpr int N = 10;
int b(int i)
{
    if (i < 0)
        return N - 1;
    if (i >= N)
        return 0;
    return i;
}
void Next(std::array<std::array<bool, 10>, 10> &Board)
{
    std::array<std::array<bool, 10>, 10> BoardBefore = Board;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sum = 0;

            for (int ii = i - 1; ii <= i + 1; ii++)
            {
                for (int jj = j - 1; jj <= j + 1; jj++)
                {
                    if (!(jj == j && ii==i))
                    {
                        sum += BoardBefore[b(ii)][b(jj)];
                    }
                }
            }
            if (BoardBefore[i][j])
            {
                if (sum < 2)
                    Board[i][j] = 0;
                if (sum == 2 or sum == 3)
                    Board[i][j] = 1;
                if (sum > 3)
                    Board[i][j] = 0;
            }
            else
            {
                if (sum == 3)
                {
                    Board[i][j] = 1;
                }
            }
        }
    }
}

void Draw(std::array<std::array<bool, 10>, 10> Board)
{
    // system ("CLS");
    // system("clear");
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    {

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                std::cout << Board[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(777));


}

int main(int argc, char const *argv[])
{
    //Glider:

    // std::array<std::array<bool, 10>, 10> data = {{
    //     {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    //     {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // }};

    //Blinker

    std::array<std::array<bool, 10>, 10> data = {{
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }};
    for (int i = 0; i < 20; i++)
    {
        Draw(data);
        Next(data);
    }

    return 0;
}
