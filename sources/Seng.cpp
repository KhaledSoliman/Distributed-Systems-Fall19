#include "../headers/Seng.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

Seng::Seng() {

}

void Seng::StringToImage(const std::string &imagePath, const std::string &textPath, const std::string &password) {
    std::ofstream outfile;
    outfile.open(imagePath);
    outfile << textPath;
    outfile.close();
    // -cf ---> coverfile filename
    // Specify the cover file that will be used to embed data.
    std::string embedCommand = "steghide embed -cf " + imagePath + " -ef " + textPath + " -f -p " + password;
    system(embedCommand.c_str());
    std::string delCommand = "rm " + textPath;
    system(delCommand.c_str());
}

std::string Seng::ImageToString(const std::string& imagePath,const std::string& password) {
    std::string extractCommand = "steghide extract --stegofile " + imagePath;
    extractCommand += " -xf k.txt -f -p " + password;
    system(extractCommand.c_str());
    std::ifstream in;
    in.open("k.txt");
    std::string text((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());
    std::string delCommand = "rm k.txt";
    system(delCommand.c_str());
    return text;
}

Seng::~Seng() {

}






