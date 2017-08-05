include_directories(
    ${MPI_INCLUDE_PATH}
    include
	include/Utility)

add_library(PIC-MDK
    include/Adapter/Adapter.h
    include/Adapter/Cell.h
    include/Adapter/Ensemble.h
    include/Adapter/Grid.h
    include/Adapter/Input.h		
    include/Adapter/Particle.h
    include/Adapter/Types.h	
	include/Constants.h
	include/MPIWrapper.h	
	include/OpenMPWrapper.h		
	include/Utility.h
    include/Vector.h

    include/Communicator.h		
	include/ComputationLog.h
    include/Controller.h
	include/Exception.h
	include/Event.h		
    include/Handler.h
    include/InterData.h
    include/Module.h
    include/ModuleInstantiator.h		

    src/Communicator.cpp		
    src/ComputationLog.cpp
    src/Exception.cpp
    src/InterData.cpp	
	src/MPIWrapper.cpp
)
