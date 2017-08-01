#ifndef MDK_INTERDATA_H
#define MDK_INTERDATA_H


#include "Exception.h"
#include "Utility.h"
#include "Vector.h"

#include <cstring>
#include <string>


namespace picmdk {

class Handler;

class InterData {
public:

    struct SynchronizationMode {
        // Operation to perform during synchonization:
        // all except Gather are reduction-type operations,
        // Gather is analogue of MPI_Gather
        enum Operation {None, Sum, Product, Max, Min, Average, Gather};

        // Perform synchronization globally - for all domains and threads
        // or locally - for each domain synchronize between its threads
        enum Locality {Global, Local};

        // Finalization of the original data after synchronization is done
        // None - do nothing, Clear - set to 0
        enum Finalization { Keep, Clear };

        Operation operation;
        Locality locality;
        Finalization finalization;

        SynchronizationMode(Operation _operation, Locality _locality, Finalization _finalization = Keep):
            operation(_operation),
            locality(_locality),
            finalization(_finalization)
        {
        }
    };

    // Register the given data set to be exported to
    template<class DataSet>
    void registerExport(DataSet* dataSet, const std::string& name, SynchronizationMode mode)
    {
        registerExport(dataSet, name, mode, createSynchronizer(dataSet, mode), createFinalizer(dataSet, mode));
    }

    // Register the given data set to be imported from the existing dataset with the given name
    template<class DataSet>
    void registerImport(DataSet* dataSet, const std::string& name)
    {
        registerImport(dataSet, name);
    }

    //// TODO: change element(...) methods to at(...) STL-style ?

    // Abstract base class for all InterData data sets
    class DataSetBase {
    public:
        virtual ~DataSetBase()
        {
        }

        // Access to raw data and size
        virtual void* getRaw() = 0;
        virtual int getRawSizeBytes() const = 0;
    };

    // Helper CRTP helper class for Value<T>, Array<T>, Array2d<T> and Array3d<T> to inherit.
    // Template parameter is the dataset itself, not T.
    template <class DataSet>
    class DataSetImplementation: public DataSetBase {
    public:

        typedef typename DataSet::ValueType ValueType;
        typedef typename DataSet::IndexType IndexType;

        DataSetImplementation():
            raw(0),
            rawSize(0),
            ownsMemory(true)
        {
        }

        DataSetImplementation(IndexType _size, int _rawSize, ValueType value = 0):
            size(size),
            rawSize(_rawSize),
            ownsMemory(true)
        {
            raw = new ValueType[rawSize];
            std::fill(raw, raw + rawSize, value);
        }

        virtual ~DataSetImplementation()
        {
            if (ownsMemory)
                delete[] raw;
        }

        int getNumElements() const
        {
            return rawSize;
        }

        // Implementation of DataSetBase interface
        virtual void* getRaw()
        {
            return (void*)raw;
        }

        // Implementation of DataSetBase interface
        virtual int getRawSizeBytes() const
        {
            return rawSize * sizeof(ValueType);
        }

    protected:

        IndexType size; // number of elements for each dimension (depends on DataSet)
        ValueType* raw; // pointer to raw data
        int rawSize; // number of elements in raw data
        bool ownsMemory; // whether the data set owns the raw pointer and has to free it

    private:

        // Set raw pointer to a new position and change it to non-owning
        void changeRaw(ValueType* newRaw)
        {
            std::memcpy(newRaw, raw, rawSize * sizeof(ValueType));
            raw = newRaw;
            ownsMemory = false;
        }
        
        // Copy and assignment are forbidden
        DataSetImplementation(const DataSetImplementation&);
        DataSetImplementation& operator=(const DataSetImplementation&);

        friend class InterData;
    };

    // Preferrably should be thrown in case std::bad_alloc is caught
    class OutOfRangeException : public NamedException {
    public:
        OutOfRangeException(const std::string& message):
            NamedException(message, "out of range exception")
        {
        }

        virtual ~OutOfRangeException() throw()
        {
        }
    };

    // All following classes are parametrized with type T,
    // which is supposed to be an arithmetic type (e.g. char, int, double, float)
    // or a POD structure (e.g. Vector3 of an arithmetic type)
    // Access to arrays is done by 0-based indexes

    // Interface of InterData::Value<T> class for module developers.
    // The class represents a single value of type T
    template<typename T>
    class Value : public DataSetImplementation<Value<T> > {
    public:
        typedef T ValueType;
        typedef int IndexType; // for compatibility with DataSetImplementation

        Value(ValueType value = 0):
            DataSetImplementation<Value<T> >(1, 1, value)
        {
        }

        // Access the value
        ValueType& operator()()
        {
            return raw[0];
        }

        const ValueType& operator()() const
        {
            return raw[0];
        }

        // Access the value, these are for consistency with vector and matrix
        ValueType& element()
        {
            return raw[0];
        }

        const ValueType& element() const
        {
            return raw[0];
        }
    };

