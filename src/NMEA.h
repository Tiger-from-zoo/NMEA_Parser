#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

// Check if GNSS system ID is enabled with NMEAVERSION 4.11
// Table of GNSS System and Signal ID'S: (https://docs.novatel.com/OEM7/Content/Logs/GPGRS.htm#System)
// NMEAVERSION command: (https://docs.novatel.com/OEM7/Content/Commands/NMEAVERSION.htm)

// https://cplusplus.com/reference/string/string/append/
// https://en.wikipedia.org/wiki/List_of_GPS_satellites

class NMEA_Parser {
    NMEA_Parser () {
        init_enums();
    }

    public:
    // Latitude
    // DD, NOT DMS OR DMM
    double latitude = 0;

    // Longitude
    // DD, NOT DMS OR DMM
    double longitude = 0;

    /// All precision dilutions

    double hdop = 0;
    double vdop = 0;
    double pdop = 0;

    // Altitude
    // METERS
    double msl = 0;

    // Geoid Seperation / Undulation
    // METERS
    double gsep = 0;

    // Time since latest correction data update
    // SECONDS
    double adgd = 0;

    //Track made good / True heading
    // DEGREES
    double deg_t = 0;

    // Magnetic Variation
    // DEGREES
    double mag_var = 0;

    // Magnetic Variation Direction
    // STING (E OR W)
    std::string mag_var_dir;

    //Position Status
    // A = Valid
    // V = invalid
    std::string pos_stat;

    // Spped
    // KNOTS
    double speed;

    /// Time

    // Hours
    int hh = 0;

    // Minutes
    int mm = 0;

    // Seconds
    int ss = 0;

    // Milliseconds
    int ms = 0;

    /// Misc.

    // Fix
    // 0 = Fix not availible or valid
    // 1 = Single point
    // 2 = Pseudorange differential
    // 4 = RTK fixed ambiguity solution
    // 5 = RTK floating ambiguity solution
    // 6 = Dead reckoning mode (ESTIMATED POSITION)
    // 7 = Manually inputed position
    // 8 = Simulation
    // 9 = SBAS
    int fix = 0;

    // Satellites
    // COUNT
    int sat = 0;

    // Fix mode
    // 1 = Fix not availible 
    // 2 = 2D
    // 3 = 3D
    int mode = 0;

    // Satellite information vector
    // Each element is an array of 4 doubles and, in order, they mean: {PRN, ELEVATION, AZIMUTH, SNR}
    std::vector<std::array<double, 4>> sat_status;

    // Takes in a pointer to a buffer string to parse NMEA sentences
    void parse(std::string* sentence_group) {
        std::size_t idx = 0;
        std::string delim = " ";
        // std::string sentence_group_cpy = sentence_group;
        
        // Remove any spaces
        while ((idx = (*sentence_group).find(delim)) != std::string::npos) {
            (*sentence_group).erase((*sentence_group).begin()+idx);
        }
        
        // Get individual sentences
        std::size_t start_idx = 0;
        std::size_t end_idx;
        std::string start_delim = "$";
        std::string end_delim = "*";
        std::string NMEA_sentence;

        /// Loops through every valid NMEA sentence, parses it, hands off sentence in a vector to appropriate sub-parser function, and stores relevant information into variables
        while ((start_idx = (*sentence_group).find(start_delim)) != std::string::npos && (end_idx = (*sentence_group).find(end_delim)) != std::string::npos) {
            NMEA_sentence = (*sentence_group).substr(start_idx, (end_idx-start_idx+3));
            switch (NMEA_sentence_map[NMEA_sentence.substr(0, 6)]) {
                case ev_GGA:
                    parse_GGA(split(NMEA_sentence));
                    break;
                case ev_GSV:
                    parse_GSV(split(NMEA_sentence));
                    break;
                case ev_RMC:
                    parse_GSV(split(NMEA_sentence));
                    break;
                case ev_GSA:
                    parse_GSV(split(NMEA_sentence));
                    break;
                default:
                    //Remove message?
                    std::cout << "\"" << NMEA_sentence.substr(0, 6) << "\"" << "is not a verified GPS NMEA sentence or unsupported NMEA sentence type" << std::endl;
                    break;
            }
            (*sentence_group).erase(start_idx, (end_idx-start_idx+3));
        }

        return;
    }

    private:
    enum StringValue {ev_GGA, ev_GSV, ev_RMC, ev_GSA};
    static std::map<std::string, StringValue> NMEA_sentence_map;

    void init_enums() {
        NMEA_sentence_map["$GPGGA"] = ev_GGA;
        NMEA_sentence_map["$GPGSV"] = ev_GSV;
        NMEA_sentence_map["$GPRMC"] = ev_RMC;
        NMEA_sentence_map["$GPGSA"] = ev_GSA;
    }

    // Sub-parser functions

