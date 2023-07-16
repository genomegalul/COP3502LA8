#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void heapSort(int arr[], int n)
{
    extraMemoryAllocated = 0;

    for (int i = n / 2 - 1; i >= 0; i--) 
    {
        int parent = i;
        int child = 2 * parent + 1;

        while (child < n) 
        {
            if (child + 1 < n && arr[child] < arr[child + 1]) child++;

            if (arr[parent] < arr[child]) 
            {
                int temp = arr[parent];
                arr[parent] = arr[child];
                arr[child] = temp;

                parent = child;
                child = 2 * parent + 1;
            }
            else 
            {
                break;
            }
        }
    }

    for (int i = n - 1; i > 0; i--) 
    {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        int parent = 0;
        int child = 2 * parent + 1;

        while (child < i) 
        {
            if (child + 1 < i && arr[child] < arr[child + 1]) child++;

            if (arr[parent] < arr[child]) 
            {
                int temp = arr[parent];
                arr[parent] = arr[child];
                arr[child] = temp;

                parent = child;
                child = 2 * parent + 1;
            }
            else 
            {
                break;
            }
        }
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int mid = l + (r - l) / 2;

        mergeSort(arr, l, mid);
        mergeSort(arr, mid + 1, r);

        int leftSize = mid - l + 1;
        int rightSize = r - mid;

        int* leftArr = (int*)malloc(leftSize * sizeof(int));
        int* rightArr = (int*)malloc(rightSize * sizeof(int));

        extraMemoryAllocated += leftSize * sizeof(int) + rightSize * sizeof(int);

        for (int i = 0; i < leftSize; i++)
            leftArr[i] = arr[l + i];
        for (int j = 0; j < rightSize; j++)
            rightArr[j] = arr[mid + 1 + j];

        int i = 0;
        int j = 0;
        int k = l;

        while (i < leftSize && j < rightSize) {
            if (leftArr[i] <= rightArr[j]) {
                arr[k] = leftArr[i];
                i++;
            }
            else {
                arr[k] = rightArr[j];
                j++;
            }
            k++;
        }

        while (i < leftSize) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }

        while (j < rightSize) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }

        free(leftArr);
        free(rightArr);
    }
}

// parses input file to an integer array
int parseData(char* inputFileName, int** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n, * data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int*)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i = 0; i < 100; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

	for (i = 0; i < 4; ++i)
	{
		int* pDataSrc, * pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int*)malloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}

}