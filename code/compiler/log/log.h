#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

namespace log{

    class Log{
        public:
            Log(const string &log_filename, Log *lower=NULL);
            virtual ~Log(){}

            template<class T>
            Log& operator<<(const T &s){
                if(lower)
                    *lower << s;
                return *this;
            }
        protected:
            ofstream file;
            Log *lower;
    };

    class Fatal:public Log{
        public:
            Fatal(const string &filename, Log *lower=NULL):Log(filename, lower){}
            virtual ~Fatal(){}

            template<class T>
            Fatal& operator<<(const T &s){
                file << "[FATAL]" << s << endl;
                cerr << "[FATAL]" << s << endl;
                return dynamic_cast<Fatal&>(Log::operator<<(s));
            }
    };

    class Error:public Log{
        public:
            Error(const string &filename, Log *lower=NULL):Log(filename, lower){}
            virtual ~Error(){}

            template<class T>
            Error& operator<<(const T &s){
                file << "[Error]" << s << endl;
                cerr << "[Error]" << s << endl;
                return dynamic_cast<Error&>(Log::operator<<(s));
            }
    };

    class Debug:public Log{
        public:
            Debug(const string &filename, Log *lower=NULL):Log(filename, lower){}
            virtual ~Debug(){}

            template<class T>
            Debug& operator<<(const T &s){
                file << "[DEBUG]" << s << endl;
                //cerr << "[DEBUG]" << s << endl;
                return dynamic_cast<Debug&>(Log::operator<<(s));
            }
    };

    class Homework:public Log{
        public:
            Homework(const string &filename, Log *lower=NULL):Log(filename, lower){}
            virtual ~Homework(){}

            template<class T>
            Homework& operator<<(const T &s){
                clog << "This is " << s << "!" << endl;
                return dynamic_cast<Homework&>(Log::operator<<(s));
            }
    };

    extern Fatal fatal;
    extern Error error;
    extern Debug warning;
    extern Debug info;
    extern Debug debug;

    extern Homework hw;
}

#endif//LOG_H
