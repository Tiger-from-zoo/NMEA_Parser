#include <iostream>

#include "NMEA.h"

std::vector<std::string> split(std::string sentence) {
    std::size_t idx = 0;
    std::string delim = ",";
    std::string sentence_cpy = sentence;
    std::vector<std::string> rtn;
    // Seperate message parts
    while ((idx = sentence_cpy.find(delim)) != std::string::npos) {
        rtn.push_back(sentence_cpy.substr(0, idx));
        sentence_cpy.erase(0, idx + delim.length());
    }
    rtn.push_back(sentence_cpy);
    return rtn;
}

int main() {
    //std::string test_nmea = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76";
    //std::string test_nmea0, test_nmea1, test_nmea2, test_nmea3, test_nmea4, test_nmea5;
    //test_nmea0 = "$GPGSV,4,1,16,02,82,150,53,11,78,139,,12,72,191,53,25,50,296,51*77";
    //test_nmea1 = "$GPGSV,4,2,16,06,43,056,49,20,33,149,45,29,20,275,44,19,16,087,46*73";
    //test_nmea2 = "$GPGSV,4,3,16,31,11,329,42,05,10,169,42,24,07,212,44,04,01,033,*7C";
    //test_nmea3 = "$GPGSV,4,4,16,44,32,184,47,51,31,171,48,48,31,194,47,46,30,199,47*7E";
    //test_nmea4 = "$GNGSA,M,3,66,85,75,67,73,84,83,,,,,,0.8,0.5,0.6*26";
    //test_nmea5 = "$GPGSV,3,3,09,30,02,112,";
//
    //std::vector<std::string> x = split(test_nmea4);
    //std::vector<std::string> x2 = split(test_nmea5);
//
    //std::vector<std::string> GSV = split(test_nmea5);
    //std::size_t svs = (GSV.size()-4)/4;
    //std::cout << x.size() << " " << x2.size() << " " << svs << std::endl;
//
    //int prn = 4;
    //for (int i = 0; i < svs; i++) {
    //    std::cout << prn+(i*4) << std::endl;
    //}
    // NMEA_Parser par;
    // std::cout << par.adgd << std::endl;

    std::string date = "210307";
    int year = 0;
    int month = 0;
    int day = 0;

    day = std::stoi(date.substr(0, 2));
    month = std::stoi(date.substr(2, 2));
    year = 2000 + std::stoi(date.substr(4, 2));
    
    std::cout << day << std::endl;
    std::cout << month << std::endl;
    std::cout << year << std::endl;

    return 0;
}