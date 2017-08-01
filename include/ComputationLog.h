#ifndef PICMDK_COMPUTATION_LOG_H
#define PICMDK_COMPUTATION_LOG_H


#include <iostream>
#include <fstream>
#include <string>


namespace picmdk {


// Access to computation log for writing messages, warnings and errors.
// There is a separate local log for each MPI process and global log.
// Is Singleton, access to the only instance via ComputationalLog::getInstance()
class ComputationLog {

// Output functionality for handlers
public:

    void write(const std::string& message, bool copyToGlobal = false);
    void writeWarning(const std::string& warning);
    void writeError(const std::string& error);
    void writeGlobal(const std::string& message);
    std::ostream& getLocalStream();

// These methods are not for handlers
public:

    // Access to the only instance
    static ComputationLog& getInstance();

    // Reset computational log
    static void reset(const std::string& outDirName, bool useStdout, const int& rank);

private:

    // Explicit construction and copying are forbidden
    ComputationLog();
    ComputationLog(const ComputationLog& );
    ComputationLog& operator =(const ComputationLog& );

    // Generalized output stream which can be reset
    class OutStream {
    public:
        OutStream(std::ostream& _stream): stream(&_stream) {}
        void setToFile(std::string fileName);
        std::ostream& operator()() { return *stream; }
    private:
        std::ostream* stream;
        std::ofstream file;
    };

    OutStream globalLog; // global simulation log
    OutStream localLog; // process log
    int rank; // process rank

};


}

#endif
