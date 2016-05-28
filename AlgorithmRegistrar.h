#ifndef __ALGORITHM_REGISTRAR_H_
#define __ALGORITHM_REGISTRAR_H_

#include <list>
#include <functional>
#include <memory>

#include "AbstractAlgorithm.h"

using namespace std;
class AlgorithmRegistrar {
    list<string> algorithmNames;
    list<function<unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    void registerAlgorithm(function<unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
        instance.algorithmFactories.push_back(algorithmFactory);
    }

public:
    friend class AlgorithmRegistration;
    enum {ALGORITHM_REGISTERED_SUCCESSFULY = 0, FILE_CANNOT_BE_LOADED = -1, NO_ALGORITHM_REGISTERED = -2};
    void setNameForLastAlgorithm(const string& algorithmName) {
        //assert(algorithmFactories.size()-1 == algorithmNames.size());
        algorithmNames.push_back(algorithmName);
    }
    list<unique_ptr<AbstractAlgorithm>> getAlgorithms()const {
        list<unique_ptr<AbstractAlgorithm>> algorithms;
        for(auto algorithmFactoryFunc : algorithmFactories) {
            algorithms.push_back(algorithmFactoryFunc());
        }
        return algorithms;
    }
    const list<string>& getAlgorithmNames()const {
        return algorithmNames;
    }
    size_t size()const {
        return algorithmFactories.size();
    }
    static AlgorithmRegistrar& getInstance() {
        return instance;
    }
private:
    static AlgorithmRegistrar instance;
};

#endif