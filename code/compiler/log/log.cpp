#include "log.h"

#include <iostream>

using namespace std;

namespace mylog{
    Debug debug("log/debug.log");
    Debug info("log/info.log", &debug);
    Debug warning("log/warning.log", &info);
    Error error("log/error.log", &warning);
    Fatal fatal("log/fatal.log", &error);

    Homework hw("log/hw.log");
    FileNormal ass("../../asm/test.asm");
    FileNormal tup("../../tuple/tuple.txt");

    Log::Log(const string &filename, Log *lower)
        :file(filename), lower(lower){
    }
}
