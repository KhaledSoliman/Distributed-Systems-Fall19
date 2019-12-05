//
// Created by khloud ghattas on 12/4/19.
//
#include "Seng.h"
using namespace std;
Seng::Seng()
{
    return 0;
}
 void Seng::StringToImage(std::string imagePath, std::string text, std::string password)
{
    ofstream outfile;
    outfile.open("/Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/k.txt");
    outfile << text;
    outfile.close();
    // -cf ---> coverfile filename
    // Specify the cover file that will be used to embed data.
    string  CommForEmbedding = "steghide embed -cf " + imagePath + " -ef /Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/k.txt -f -p " + password;
    system(CommForEmbedding.c_str());
    string CommForDelt = "rm /Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/k.txt";
    system(CommForDelt.c_str());
}
string Seng::ImageToString(std::string imagePath, std::string password)
{
    string CommForExtracting ="steghide extract --stegofile " + imagePath;
    CommForExtracting += " -xf  /Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/k.txt -f -p " + password;
    system(CommForExtracting.c_str());
      // Deleting part
    string CommForExtracting = "rm /Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/k.txt";
    system(CommForExtracting.c_str());
}
Seng::~Seng()
{
    delete;
}






