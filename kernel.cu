#include"cuda_runtime.h"
#include"device_launch_parameters.h"

#include"rtweekend.h"

#include<iostream>
#include<cstdio>
#include"hittable.h"
#include"hittable_list.h"
#include"sphere.h"
#include"camera.h"
#include"Metal.h"

cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size);

__global__ void addKernel(int* c, const int* a, const int* b)
{
	int i = threadIdx.x;

	c[i] = a[i] + b[i];
}

int main()
{
	
#pragma region Ray Tracing - CUDA setup
	std::cout << "CUDA Vector Addtion Example" << std::endl;
	const int arraySize = 5;
	const int a[arraySize] = { 1,2,3,4,5 };
	const int b[arraySize] = { 10,20,30,40,50 };	
	int c[arraySize] = { 0 };

	cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	printf("{1,2,3,4,5}+{10,20,30,40,50}={%d,%d,%d,%d,%d}\n", c[0], c[1], c[2], c[3], c[4]);


	//프로그램 종료 전 GPU 디바이스 리셋
	//Nsight, Visual profiler 등 프로파일링 도구가 완전한 트레이스를 표시하려면 필요
	cudaStatus = cudaDeviceReset();

	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	std::cin.get();
#pragma endregion

}

// CUDA 벡터 덧셈 수행 헬퍼 함수
// CUDA의 기본 흐름을 보여줌
// 1. GPU 디바이스 선택
// 2. GPU 메모리 할당 (cudaMalloc)
// 3. 호스트 -> 디바이스 메모리 복사 (cudaMemcpy)
// 4. 커널 실행 (addKernel<<<>>)
// 5. 디바이스 -> 호스트 메모리 복사 (cudaMemcpy)
// 6. GPU 메모리 해제 (cudaFree)
cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size)
{
	// 디바이스(GPU)측 포인터 선언
	int* dev_a = 0;
	int* dev_b = 0;
	int* dev_c = 0;
	cudaError_t cudaStatus;


	// 사용할 GPU 선택
	cudaStatus = cudaSetDevice(0);

	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaSetDevice failed!");
		goto Error;
	}

	// GPU 메모리 할당 - 입력 2개 (dev_a, dev_b) 출력 1개 (dev_c)
	cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}


	// 호스트(CPU) 메모리 -> 디바이스(GPU) 메모리로 입력 데이터 복사
	cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cuddaMemcpy failed!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cuddaMemcpy failed!");
		goto Error;
	}


	// 커널 실행 <<<1,size>>> = 1개 블록, size개 스레드
	// 각 스레드가 배열의 한 원소씩 병렬로 덧셈 수행

	addKernel<<<1, size>>> (dev_c, dev_a, dev_b);


	// 커널 실행 시 에러 확인
	cudaStatus = cudaGetLastError();

	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// GPU 커널이 완료할 때 까지 대기
	cudaStatus = cudaDeviceSynchronize();

	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}

	// 디바이스(GPU) -> 호스트(CPU)로 결과 복사
	cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);

	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

Error:
	cudaFree(dev_c);
	cudaFree(dev_a);
	cudaFree(dev_b);

	return cudaStatus;
}


