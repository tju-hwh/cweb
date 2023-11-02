#include "infer.h"
Infer* myInfer = new Infer;
    
void process(){
    myInfer->process();
    
}

void dele(){
    delete myInfer;
    myInfer = nullptr;
}