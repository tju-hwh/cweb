#include "controller.h"
#include <iostream>
#include <map>
// #include "acl/acl.h"

// #define INFO_LOG(fmt, args...) fprintf(stdout, "[INFO] " fmt "\n", ##args)
// #define WARN_LOG(fmt, args...) fprintf(stdout, "[WARN] " fmt "\n", ##args)
// #define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR] " fmt "\n", ##args)

// #include "image.h"
#include "infer.h"
// #include "util.h"



// std::string ControHello(){
//     // cout << "Handler1111 \n";
//     // ACL_HELLO();
//     // Aaa();
//     // ImageProcess(1);
//     return "hello 123123";
// }
std::map<int, Infer*> pointerMap;

void MakeHeelo(){
    std::cout << "\nMap length: " << pointerMap.size();
    Infer* myInfer = new Infer;
    myInfer->preprocess();
    pointerMap[0] = myInfer;
    std::cout << "\nMap length: " << pointerMap.size();
}

void ControHello(){
    std::cout << "\nMap length: " << pointerMap.size();
    
    Infer* myInfer1 = pointerMap[0];
    
    myInfer1->process();
    
}

void FreeHello(){
    std::cout << "\nMap length: " << pointerMap.size();
    Infer* myInfer1 = pointerMap[0];
    myInfer1->postprocess();

    delete myInfer1;
    myInfer1 = nullptr;
    std::cout << "\nMap length: " << pointerMap.size();

}

void Start(std::string input){
    // GetTime();
    std::cout << "enter\n";
    
    // std::cout << input.length()<<" "<<input;
    if (input == "1") {
        std::cout << "输入等于1" << std::endl;
        MakeHeelo();
    } else if (input == "2") {
        std::cout << "输入等于2" << std::endl;
        ControHello();
    } else if (input == "3") {
        std::cout << "输入等于3" << std::endl;
        FreeHello();
    } else if (input == "4") {
        std::cout << "输入等于4" << std::endl;
        // Postprocess2();
    } else {
        std::cout << "输入不等于1或2 3 4" << std::endl;
    }
    std::cout << "enterxx\n";
}