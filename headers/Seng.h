//
// Created by khloud ghattas on 12/4/19.
//

#ifndef PROJ_SENG_H
#define PROJ_SENG_H
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <cstring>
using namespace std;

class Seng
{
public:
     Seng();
    ~Seng();
    static void StringToImage(std::string imagePath, std::string text, std::string password);
    static string ImageToString(string imagePath, string password);
};











#endif //PROJ_SENG_H
