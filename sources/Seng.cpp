#include "../headers/Seng.h"
#include <iostream>
#include <fstream>
#include <cstdlib>


void Seng::stringToImage(const std::string &imagePath, const std::string &textPath, const std::string& text, const std::string &password) {
    std::ofstream out;
    out.open(textPath);
    out << text;
    out.close();
    std::string embedCommand = "steghide embed -cf " + imagePath + " -ef " + textPath + " -f -p " + password;
    system(embedCommand.c_str());
    remove(textPath.c_str());
}

std::string Seng::imageToString(const std::string& imagePath, const std::string& password) {
    std::string textPath = imagePath + ".txt";
    std::string extractCommand = "steghide extract --stegofile " + imagePath + " -xf " + textPath + " -f -p " + password;
    system(extractCommand.c_str());
    std::ifstream in;
    in.open(textPath + ".txt");
    std::string text((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());
    remove(textPath.c_str());
    in.close()
    return text;
}






