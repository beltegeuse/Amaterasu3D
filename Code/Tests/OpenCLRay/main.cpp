#include <CL/cl.h>

// Amaterasu Headers
#include <Utilities/File.h>

// C++ Headers
#include <iostream>
#include <map>

// C Headers ...
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

class OpenCLManager
{
private:
	typedef std::map<std::string, cl_program> ProgramLists;

	/*
	 * Attributes
	 */
	cl_platform_id m_Platform;
	cl_device_id m_Device;
	cl_context m_Ctx;
	cl_command_queue m_Queue;
	ProgramLists m_Program;

public:
	OpenCLManager()
	{
		// On récupère la platforme grâce à notre fonction précédente
		m_Platform = GetPlatform();

		// On récupère les devices
		// TODO: Multi GPU ?
//		cl_uint num_devices;
//		clGetDeviceIDs(m_Platform, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
		clGetDeviceIDs(m_Platform, CL_DEVICE_TYPE_GPU, 1, &m_Device, NULL);

		// On créé le contexte
		m_Ctx = clCreateContext(0, 1, &m_Device, NULL, NULL, NULL);

		// On créé la liste de commande
		m_Queue = clCreateCommandQueue(m_Ctx, m_Device, 0, NULL);
	}

	virtual ~OpenCLManager()
	{
		clReleaseCommandQueue(m_Queue);
		clReleaseContext(m_Ctx);
	}

	cl_kernel LoadKernel(const std::string& sourcefile, const std::string& kernel)
	{
		LoadProgram(sourcefile);
		clCreateKernel(m_Program[sourcefile], kernel.c_str(), NULL);
	}


	cl_context GetContext() { return m_Ctx; }
	cl_command_queue GetQueue() { return m_Queue; }

private:

	void LoadProgram(const std::string& file)
	{
		// Program already loaded
		if(m_Program.find(file) != m_Program.end())
			return;

		// Read the file
		FILE* fp = fopen(file.c_str(), "rb");
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		rewind(fp);
		char* src = (char*)malloc(size+1);
		fread(src, 1, size, fp);
		src[size] = 0x0;
		fclose(fp);

		// Create OpenCL program
		cl_int errorCode;
		const char* tFile[1] = {src};
		cl_program prog = clCreateProgramWithSource(m_Ctx, 1, tFile, &size, &errorCode);
		assert(errorCode == CL_SUCCESS);

		// Build it !
		errorCode = clBuildProgram(prog, 0, NULL, NULL, NULL, NULL);

		// Shows the log
		char* build_log;
		size_t log_size;
		// First call to know the proper size
		clGetProgramBuildInfo(prog, m_Device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		if(log_size == 0)
		{
			build_log = new char[log_size+1];
			// Second call to get the log
			clGetProgramBuildInfo(prog, m_Device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
			build_log[log_size] = '\0';

			std::cout << "===== BUILD INFORMATION ====" << std::endl;
			std::cout << build_log << std::endl;
			std::cout << "======== BUILD END =========" << std::endl;
			delete[] build_log;
		}
		else
		{
			std::cout << "[INFO] No build log for : " << file << ":)" << std::endl;
		}
		assert(errorCode == CL_SUCCESS);

		m_Program[file] = prog;
	}

	cl_platform_id GetPlatform()
	{
		cl_uint numPlatforms;
		cl_platform_id platform;

		// On récupère le nombre de plateformes disponibles
		clGetPlatformIDs(0, NULL, &numPlatforms);
		if(numPlatforms > 0)
		{
			std::cout << "[INFO] GetPlatform : " << numPlatforms << std::endl;
			// On récupère la liste des plateformes disponibles
			cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);
			clGetPlatformIDs(numPlatforms, platforms, NULL);

			char platformName[100];
			int i = 0;
			// On récupère la meilleure plateforme
			for(i=0; i < numPlatforms; i++){
				clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(platformName), platformName, NULL);
				platform = platforms[i];
				std::cout << "   * Platform : " << platformName << std::endl;
				if(!strcmp(platformName, "Advanced Micro Devices, Inc.") || !strcmp(platformName, "NVIDIA")){
					break;
				}
			}
			free(platforms);
		}
		else
		{
			std::cout << "[INFO] No platform find :(" << std::endl;
		}

		return platform;
	}

};


int main()
{
	OpenCLManager cl;
	cl_kernel addKernel = cl.LoadKernel("../Donnees/Kernels/test_kernel.cl", "add");

	// Initialisation for the data
	int sizeData = 123456;
	float* dataA = new float[sizeData];
	float* dataB = new float[sizeData];

	for(int i = 0; i < sizeData; i++)
	{
		dataA[i] = i;
		dataB[i] = -i;
	}
	int mensize = sizeof(float)*1024;

	// Create openCL buffers
	cl_mem src_a_d = clCreateBuffer(cl.GetContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mensize, dataA, NULL);
	cl_mem src_b_d = clCreateBuffer(cl.GetContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mensize, dataB, NULL);
	cl_mem res_d = clCreateBuffer(cl.GetContext(), CL_MEM_WRITE_ONLY, mensize, NULL, NULL);

	// Enqueuing parameters
	// Note that we inform the size of the cl_mem object, not the size of the memory pointed by it
	cl_int error = clSetKernelArg(addKernel, 2, sizeof(cl_mem), &src_a_d);
	error |= clSetKernelArg(addKernel, 1, sizeof(cl_mem), &src_b_d);
	error |= clSetKernelArg(addKernel, 0, sizeof(cl_mem), &res_d);
	error |= clSetKernelArg(addKernel, 3, sizeof(size_t), &sizeData);
	assert(error == CL_SUCCESS);

//	const size_t local_ws = ;	// Number of work-items per work-group
	// shrRoundUp returns the smallest multiple of local_ws bigger than size
	const size_t global_ws = sizeData; // Total number of work-items
//	std::cout << local_ws << "x" << global_ws << std::endl;
	error = clEnqueueNDRangeKernel(cl.GetQueue(), addKernel, 1, NULL, &global_ws, NULL, 0, NULL, NULL);
	assert(error == CL_SUCCESS);

	float* check = new float[sizeData];
	clEnqueueReadBuffer(cl.GetQueue(), res_d, CL_TRUE, 0, mensize, check, 0, NULL, NULL);

	float sum = 0.0;
	for(int i = 0; i < sizeData; ++i)
		sum += check[i];

	std::cout << "Sum : " << sum << std::endl;
	std::cout << "check[0] : " << check[0] << std::endl;

	clReleaseKernel(addKernel);
	clReleaseMemObject(src_a_d);
	clReleaseMemObject(src_b_d);
	clReleaseMemObject(res_d);

	delete[] dataA;
	delete[] dataB;
	delete[] check;

	return 0;
}

