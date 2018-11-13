#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>

using namespace std;

namespace log{

    class Log{
        public:
            Log(const string &log_filename, Log *lower=NULL);
            virtual Log& operator<<(const string &);
        protected:
            ofstream file;
            Log *lower;
    };

    class Debug:public Log{
        public:
            Debug(const string &filename, Log *lower=NULL):Log(filename, lower){}
            Log& operator<<(const string &);
    };

    extern Debug fatal;
    extern Debug error;
    extern Debug warning;
    extern Debug info;
    extern Debug debug;
}

#endif//LOG_H