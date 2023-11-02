#include <iostream>
#include "infer.h"

#include "acl/acl.h"

#include <fstream>
#include <cstring>
#include <map>

#include "util.h"

using namespace std;







// AscendCL初始化、运行管理资源申请（指定计算设备）
void Infer::InitResource()
{
	aclError ret = aclInit(nullptr);
    cout<<"\nacl init re:"<<ret<<"\n";
	ret = aclrtSetDevice(deviceId_);
}


// 申请内存，使用C/C++标准库的函数将测试图片读入内存
void Infer::ReadPictureTotHost(const char *picturePath)
{
	string fileName = picturePath;
	ifstream binFile(fileName, ifstream::binary);
	binFile.seekg(0, binFile.end);
	pictureDataSize = binFile.tellg();
	binFile.seekg(0, binFile.beg);
	aclError ret = aclrtMallocHost(&pictureHostData, pictureDataSize);
	binFile.read((char*)pictureHostData, pictureDataSize);
	binFile.close();
}


// 申请Device侧的内存，再以复制内存的方式将内存中的图片数据传输到Device
void Infer::CopyDataFromHostToDevice()
{
	aclError ret = aclrtMalloc(&pictureDeviceData, pictureDataSize, ACL_MEM_MALLOC_HUGE_FIRST);
	ret = aclrtMemcpy(pictureDeviceData, pictureDataSize, pictureHostData, pictureDataSize, ACL_MEMCPY_HOST_TO_DEVICE);
}

//在模型加载之后，模型执行之前，需要准备输入、输出数据结构，将输入数据传输到模型输入数据结构的对应内存中
// 准备模型推理的输入数据结构
void Infer::CreateModelInput()
{
	// 创建aclmdlDataset类型的数据，描述模型推理的输入
	inputDataSet = aclmdlCreateDataset();
	inputDataBuffer = aclCreateDataBuffer(pictureDeviceData, pictureDataSize);
	aclError ret = aclmdlAddDatasetBuffer(inputDataSet, inputDataBuffer);
}


// 准备模型推理的输出数据结构
void Infer::CreateModelOutput()
{
	// 创建模型描述信息
	modelDesc =  aclmdlCreateDesc();
	aclError ret = aclmdlGetDesc(modelDesc, modelId);
	
	// 创建aclmdlDataset类型的数据，描述模型推理的输出
	outputDataSet = aclmdlCreateDataset();
	
	// 获取模型输出数据需占用的内存大小，单位为Byte
	outputDataSize = aclmdlGetOutputSizeByIndex(modelDesc, 0);
	
	// 申请输出内存
	ret = aclrtMalloc(&outputDeviceData, outputDataSize, ACL_MEM_MALLOC_HUGE_FIRST);
	outputDataBuffer = aclCreateDataBuffer(outputDeviceData, outputDataSize);
	ret = aclmdlAddDatasetBuffer(outputDataSet, outputDataBuffer);
}


// 将图片数据读入内存
void Infer::LoadPicture(const char* picturePath)
{	
	std::cout << " 开始图片读入内存 ";
	GetTime();
	ReadPictureTotHost(picturePath);

	std::cout << " 开始图片内存拷贝 ";
	GetTime();
	CopyDataFromHostToDevice();
}


// 加载模型
void Infer::LoadModel(const char* modelPath)
{
    //aclmdlLoadFromFile从文件加载离线模型数据，由系统内部管理内存
	aclError ret = aclmdlLoadFromFile(modelPath, &modelId);
}


// 执行推理
void Infer::Inference()
{
    CreateModelInput();
	CreateModelOutput();
    for( int a = 1; a < 10; a = a + 1 )
    {
       	cout<<"a:"<<a;
	    aclError ret = aclmdlExecute(modelId, inputDataSet, outputDataSet);
	    
    }
	aclError ret = aclmdlExecute(modelId, inputDataSet, outputDataSet);
}


