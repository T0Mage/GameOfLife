#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <boost/program_options.hpp>
#include <memory>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <sstream>
//used 7.4 g++ compiler for
//#include <experimental/filesystem>
#include <SFML/Graphics.hpp>
#include <filesystem>
using namespace std;
using namespace boost::program_options;
inline int b(int i, int N);

void SetVariables(int &x, int &y, int &xsize, int &ysize, int &iter, float &speed, string &Patern, int &argc, const char **argv);
void Next(std::vector<std::vector<bool>> &Board);
template <bool Use_GUI>
void Draw_result(std::vector<std::vector<bool>> const &Board, float speed);
void Place(std::vector<std::vector<bool>> &Board, int x, int y, const std::vector<std::vector<bool>> &pattern);

inline string ReadFromFile(string filename, int &xpattern, int &ypattern);
void Decoding(std::vector<std::vector<bool>> &Board, string &repr, int x, int y);
void SaveToFile(vector<vector<bool>> Board, string filename);

template <>
void Draw_result<false>(std::vector<std::vector<bool>> const &Board, float speed)
{
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << "\n";
    }

    for (int i = 0; i < (int)Board.size(); i++)
    {
        for (int j = 0; j < (int)Board[0].size(); j++)
        {
            if (Board[i][j])
                std::cout << 0;
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speed)));
}
constexpr int cells_size = 8;
constexpr int cells_distance = 1;

template <>
void Draw_result<true>(std::vector<std::vector<bool>> const &Board, float speed)
{

    static sf::RenderWindow window(sf::VideoMode((Board.size()+1)*(cells_size+cells_distance), (Board[0].size()+1)*(cells_size+cells_distance)), "GameOfLife");
    sf::RectangleShape shape(sf::Vector2f(cells_size, cells_size));
    sf::RectangleShape temp(sf::Vector2f(cells_size, cells_size));
    shape.setFillColor(sf::Color::Green);
    shape.setFillColor(sf::Color::White);

 //   window.setFramerateLimit(speed);
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    window.clear();

    for (int i = 0; i < (int)Board.size(); i++)
    {
        for (int j = 0; j < (int)Board[0].size(); j++)
        {
            if (Board[i][j])
            {
                temp = shape;
                temp.move(i * (cells_size + cells_distance), j * (cells_size + cells_distance));
                window.draw(temp);
            }
        }
        std::cout << "\n";
    }
    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speed)));
}
void Place(std::vector<std::vector<bool>> &Board, int x, int y, const std::vector<std::vector<bool>> &pattern)
{
    for (int i = 0; i < (int)pattern.size(); i++)
    {
        for (int j = 0; j < (int)pattern[0].size(); j++)
        {
            Board[x + i][y + j] = pattern[i][j];
            // cout<<pattern[i][j];
        }
        // cout<<"\n";
    }
}

template <bool Use_GUI>
void GameOfLife(int argc, char const *argv[]);
template <>
void GameOfLife<true>(int argc, char const *argv[])
{
    //values not used
    int x = 0, y = 0, xsize = 11, ysize = 11, iter = 1, xpattern = 3, ypattern = 3;
    float speed = 0;
    string map;
    //cli
    SetVariables(x, y, xsize, ysize, iter, speed, map, argc, argv);
    string repr = ReadFromFile(map, xpattern, ypattern);
    std::vector<std::vector<bool>> Board(xsize, std::vector<bool>(ysize, 0));

    Decoding(Board, repr, x, y);
    Draw_result<1>(Board, speed);
    for (int i = 0; i < iter; i++)
    {
        Next(Board);
        Draw_result<1>(Board, speed);
    }

    SaveToFile(Board, "save.txt");
}
template <>
void GameOfLife<false>(int argc, char const *argv[])
{
    //values not used
    int x = 0, y = 0, xsize = 11, ysize = 11, iter = 1, xpattern = 3, ypattern = 3;
    float speed = 0;
    string map;
    //cli
    SetVariables(x, y, xsize, ysize, iter, speed, map, argc, argv);
    string repr = ReadFromFile(map, xpattern, ypattern);
    std::vector<std::vector<bool>> Board(xsize, std::vector<bool>(ysize, 0));

    Decoding(Board, repr, x, y);
    Draw_result<0>(Board, speed);
    for (int i = 0; i < iter; i++)
    {
        Next(Board);
        Draw_result<0>(Board, speed);
    }

    SaveToFile(Board, "save.txt");
}

constexpr bool draw_gui = 1;

