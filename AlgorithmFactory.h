/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */
#ifndef _ALGORITHMFACTORY
#define _ALGORITHMFACTORY
#include <map>

typedef AbstractAlgorithm *algorithm_constructor();
/*extern */map<string, algorithm_constructor *> AlgorithmFactory;
#endif