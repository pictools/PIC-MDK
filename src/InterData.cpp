#include "InterData.h"

#include "Handler.h"

namespace {


using namespace picmdk;




} // anonymous namespace



namespace picmdk {


void InterData::registerExport(DataSetBase* dataSet, const std::string& name, SynchronizationMode mode,
    SynchronizerBase* synchronizer, FinalizerBase* finalizer)
{
    ExportDescription exportDescription;
    exportDescription.dataSet = dataSet;
    exportDescription.name = name;
    exportDescription.isGlobal = (mode.locality == SynchronizationMode::Global);
    exportDescription.synchronizer = synchronizer;
    exportDescription.finalizer = finalizer;
    exportDescription.destination = 0;
    exportDescription.threadIdx = 0;
    bool isMultithreaded = (currentHandler->getType() == Handler::Particle || currentHandler->getType() == Handler::Cell);
    if (!exportData.empty() && exportData.back().name == exportDescription.name)
        exportDescription.threadIdx = exportData.back().threadIdx + 1;
}

void InterData::registerImport(DataSetBase* dataSet, const std::string& name)
{
    ImportDescrtiption importDescription;
    importDescription.dataSet = dataSet;
    importDescription.name = name;
    importData.push_back(importDescription);
}


void InterData::setCurrentHandler(Handler* handler)
{
    currentHandler = handler;
}

void InterData::finalizeInit()
{

}

void InterData::synchronizeAll()
{

}



} // namespace picmdk