    // Interface of InterData::Array<T> class for module developers.
    // The class represents a 1d array (vector) of type T
    template<typename T>
    class Array : public DataSetImplementation<Array<T> > {
    public:
        typedef T ValueType;
        typedef int IndexType;

        // Create an empty vector
        Array():
            DataSetImplementation<Array<T> >()
        {
        }

        // Create an array of the given size with the given value of elements
        Array(IndexType size, ValueType value = 0):
            DataSetImplementation<Array<T> >(size, size, value)
        {
        }

        // Get size of the vector
        IndexType getSize() const
        {
            return size;
        }

        // Access the value by index without checking the index
        ValueType& operator()(IndexType index)
        {
            return raw[index];
        }

        const ValueType& operator()(IndexType index) const
        {
            return raw[index];
        }

        // Access the value by index with checking the index
        ValueType& element(IndexType index)
        {
            if ((index >= 0) && (index < size))
                return operator()(index);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(index) + " is out of range for Array of size " + toString(size)));
        }

        const ValueType& element(IndexType index) const
        {
            if ((index >= 0) && (index < size))
                return operator()(index);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(index) + " is out of range for Array of size " + toString(size)));
        }
    };

    // Interface of InterData::Array2d<T> class for module developers.
    // The class represents a 2d array (matrix) of type T
    template<typename T>
    class Array2d : public DataSetImplementation<Array2d<T> > {
    public:
        typedef T ValueType;
        typedef Vector2<int> IndexType;

        // Create an empty matrix
        Array2d():
            DataSetImplementation<Array2d<T> >()
        {
        }

        // Create a matrix of the given size with the given value of elements
        Array2d(int nRows, int nCols, ValueType value = 0):
            DataSetImplementation<Array2d<T> >(Vector2<int>(nRows, nCols), nRows * nCols, value)
        {
        }

        Array2d(IndexType size, ValueType value = 0):
            DataSetImplementation<Array2d<T> >(size, size.volume(), value)
        {
        }

        // Get number of rows, columns and total number of elements
        IndexType getSize() const
        {
            return size;
        }

        int getNumRows() const
        {
            return size.x;
        }

        int getNumCols() const
        {
            return size.z;
        }

        int getNumElements() const
        {
            return rawSize;
        }

        // Access the value by index without checking the index
        ValueType& operator()(int i, int j)
        {
            return raw[i * size.y + j];
        }

        const ValueType& operator()(int i, int j) const
        {
            return raw[i * size.y + j];
        }

        ValueType& operator()(IndexType index)
        {
            return raw[index.x * size.y + index.y];
        }

        const ValueType& operator()(IndexType index) const
        {
            return raw[index.x * size.y + index.y];
        }

        // Access the value by index with checking the index
        ValueType& element(int i, int j)
        {
            if ((i >= 0) && (i < size.x) && (j >= 0) && (j < size.y))
                return operator()(i, j);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(IndexType(i, j)) + " is out of range for Array2d of size " + toString(size)));
        }

        const ValueType& element(int i, int j) const
        {
            if ((i >= 0) && (i < size.x) && (j >= 0) && (j < size.y))
                return operator()(i, j);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(IndexType(i, j)) + " is out of range for Array2d of size " + toString(size)));
        }

        ValueType& element(IndexType index)
        {
            if ((index.x >= 0) && (index.y < size.x) && (index.y >= 0) && (index.y < size.y))
                return operator()(index);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(index) + " is out of range for Array2d of size " + toString(size)));
        }

        const ValueType& element(IndexType index) const
        {
            if ((index.x >= 0) && (index.y < size.x) && (index.y >= 0) && (index.y < size.y))
                return operator()(index);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(index) + " is out of range for Array2d of size " + toString(size)));
        }
    };

    // Interface of InterData::Array3d<T> class for module developers.
    // The class represents a 3d array of type T
    template<typename T>
    class Array3d : public DataSetImplementation<Array3d<T> > {
    public:
        typedef T ValueType;
        typedef Vector3<int> IndexType;

        // Create an empty 3d array
        Array3d():
            DataSetImplementation<Array3d<T> >()
        {
        }

        // Create a matrix of the given size with the given value of elements
        Array3d(int n1, int n2, int n3, T value = 0):
            DataSetImplementation<Array3d<T> >(IndexType(n1, n2, n3), n1 * n2 * n3, value)
        {
        }

        Array3d(IndexType size, T value = 0):
            DataSetImplementation<Array3d<T> >(size, size.volume(), value)
        {
        }

        // Get size and total number of elements
        IndexType getSize() const
        {
            return size;
        }

        int getNumElements() const
        {
            return rawSize;
        }

        // Access the value by index without checking the index
        ValueType& operator()(int i, int j, int k)
        {
            return raw[(i * size.y + j) * size.z + k];
        }

        const ValueType& operator()(int i, int j, int k) const
        {
            return raw[(i * size.y + j) * size.z + k];
        }

        ValueType& operator()(IndexType index)
        {
            return raw[(index.x * size.y + index.y) * size.z + index.z];
        }

        const ValueType& operator()(IndexType index) const
        {
            return raw[(index.x * size.y + index.y) * size.z + index.z];
        }

        // Access the value by index with checking the index
        ValueType& element(int i, int j, int k)
        {
            if ((i >= 0) && (i < size.x) && (j >= 0) && (j < size.y) && (k >= 0) && (k < size.z))
                return operator()(i, j, k);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(IndexType(i, j, k)) + " is out of range for Array3d of size " + toString(size)));
        }

        const ValueType& element(int i, int j, int k) const
        {
            if ((i >= 0) && (i < size.x) && (j >= 0) && (j < size.y) && (k >= 0) && (k < size.z))
                return operator()(i, j, k);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(IndexType(i, j, k)) + " is out of range for Array3d of size " + toString(size)));
        }

        ValueType& element(IndexType index)
        {
            if ((index.x >= 0) && (index.x < size.x) && (index.y >= 0) && (index.y < size.y) && (index.z >= 0) && (index.z < size.z))
                return operator()(index);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(index) + " is out of range for Array3d of size " + toString(size)));
        }

        const ValueType& element(IndexType index) const
        {
            if ((index.x >= 0) && (index.x < size.x) && (index.y >= 0) && (index.y < size.y) && (index.z >= 0) && (index.z < size.z))
                return operator()(index);
            else
                PICMDK_THROW(OutOfRangeException, ("index " + toString(index) + " is out of range for Array3d of size " + toString(size)));
        }

    };


