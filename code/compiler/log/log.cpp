#include "log.h"

#include <iostream>

using namespace std;

namespace log{
    Debug debug("log/debug.log");
    Debug info("log/info.log", &debug);
    Debug warning("log/warning.log", &info);
    Debug error("log/error.log", &warning);
    Debug fatal("log/fatal.log", &error);

    Log::Log(const string &filename, Log *lower)
        :file(filename), lower(lower){
    }

    Log& Log::operator<<(const string &s){
        if(lower)
            *lower << s;
        return *this;
    }

    Log& Debug::operator<<(const string &s){
        file << "[DEBUG]" << s << endl;
        //cerr << "[DEBUG]" << s << endl;
        return Log::operator<<(s);
    }
}