// 在终端上屏显测试图片的top5置信度的类别编号
void Infer::PrintResult()
{
	aclError ret = aclrtMallocHost(&outputHostData, outputDataSize);
	ret = aclrtMemcpy(outputHostData, outputDataSize, outputDeviceData, outputDataSize, ACL_MEMCPY_DEVICE_TO_HOST);
	float* outFloatData = reinterpret_cast<float *>(outputHostData);
	
	map<float, unsigned int, greater<float>> resultMap;
	for (unsigned int j = 0; j < outputDataSize / sizeof(float);++j)
	{
		resultMap[*outFloatData] = j;
		outFloatData++;
	}
	
	int cnt = 0;
	for (auto it = resultMap.begin();it != resultMap.end();++it)
	{
		if(++cnt > 5)
		{
			break;
		}
		printf("top %d: index[%d] value[%lf] \n", cnt, it->second, it->first);
	}
}


// 卸载模型
void Infer::UnloadModel()
{
	aclmdlDestroyDesc(modelDesc);
	aclmdlUnload(modelId);
}


// 释放内存、销毁推理相关的数据类型，防止内存泄露
void Infer::UnloadPicture()
{
	aclError ret = aclrtFreeHost(pictureHostData);
	pictureHostData = nullptr;
	ret = aclrtFree(pictureDeviceData);
	pictureDeviceData = nullptr;
	aclDestroyDataBuffer(inputDataBuffer);
	inputDataBuffer = nullptr;
	aclmdlDestroyDataset(inputDataSet);
	inputDataSet = nullptr;
	
	ret = aclrtFreeHost(outputHostData);
	outputHostData = nullptr;
	ret = aclrtFree(outputDeviceData);
	outputDeviceData = nullptr;
	aclDestroyDataBuffer(outputDataBuffer);
	outputDataBuffer = nullptr;
	aclmdlDestroyDataset(outputDataSet);
	outputDataSet = nullptr;
}


// AscendCL去初始化、运行管理资源释放（指定计算设备）
void Infer::DestroyResource()
{
	aclError ret = aclrtResetDevice(deviceId_);
	aclFinalize();
    // aclError ret = aclrtDestroyStream(stream);
    // ret = aclrtDestroyContext(context);
}

void Infer::ImageProcess()
{
    cout<<"access";
	// 1.定义一个资源初始化的函数，用于AscendCL初始化、运行管理资源申请（指定计算设备）
	InitResource();
	
	// 2.定义一个模型加载的函数，加载图片分类的模型，用于后续推理使用
	const char *mdoelPath = "./model/resnet50.om";
	LoadModel(mdoelPath);
	
	// 3.定义一个读图片数据的函数，将测试图片数据读入内存，并传输到Device侧，用于后续推理使用
    
    const char *picturePath = "./data/dog2_1024_683.bin";
    
	
	LoadPicture(picturePath);
	
    // 4.定义一个推理的函数，用于执行推理
	Inference();

    // 5.定义一个推理结果数据处理的函数，用于在终端上屏显测试图片的top5置信度的类别编号
	PrintResult();
  
	// 6.定义一个模型卸载的函数，卸载图片分类的模型
	UnloadModel();
	
	// 7.定义一个函数，用于释放内存、销毁推理相关的数据类型，防止内存泄露
	UnloadPicture();
	
	// 8.定义一个资源去初始化的函数，用于AscendCL去初始化、运行管理资源释放（指定计算设备）
	DestroyResource();
	cout<<"11111111111111";
}


