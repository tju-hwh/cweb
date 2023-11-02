#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "acl/acl.h"

extern int32_t deviceId_;
extern uint32_t modelId;
extern size_t pictureDataSize;
extern void *pictureHostData;
extern void *pictureDeviceData;
extern aclmdlDataset *inputDataSet;
extern aclDataBuffer *inputDataBuffer;
extern aclmdlDataset *outputDataSet;
extern aclDataBuffer *outputDataBuffer;
extern aclmdlDesc *modelDesc;
extern size_t outputDataSize;
extern void *outputDeviceData;
extern void *outputHostData;

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
void ImageProcess(int a);

void Aaa();

#endif // IMAGEPROCESS_H
