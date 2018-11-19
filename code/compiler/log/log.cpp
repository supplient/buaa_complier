#include "log.h"

#include <iostream>

using namespace std;

namespace log{
    Debug debug("log/debug.log");
    Debug info("log/info.log", &debug);
    Debug warning("log/warning.log", &info);
    Debug error("log/error.log", &warning);
    Debug fatal("log/fatal.log", &error);

    Homework hw("log/hw.log");

    Log::Log(const string &filename, Log *lower)
        :file(filename), lower(lower){
    }
}