void Infer::preprocess(){
	aclError ret = aclInit(nullptr);
    std::cout<<"\nacl init re:"<<ret<<"\n";
	ret = aclrtSetDevice(0);
    std::cout<<"\nacl set device re:"<<ret<<"\n";

    std::cout << " process\n";
    deviceId_ = 0;
    size_t pictureDataSize = 0;
    size_t outputDataSize = 0;
	modelId = 1;


	// char modleType = 'D';
 
	// switch(modleType)
	// {
	// case 'A' :
	// 	char *mdoelPath = "./model/resnet50.om";
	// 	char *picturePath = "./data/dog2_1024_683.bin";
	// 	break;
	// case 'B' :
	// 	char *mdoelPath = "./model/AnimeGANv2_256.om";
	// 	char *picturePath = "./data/car.bin";
	// case 'C' :
	// 	char *mdoelPath = "./model/shadownet_tf_64batch.om";
	// 	char *picturePath = "./data/batch_data_000.bin";
	// 	break;
	// case 'D' :
	// 	cout << "您通过了" << endl;
	// 	break;
	// case 'F' :
	// 	cout << "最好再试一下" << endl;
	// 	break;
	// default :
	// 	// cout << "无效的成绩" << endl;
	// }

    // InitResource();
   
	


	// GetTime();
    // const char *picturePath = "./data/dog2_1024_683.bin";
	// const char *picturePath = "./data/car.bin";
	char *picturePath = "./data/batch_data_000.bin";
	// char *picturePath = "./data/gnn_model_input.bin";
    LoadPicture(picturePath);


	std::cout << " 开始模型加载 ";
	GetTime();
	// const char *mdoelPath = "./model/resnet50.om";
	// const char *mdoelPath = "./model/AnimeGANv2_256.om";
	char *mdoelPath = "./model/shadownet_tf_64batch.om";
	// char *mdoelPath = "./model/ngnn_last_6input.om";
    LoadModel(mdoelPath);

	std::cout << " 申请模型输入输出结构内存 ";
	GetTime();
    CreateModelInput();
	CreateModelOutput();
    // std::cout << " p111\n";
    // std::cout <<modelId<< " modelId\n";
    // std::cout <<inputDataSet<< " inputDataSet\n";
    // std::cout <<outputDataSet<< " outputDataSet\n";

	std::cout << " 开始推理 ";
	GetTime();
    ret = aclmdlExecute(modelId, inputDataSet, outputDataSet);
	std::cout << ret<< " mdlExecute ret\n";

	std::cout << " 返回结果 ";
	GetTime();
    PrintResult();
	
	// ret = aclrtGetCurrentContext(context);
	// std::cout << ret<< " aclrt Get Current Context ret\n";
	// std::cout << " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	std::cout << " 卸载输入内存 ";
	GetTime();
	UnloadPicture();

	std::cout << " 卸载模型 ";
	GetTime();
	UnloadModel();
    std::cout << " 结束时间 ";
	GetTime();

	DestroyResource();
}

void Infer::process(){
    std::cout << " process\n";
    std::cout << " p111\n";
    std::cout <<modelId<< " modelId\n";
    std::cout <<inputDataSet<< " inputDataSet\n";
    std::cout <<outputDataSet<< " outputDataSet\n";
	
	aclError ret = aclrtSetCurrentContext(context);
	std::cout << ret<< " aclrt Set Current Context ret\n";

    for( int a = 1; a < 10; a = a + 1 )
    {
       	
	    aclError ret = aclmdlExecute(modelId, inputDataSet, outputDataSet);
	    cout<<"a:"<<a << "ret:"<< ret <<" ";
    }
    ret = aclmdlExecute(modelId, inputDataSet, outputDataSet);
    PrintResult();
}

void Infer::postprocess(){
    std::cout << " process\n";
    UnloadModel();
    UnloadPicture();
    // DestroyResource();
}

void Infer::Postprocess2(){
    // std::cout << " process\n";
    // UnloadModel();
    // UnloadPicture();
    DestroyResource();
}

Infer::Infer(){
    std::cout << "Infer constructed\n";
    // deviceId_ = 0;
    // size_t pictureDataSize = 0;
    // size_t outputDataSize = 0;
    // preprocess();
}

Infer::~Infer(){
    std::cout << "Infer destroyed\n";

}
