/** 
 * \file parser.cpp
 * \brief The program parses GPS data read from file. Output is also written to a file.
 * \author Marcin Tomaszewski
 * \version Revision: 1.0
 *
 * The application parses data supplied in 'wejscie.txt' file.
 * Formatted data are written to 'wyjscie.txt' file.
 */
#include "stdafx.h"

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * \param line - a std::string line to be splitted
 * \return Returns a std::vector<std::string> containing words extracted from input line.
 *
 * Function splits a string into elements separated by ',' character. Data is stored as a std::vector.
 */
vector<string> split(const string &line)
{
    vector<string> words;
    stringstream stream(line);
    string word;

    while (std::getline(stream, word, ',')) {
        words.push_back(word);
    }
    return words;
}

/**
 * \param is - input file stream
 * \param os - output file stream
 *
 * The function parses text data read from the file stream, searches for GPS data tokens and writes
 * them into output file stream.
 * The output is also shown in the console.
 */
void parse(ifstream &is, ofstream &os)
{
    string line;
    int requiredInfoRead = 0;
    string time, latitude, longitude, horizontal, vertical, date, speed;
    vector<string> azimuths;

    // Analyse each line in input stream
    while (getline(is, line))
    {
        // Detect data entries sepearator
        if (line.find("------------------") != string::npos)
        {
            requiredInfoRead = 0;
            continue;
        }

        auto dataFromLine = split(line);

        // Each if statement analysis different data tokens
        if (line.find("GPGGA") != string::npos)
        {
            time = dataFromLine[1];
            ++requiredInfoRead;
        }

        if (line.find("GPRMC") != string::npos)
        {
            latitude = dataFromLine[3];
            horizontal = dataFromLine[4];
            longitude = dataFromLine[5];
            vertical = dataFromLine[6];
            date = dataFromLine[9];
            requiredInfoRead += 5;
        }

        if (line.find("GPVTG") != string::npos)
        {
            speed = dataFromLine[7];
            requiredInfoRead++;
        }

        if (line.find("GPGSA") != string::npos)
        {

        }

        if (line.find("GPGSV") != string::npos)
        {
            azimuths.push_back(dataFromLine[6]);
            requiredInfoRead++;
        }

        // Generate outpt when read correct amount of data
        if (requiredInfoRead == 10)
        {
            stringstream strs;
            // Format data, prepare it to write to file
            strs << "-------------------------------------------------------------------\n";
            strs << time << " czas\n";
            strs << latitude << " " << horizontal << " szerokosc\n";
            strs << longitude << " " << vertical << " dlugosc\n";
            strs << date.substr(0, 2) << "." << date.substr(2, 2) << "." << date.substr(4, 2) << " data\n"; // date "data"
            strs << speed << " predkosc\n";
            strs << azimuths[0] + ", " << azimuths[1] << ", " << azimuths[2] << " azymuty\n";

            // Write to file
            os << strs.str();

            // Print to console
            cout << strs.rdbuf();

            // Reset data read
            requiredInfoRead = 0;
        }
    }
}

const string inFile = "./wejscie.txt";
const string outFile = "./wyjscie.txt";

/**
 * Main program entry function.
 */
int main()
{
    ifstream ifstr(inFile);
    ofstream ofstr(outFile);
    
    parse(ifstr, ofstr);
    return 0;
}
