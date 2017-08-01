#include "ComputationLog.h"

#include "MPIWrapper.h"
#include "Utility.h"

using namespace std;


namespace {


ostream& tryOpenFile(const string& name, ofstream& file)
{
    file.open(name.c_str(), ios::app | ios::out);
    // In case failed to open the file report it and fall back to cout
    if (!file.is_open()) {
        string message = "ERROR: unable to open file " + name +
            " for computation log, switching to stdout instead";
        cerr << message;
        cout << message;
        return cout;
    }
    return file;
}


string prefix(const string& tag)
{
    return tag + ": ";
}


string processPrefix(const int& rank)
{
    return " at process " + picmdk::toString(rank);
}


string postfix()
{
    return "\n";
}


} // anonymous namespace


namespace picmdk {


// By default set output to stdout until reset happens
ComputationLog::ComputationLog():
    globalLog(cout),
    localLog(cout)
{
}


ComputationLog& ComputationLog::getInstance()
{
    static ComputationLog computationLog;
    return computationLog;
}


void ComputationLog::reset(const string& outDirName, bool useStdout, const int& rank)
{
    ComputationLog& computationLog = ComputationLog::getInstance();
    computationLog.rank = rank;
    if (!useStdout) {
        const string prefix = ((outDirName == "") ? "" : outDirName + "/");
        const string dir = prefix + "ComputationLog";
        computationLog.globalLog.setToFile(prefix + "ComputationLog.txt");
        computationLog.localLog.setToFile(dir + "/ComputationLog_" + picmdk::toString(rank) + ".txt");
    }
}


void ComputationLog::write(const std::string& message, bool copyToGlobal)
{
    localLog() << message + postfix();
    localLog().flush();
    if (copyToGlobal && rank == 0) {
        globalLog() << message + postfix();
        globalLog().flush();
    }
}


void ComputationLog::writeWarning(const std::string& warning)
{
    localLog() << prefix("WARNING") + warning + postfix();
    localLog().flush();
    globalLog() << prefix("WARNING" + processPrefix(rank)) + warning + postfix();
    globalLog().flush();
}


void ComputationLog::writeError(const std::string& error)
{
    localLog() << prefix("ERROR") + error + postfix();
    localLog().flush();
    string globalError = prefix("ERROR" + processPrefix(rank)) + error + postfix();
    globalLog() << globalError;
    globalLog().flush();
    cerr << globalError;
}


void ComputationLog::writeGlobal(const std::string& message)
{
    if (rank == 0) {
        globalLog() << message + postfix();
        globalLog().flush();
    }
}


ostream& ComputationLog::getLocalStream()
{
    return localLog();
}


void ComputationLog::OutStream::setToFile(std::string fileName)
{
    stream = &tryOpenFile(fileName, file);
}


} // namespace picmdk