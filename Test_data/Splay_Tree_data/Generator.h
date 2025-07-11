#pragma once
#include <vector>

class Generator {
public:
    //elementy po vozrastaniyu
    static void lineTest1(int size, std::vector<int>& arr, bool file);
    //elementy po ubyvaniyu
    static void lineTest2(int size, std::vector<int>& arr, bool file);
    //chereduyushiesya dannie po vozrastaniyu
    static void twoLineTest(int size, std::vector<int>& arr, bool file);
    //sluchaynie dannie
    static void randTest(int size, int max, int min, std::vector<int>& arr, bool file);
    //zubchatie dannie
    static void zubTest(int size, std::vector<int>& arr, bool file);
    //generating stress test data with big length 
    static void stressSizeTest(std::vector<int>& arr, bool file);

};