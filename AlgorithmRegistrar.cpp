#include "AlgorithmRegistrar.h"

int AlgorithmRegistrar::loadAlgorithm(const std::string& path, const std::string& so_file_name_without_so_suffix) {
    size_t size = instance.size();
    // this is mockup code, real code will load the .so files
    if(so_file_name_without_so_suffix == "algorithm1") {
        //register1();
    }
    else return FILE_CANNOT_BE_LOADED; 

    if(instance.size() == size) {
        return NO_ALGORITHM_REGISTERED; // no algorithm registered
    }
    instance.setNameForLastAlgorithm(so_file_name_without_so_suffix);
    return ALGORITHM_REGISTERED_SUCCESSFULY;
}

AlgorithmRegistrar AlgorithmRegistrar::instance;