private:

    class SynchronizerBase {
    public:
        virtual ~SynchronizerBase() {}
        virtual void run(void* destination, const void* source) = 0;
    };

    template<typename T>
    class NoneSynchronizer : public InterData::SynchronizerBase {
    public:
        virtual void run(void* destination, const void* source)
        {
        }
    };

    template<typename T>
    class AddSynchronizer : public InterData::SynchronizerBase {
    public:
        AddSynchronizer(int _numElements) :
            numElements(_numElements)
        {
        }
        virtual void run(void* destination, const void* source)
        {
            T* dst = (T*)destination;
            const T* src = (const T*)source;
            for (int i = 0; i < numElements; i++)
                dst[i] += src[i];
        }
    private:
        int numElements;
    };

    template<class DataSet>
    SynchronizerBase* createSynchronizer(DataSet* dataSet, SynchronizationMode mode)
    {
        switch (mode.operation) {
            case SynchronizationMode::None: return new NoneSynchronizer<DataSet::ValueType>;
            case SynchronizationMode::Add: return new AddSynchronizer<DataSet::ValueType>(dataSet->getNumElements());
            default: PICMDK_THROW(NotImplementedException, ("Synchronization mode is not currently implemented"));
        }
    }

    class FinalizerBase {
    public:
        virtual ~FinalizerBase() {}
        virtual void run(void* data) = 0;
    };

    template<typename T>
    class KeepFinalizer : public InterData::FinalizerBase {
    public:
        virtual void run(void* data)
        {
        }
    };

    template<typename T>
    class ClearFinalizer : public InterData::FinalizerBase {
    public:
        ClearFinalizer(int _numElements) :
            numElements(_numElements)
        {
        }
        virtual void run(void* data)
        {
            T* d = (T*)data;
            for (int i = 0; i < numElements; i++)
                d[i] += (T)0;
        }
    private:
        int numElements;
    };

    template<class DataSet>
    FinalizerBase* createFinalizer(DataSet* dataSet, SynchronizationMode mode)
    {
        switch (mode.finalization) {
            case SynchronizationMode::Keep: return new KeepFinalizer<DataSet::ValueType>;
            case SynchronizationMode::Clear: return new ClearFinalizer<DataSet::ValueType>(dataSet->getNumElements());
            default: PICMDK_THROW(NotImplementedException, ("Finalization mode is not currently implemented"));
        }
    }


    struct ExportDescription {
        DataSetBase* dataSet;
        std::string name;
        int threadIdx;
        bool isGlobal;
        SynchronizerBase* synchronizer;
        FinalizerBase* finalizer;
        void* destination;
    };

    struct ImportDescrtiption {
        DataSetBase* dataSet;
        std::string name;
    };

    void registerExport(DataSetBase* dataSet, const std::string& name, SynchronizationMode mode,
        SynchronizerBase* synchronizer, FinalizerBase* finalizer);
    void registerImport(DataSetBase* dataSet, const std::string& name);

    std::vector<ExportDescription> exportData;
    std::vector<ImportDescrtiption> importData;
    Handler* currentHandler;

    // These methods are for Controller to call
    void setCurrentHandler(Handler* handler);
    void finalizeInit();
    void synchronizeAll();

};


} // namespace MDK


#endif

