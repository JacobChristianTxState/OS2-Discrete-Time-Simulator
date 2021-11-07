#ifndef LOGGER_DTS
#define LOGGER_DTS

#include <string>
#include <fstream>
#include <iostream>


class SimulatorLogger {
    private:
        std::string fileName;
        std::fstream fileStream;
        std::string CreateDefaultFileName();
        float totalWorkTime;
    public:
        SimulatorLogger();
        void OpenFile();
        void CloseFile();
        bool FileIsOpen();
        void WriteToFile(std::string outStream);
        void ReadFromFile();
};

#endif //DTS
