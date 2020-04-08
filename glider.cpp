#include <iostream>
#include <chrono>
#include <thread>
#include <boost/program_options.hpp>
#include <memory>
using namespace std;
using namespace boost::program_options;
inline int b(int i, int N);
void Next(std::vector<std::vector<bool>> &Board);
void Draw(std::vector<std::vector<bool>> const &Board, float speed);
void Place(std::vector<std::vector<bool>> &Board, int x, int y, const std::vector<std::vector<bool>> &pattern);

int main(int argc, char const *argv[])
{
    int x , y , xsize , ysize , iter;
    float speed ;

    try
    {
        options_description desc{"Options"};
        desc.add_options()
        ("help,h", "Help screen")
        ("x,x", value<int>()->default_value(1), "glider x offset")
        ("y,y", value<int>()->default_value(1), "glider y offset")
        ("xsize,W", value<int>()->default_value(10), "width")
        ("ysize,H", value<int>()->default_value(10), "hight")
        ("iter,i", value<int>()->default_value(10), "max iteration")
        ("speed,s", value<float>()->default_value(2.0f), "speed of simulation");
        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
        if (vm.count("help"))
        {
            cout << desc << "\n";
            return 0;
        }
        if (vm.count("x"))
            x = vm["x"].as<int>();
        if (vm.count("y"))
            y = vm["y"].as<int>();
        if (vm.count("xsize"))
            xsize = vm["xsize"].as<int>();
        if (vm.count("ysize"))
            ysize = vm["ysize"].as<int>();
        if (vm.count("iter"))
            iter = vm["iter"].as<int>();
        if (vm.count("speed"))
            speed = vm["speed"].as<float>();
    }
    catch (const error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
    cout<<speed;
    std::vector<std::vector<bool>> Board(xsize, std::vector<bool>(ysize, 0));
    std::vector<std::vector<bool>> glider = {
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1}};

    Place(Board, x, y, glider);

    for (int i = 0; i < iter; i++)
    {
        Draw(Board, speed);
        Next(Board);
    }

    return 0;
}

inline int b(int i, int N = 10)
{
    if (i < 0)
        return N - 1;
    if (i >= N)
        return 0;
    return i;
}
void Next(std::vector<std::vector<bool>> &Board)
{
    std::vector<std::vector<bool>> BoardBefore(Board);
    for (int i = 0; i < Board.size(); i++)
    {
        for (int j = 0; j < Board[0].size(); j++)
        {
            int sum = 0;

            for (int ii = i - 1; ii <= i + 1; ii++)
            {
                for (int jj = j - 1; jj <= j + 1; jj++)
                {
                    if (!(jj == j && ii == i))
                    {
                        sum += BoardBefore[b(ii, Board.size())][b(jj, Board[0].size())];
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

void Draw(std::vector<std::vector<bool>> const &Board, float speed = 1)
{
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << "\n";
    }

    for (int i = 0; i < Board.size(); i++)
    {
        for (int j = 0; j < Board[0].size(); j++)
        {
            std::cout << Board[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speed)));
}

void Place(std::vector<std::vector<bool>> &Board, int x, int y, const std::vector<std::vector<bool>> &pattern)
{
    for (int i = 0; i < pattern.size(); i++)
    {
        for (int j = 0; j < pattern[0].size(); j++)
        {
            Board[x + i][y + j] = pattern[i][j];
            // cout<<pattern[i][j];
        }
        // cout<<"\n";
    }
}