    void parse_GGA(std::vector<std::string> GGA) {
        // https://docs.novatel.com/OEM7/Content/Logs/GPGGA.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____59

        // Time
        hh = std::stoi(GGA[1].substr(0, 2));
        mm = std::stoi(GGA[1].substr(2, 2));
        ss = std::stoi(GGA[1].substr(4, 2));
        ms = std::stoi(GGA[1].substr(7, 3));

        // Latitude & Longitude
        double inter_lat, inter_lon;
        inter_lat = std::trunc((std::stod(GGA[2])/100)) + (std::stod(GGA[2].substr(2, 7))/60);
        if (GGA[3] == "S") {
            inter_lat *= -1;
        }
        inter_lon = std::trunc((std::stod(GGA[4])/100)) + (std::stod(GGA[4].substr(3, 7))/60);
        if (GGA[5] == "W") {
            inter_lon *= -1;
        }
        latitude = inter_lat;
        longitude = inter_lon;


        // Fix, satellites, and HDOP
        fix = std::stoi(GGA[6]);
        sat = std::stoi(GGA[7]);
        hdop = std::stod(GGA[8]);

        // Altitude, Geoid Seperation, and Age of Differential GPS Data
        if (GGA[9] != "") { msl = std::stod(GGA[9]); } else { msl = 0; }
        if (GGA[11] != "") { gsep = std::stod(GGA[11]); } else { gsep = 0; }
        if (GGA[13] != "") { adgd = std::stod(GGA[13]); } else { adgd = 0; }

        return;
    }

    void parse_GSV(std::vector<std::string> GSV) {
        // ^1 = https://docs.novatel.com/OEM7/Content/Logs/GPGSV.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____65

        // If a new set of GSV sentences arrives, the sentence index(Message number^1) will be "1," so it clears the old satellite information
        if (GSV[2] == "1") {
            sat_status.clear();
        }
        //             {            }{           }{           }{           }
        //   0   1 2 3   4  5  6   7  8  9  10 11 12 13  14 15 16 17 18  19
        //$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
        std::array<double, 4> sv_buffer {};
        
        // Indicies of satellite information in sv_buffer
        int prn = 4;
        int elev = 5;
        int az = 6;
        int snr = 7;

        // Amount of SV's
        // ({sentence parts}-{First four irrelevant sentence parts (sentence tpye, amount of GSV sentence, current GSV sentence, satellite count)})/{4; amount of SV information in each part}
        std::size_t svs = (GSV.size()-4)/4;

        for (int i = 0; i < svs; i++) {
            if (GSV[prn+(i*4)] != "") {
                if (GSV[snr+(i*4)] != "") {
                    sv_buffer = {std::stod(GSV[prn+(i*4)]), std::stod(GSV[elev+(i*4)]), std::stod(GSV[az+(i*4)]), std::stod(GSV[snr+(i*4)])};
                } else if (GSV[snr+(i*4)] == "") {
                    sv_buffer = {std::stod(GSV[prn+(i*4)]), std::stod(GSV[elev+(i*4)]), std::stod(GSV[az+(i*4)]), 0};
                } else {
                    sv_buffer = {std::stod(GSV[prn+(i*4)]), 0, 0, 0};
                }
                sat_status.push_back(sv_buffer);
            }
        }

        return;
    }

    void parse_RMC(std::vector<std::string> RMC) {
        // https://docs.novatel.com/OEM7/Content/Logs/GPRMC.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____69
        if(RMC[7] != "") { speed = std::stod(RMC[7]); } else { speed = 0; }
        if(RMC[8] != "") { deg_t = std::stod(RMC[8]); } else { deg_t = 0; }
        if(RMC[10] != "") { mag_var = std::stod(RMC[10]); } else { mag_var = 0;}
        if(RMC[11] != "") { mag_var_dir = RMC[11]; } else { mag_var_dir = "E"; }

        return; 
    }

    void parse_GSA(std::vector<std::string> GSA) {
        // https://aprs.gids.nl/nmea/#gsa
        // https://docs.novatel.com/OEM7/Content/Logs/GPGSA.htm?tocpath=Commands%20%2526%20Logs%7CLogs%7CGNSS%20Logs%7C_____63
        if (GSA[2] == "") {
            mode = std::stoi(GSA[2]);
            if (GSA[15] != "") { pdop = std::stod(GSA[15]); } else { pdop = 0; }
            if (GSA[16] != "") { hdop = std::stod(GSA[16]); } else { hdop = 0; }
            if (GSA[17] != "") { vdop = std::stod(GSA[17]); } else { vdop = 0; }
            } else { mode = 0; pdop = 0; hdop = 0; vdop = 0; }

        return;
    }

    // Individual sentence part splitter
    std::vector<std::string> split(std::string sentence) {
        std::size_t idx = 0;
        std::string delim = ",";
        std::string sentence_cpy = sentence;
        std::vector<std::string> rtn;

        // Seperate sentence parts
        while ((idx = sentence_cpy.find(delim)) != std::string::npos) {
            rtn.push_back(sentence_cpy.substr(0, idx));
            sentence_cpy.erase(0, idx + delim.length());
        }
        rtn.push_back(sentence_cpy);

        return rtn;
    }
};