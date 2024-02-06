#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// #include <iomanip>

#include "NMEA.h"
//
std::vector<std::string> split_to_vec(std::string sentence) {
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

std::string split_to_str(std::string snt) {
    std::size_t start_idx = 0;
    std::size_t end_idx;
    std::string start_delim = "$";
    std::string end_delim = "*";
    end_idx = snt.find(end_delim);
    
    return snt.substr(start_idx+1, (end_idx-start_idx+3)); ;
}

bool verify_checksum(std::string snt) {
    // char x;
    std::string pure_nmea = snt.substr(1, snt.length()-4);
    int inhx, i, checksum, given_checksum;
    checksum = 0;

    std::stringstream nmea_bytes;

    for(i=0; i<pure_nmea.length(); i++) {
        // x = pure_nmea[i];
        inhx = int(pure_nmea[i]);

        nmea_bytes << std::hex << inhx;

        // std::cout << i << " - " << nmea_bytes.str() << " - " << std::stoi(nmea_bytes.str(), nullptr, 16) << std::endl;
        
        checksum = checksum ^ std::stoi(nmea_bytes.str(), nullptr, 16);

        nmea_bytes.str("");
    }

    given_checksum = std::stoi(snt.substr(snt.length()-2, 2), nullptr, 16);
    if(checksum == given_checksum) {
        return true;
    } else {
        return false;
    }
}

int main() {
    std::string test_nmea = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76";
    std::string test_nmea0, test_nmea1, test_nmea2, test_nmea3, test_nmea4, test_nmea5;
    test_nmea0 = "GPGSV,4,1,16,02,82,150,53,11,78,139,,12,72,191,53,25,50,296,51";
    test_nmea0 = "$GPGSV,4,1,16,02,82,150,53,11,78,139,,12,72,191,53,25,50,296,51*77"; //11
    test_nmea1 = "$GPGSV,4,2,16,06,43,056,49,20,33,149,45,29,20,275,44,19,16,087,46*73";
    test_nmea2 = "$GPGSV,4,3,16,31,11,329,42,05,10,169,42,24,07,212,44,04,01,033,*7C";
    test_nmea3 = "$GPGSV,4,4,16,44,32,184,47,51,31,171,48,48,31,194,47,46,30,199,47*7E";
    test_nmea4 = "$GNGSA,M,3,66,85,75,67,73,84,83,,,,,,0.8,0.5,0.6*26";
    test_nmea5 = "$GPGSV,3,3,09,30,02,112,";

    // std::vector<std::string> x = split(test_nmea4);
    // std::vector<std::string> x2 = split(test_nmea5);
// 
    // std::vector<std::string> GSV = split(test_nmea5);
    // std::size_t svs = (GSV.size()-4)/4;
    // std::cout << x.size() << " " << x2.size() << " " << svs << std::endl;
// 
    // int prn = 4;
    // for (int i = 0; i < svs; i++) {
    //     std::cout << prn+(i*4) << std::endl;
    // }

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

    // std::byte BYTES_NMEA[test_nmea0.length()];
    // std::memcpy(BYTES_NMEA, test_nmea0.data(), test_nmea0.length());

    // std::stringstream nmea_bytes;
    // nmea_bytes << std::hex;
    // std::byte BYTES_NMEA[test_nmea0.length()];
    // std::memcpy(BYTES_NMEA, test_nmea0.data(), test_nmea0.length());
    // for(i=0; i>test_nmea0.length(); i++) {
    //     //checksum ^ std::stoi(BYTES_NMEA[i], nullptr, 16);
    //     //checksum ^ static_cast<int> (BYTES_NMEA[i]);
    //     nmea_bytes << std::setfill('0') << std::setw(2) << static_cast<int> (test_nmea0[i]);
    //     std::cout << nmea_bytes.str() << std::endl;
    //     checksum ^ std::stoi(nmea_bytes.str(), nullptr, 16);
    //     
    //     nmea_bytes.clear();
    // }
    
    // NMEA_Parser par;
    // par.parse(&test_nmea);
    // std::cout << par.hh << ":" << par.mm << std::endl;
    // std::cout << par.latitude << " - " << par.longitude << std::endl;
    // std::cout << par.fix << std::endl;
    // std::cout << par.sat << std::endl;
    // std::cout << par.hdop << std::endl;
    // std::cout << par.msl << std::endl;
    // std::cout << par.gsep << std::endl;
    // std::cout << par.adgd << std::endl;
    // char ar[] = "";
    // if (ar == split_to_vec(test_nmea0)[11]) {
    //     std::cout << "x" << std::endl;
    // }
// 
    // std::cout << "--" << split_to_vec(test_nmea0)[11] << "--" << std::endl;

    std::array<float, 5> x;
    memset(&x, 0, sizeof(x));
    x[0] = 4;
    for(int j=0; j<5; j++) {
        std::cout << x[j] << std::endl;
    }
    std::cout << sizeof(x) << std::endl;
    std::cout << x.size() << std::endl;

    return 0;
}