#ifndef LOGGER_DTS
#define LOGGER_DTS

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>


class Logger {
    private:
        std::string fileName;
        std::fstream fileStream;
        std::string CreateDefaultFileName();
    public:
        Logger();
        void OpenFile();
        void CloseFile();
        bool FileIsOpen();
        void WriteToFile(std::string outStream);
        void ReadFromFile();
};

#endif //DTS