int main(int argc, char const *argv[])
{
    GameOfLife<draw_gui>(argc, argv);
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
    for (int i = 0; i < (int)Board.size(); i++)
    {
        for (int j = 0; j < (int)Board[0].size(); j++)
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
//void Draw_result(std::vector<std::vector<bool>> const &Board, float speed = 1);

void SetVariables(int &x, int &y, int &xsize, int &ysize, int &iter, float &speed, string &map, int &argc, const char **argv)
{
    try
    {
        options_description desc{"Options"};
        desc.add_options()("help,h", "Help screen")("x,x", value<int>()->default_value(1), "glider x offset")("y,y", value<int>()->default_value(1), "glider y offset")("xsize,W", value<int>()->default_value(20), "width")("ysize,H", value<int>()->default_value(20), "hight")("iter,i", value<int>()->default_value(10), "max iteration")("speed,s", value<float>()->default_value(2.0f), "speed of simulation")("map,m", value<string>()->default_value("Copperhead.txt"), "name of file containing map");
        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
        if (vm.count("help"))
        {
            cout << desc << "\n";
            exit(0);
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
        if (vm.count("map"))
            map = vm["map"].as<string>();
    }
    catch (const error &e)
    {
        std::cerr << e.what() << '\n';
    }
}

string ReadFromFile(string filename, int &xpattern, int &ypattern)
{

    string repr = {};
    string line = {};
    stringstream ss;
    try
    {
        // filesystem::path path= "/"+filename;
        // if(filesystem::exists(filesystem::path( path))){
        //     cout<<"file doesn't exist";
        //     exit(1);
        // }
        string temps;
        ifstream file(filename);
        bool first_line = 1;
        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (line[0] != string("#")[0])
                {
                    if (first_line)
                    {
                        ss << line;
                        while (!ss.eof())
                        {
                            ss >> temps;
                            if (temps == "x")
                            {
                                ss >> temps;
                                if (temps == "=")
                                {
                                    ss >> xpattern;
                                }
                            }
                            if (temps == "y")
                            {
                                ss >> temps;
                                if (temps == "=")
                                {
                                    ss >> ypattern;
                                }
                            }
                            if (temps == "rule")
                            {
                                ss >> temps;
                                if (temps == "=")
                                {
                                    ss >> temps;
                                    if (temps != "B3/S23")
                                    {
                                        cout << "Unsupported ruleset\n";
                                        exit(1);
                                    }
                                }
                            }
                        }
                        //
                        // cout << line << '\n';
                        first_line = 0;
                    }
                    else
                    {
                        repr.append(line);
                    }
                }
            }
            file.close();
        }
        return repr;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

void Decoding(std::vector<std::vector<bool>> &Board, string &repr, int x = 0, int y = 0)
{
    vector<string> splited = {};
    vector<string> param;
    boost::split(splited, repr, boost::is_any_of("$"));

    int ile = 1;
    int tempi = 1;
    int row = x;
    for (auto &&i : splited)
    {
        string temp = "";
        int offset = y;

        if (i != "")
            for (int j = 0; j < (int)i.size(); j++)
            {

                if (i[j] != 'b' && i[j] != 'o')
                {
                    temp += i[j];
                }
                else
                {
                    if (!temp.empty())
                        ile = stoi(temp);
                    else
                        ile = 1;
                    if (i[j] == 'b')
                    {
                        offset += ile;
                        temp = "";
                    }

                    if (i[j] == 'o')
                    {
                        for (int end = offset + ile; offset < end; offset++)
                        {
                            Board[row][offset] = 1;
                        }
                        temp = "";
                    }
                }
                if (i[j] == i.back() && isdigit(i[j]))
                {
                    tempi = i[j] - '0';
                }
                else
                {
                    tempi = 1;
                }
            }
        row += tempi;
    }
}

void SaveToFile(vector<vector<bool>> Board, string filename = "save.txt")
{
    try
    {
        ofstream file;
        file.open(filename);
        file << "x = " << Board.size() << ", y = " << Board[0].size() << "\n";
        string repr = "";
        bool last = {};
        for (size_t i = 0; i < Board.size(); i++)
        {
            size_t counter = 0;
            //check if first line empty
            if (all_of(Board[i].begin(), Board[i].end(), [](int i) { return i == 0; }))
            {
                repr += '$';
                i++;
            }

            for (size_t j = 0; j < Board[0].size(); j++)
            {
                //first char
                if (j == 0)
                {
                    counter = 1;
                    last = Board[i][j];
                    continue;
                }
                //same character as last
                if (Board[i][j] == last)
                {
                    counter++;

                    //last character
                    if (j == Board[0].size() - 1)
                    {
                        //relevant
                        if (Board[i][j])
                        {
                            if (counter == 1)
                            {
                                repr += 'o';
                            }
                            if (counter > 1)
                            {
                                repr += (char)('0' + counter);
                                repr += 'o';
                            }
                        }
                        repr += '$';
                        //finish
                    }
                }
                //diffrent char
                else
                {
                    //not last
                    if (j < Board[0].size() - 1)
                    {
                        //save
                        if (last)
                        {
                            if (counter > 1)
                                repr += (char)('0' + counter);
                            repr += 'o';
                        }
                        else
                        {
                            if (counter > 1)
                                repr += (char)('0' + counter);
                            repr += 'b';
                        }
                        //reset state
                        last = Board[i][j];
                        counter = 1;
                    }
                    //last
                    else
                    {
                        if (last)
                        {
                            if (counter > 1)
                                repr += (char)('0' + counter);
                            repr += 'o';
                        }
                        else
                        {
                            if (counter > 1)
                                repr += (char)('0' + counter);
                            repr += 'b';
                        }
                        repr += '$';
                    }
                }
            }
        }
        int moneycounter = 0;
        string result = "";
        for (size_t i = 0; i < repr.size(); i++)
        {
            if (repr[i] != '$')
            {
                result += repr[i];
            }
            while (repr[i] == '$')
            {
                moneycounter++;
                i++;
            }
            if (moneycounter == 1)
            {
                result += '$';
                moneycounter = 0;
            }
            if (moneycounter != 0)
            {
                result += ((char)moneycounter + '0');
                result += '$';

                moneycounter = 0;
            }
        }

        file << result;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
