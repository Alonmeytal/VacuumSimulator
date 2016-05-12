#include "AlgorithmRegistrar.h"

int AlgorithmRegistrar::loadAlgorithm(const string& path, const string& so_file_name_without_so_suffix) {
    size_t size = instance.size(); // saving registrar's size before trying to load another algorithm.


    // this is mockup code, real code will load the .so files
    if(so_file_name_without_so_suffix == "algorithm1") {
        //register1();
    }
    else return FILE_CANNOT_BE_LOADED; 

    if(instance.size() == size) {
        return NO_ALGORITHM_REGISTERED; // no algorithm registered
    }
    //instance.setNameForLastAlgorithm(fileName.substr(fileName.find_last_of('/')+1,fileName.find_last_of('.')-2););
    return ALGORITHM_REGISTERED_SUCCESSFULY;
}

AlgorithmRegistrar AlgorithmRegistrar::instance;