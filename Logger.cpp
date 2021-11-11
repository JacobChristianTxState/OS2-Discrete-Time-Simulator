#include "Logger.h"

Logger::Logger() {   
    this->fileName = CreateDefaultFileName(); 
}

std::string Logger::CreateDefaultFileName(){
    return "sim.data";
}

void Logger::OpenFile() {
    this->fileStream.open(this->fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
}

void Logger::CloseFile() {
    this->fileStream.close();
}

bool Logger::FileIsOpen() {
    return fileStream.is_open();
}

void Logger::WriteToFile(std::string outStream) {
    if (Logger::FileIsOpen()) {
        //if new line character is not found at end of string, add it
        if (outStream.find("\n") == std::string::npos) {
            outStream += "\n";
        }
        this->fileStream << outStream;
    }
}

void Logger::ReadFromFile() {
    if (Logger::FileIsOpen()) {
        //return to the beginning of the file
        fileStream.clear();
        fileStream.seekg(0);
        std::string currentLine;
        while (std::getline(this->fileStream, currentLine)) {
            std::cout << currentLine << "\n";
        }
        //return to the beginning of the file
        fileStream.clear();
        fileStream.seekg(0);
    }
}