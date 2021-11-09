#include "SimulatorLogger.h"

SimulatorLogger::SimulatorLogger() {   
    this->fileName = CreateDefaultFileName(); 
}

std::string SimulatorLogger::CreateDefaultFileName(){
    return "sim.data";
}

void SimulatorLogger::OpenFile() {
    this->fileStream.open(this->fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
}

void SimulatorLogger::CloseFile() {
    this->fileStream.close();
}

bool SimulatorLogger::FileIsOpen() {
    return fileStream.is_open();
}

void SimulatorLogger::WriteToFile(std::string outStream) {
    if (SimulatorLogger::FileIsOpen()) {
        //if new line character is not found at end of string, add it
        if (outStream.find("\n") == std::string::npos) {
            outStream += "\n";
        }
        this->fileStream << outStream;
    }
}

void SimulatorLogger::ReadFromFile() {
    if (SimulatorLogger::FileIsOpen()) {
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
