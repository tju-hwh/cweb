#ifndef INFER_H
#define INFER_H
#include "acl/acl.h"
//dev2
class Infer {
    public:
        Infer();

        ~Infer();

        void process();

        void preprocess();

        void postprocess();
        void Postprocess2();

       
        int32_t deviceId_;
        uint32_t modelId;
        size_t pictureDataSize;
        void *pictureHostData;
        void *pictureDeviceData;
        aclmdlDataset *inputDataSet;
        aclDataBuffer *inputDataBuffer;
        aclmdlDataset *outputDataSet;
        aclDataBuffer *outputDataBuffer;
        aclmdlDesc *modelDesc;
        size_t outputDataSize;
        void *outputDeviceData;
        void *outputHostData;

        aclrtContext *context;
        aclrtStream *stream;
    

        void InitResource();
        void ReadPictureTotHost(const char *picturePath);
        void CopyDataFromHostToDevice();
        void CreateModelInput();
        void CreateModelOutput();
        void LoadPicture(const char *picturePath);
        void LoadModel(const char *modelPath);
        void Inference();
        void PrintResult();
        void UnloadModel();
        void UnloadPicture();
        void DestroyResource();
        void ImageProcess();

    


};


#endif // INFER_H