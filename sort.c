#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

/*
 * Binary tree structure
 * int arr[] = { 5, 10, 3, 2, 1 }
 *       5
 *      /  \
 *    10    3
 *   /  \
 *  2    1
 */

// Used by heapSort()
// Reorganizes the heap to put the largest one at the top
// n is size
void heapify(int arr[], int n, int i) {
    int largest = i; // Index of largest item
    int left = (2 * i) + 1;
    int right = (2 * i) + 2;

    // Set largest
    if (left < n && arr[left] > arr[largest]) // Left
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
    // Build our max heap (a heap that is organized and all subclasses are less than or equal to parent class
    for (int i = (n / 2) - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Restart heapify
        heapify(arr, i, 0);
    }
}

// Used by mergeSort()
void merge(int pData[], int l, int mid, int r) {
    int firstHalf = mid - l + 1;
    int secHalf = r - mid;

    // Temporary arrays
    int *left = malloc(firstHalf * sizeof(int));
    int *right = malloc(secHalf * sizeof(int));

    // Move values to temp arrays
    for (int i = 0; i < firstHalf; i++)
        left[i] = pData[i + l];
    for (int i = 0; i < secHalf; i++)
        right[i] = pData[mid + i + 1];

    int leftIterator = 0;
    int rightIterator = 0;
    int mergeIterator = l;

    // We compare and move values back to pData[]
    while (leftIterator < firstHalf && rightIterator < secHalf) {
        if (left[leftIterator] <= right[rightIterator]) {
            pData[mergeIterator] = left[leftIterator];
            leftIterator++;
        }
        else {
            pData[mergeIterator] = right[rightIterator];
            rightIterator++;
        }

        mergeIterator++;
    }

    // Empty any remaining values from left[] or right[] if present
    while (leftIterator < firstHalf) {
        pData[mergeIterator] = left[leftIterator];
        leftIterator++;
        mergeIterator++;
    }
    while (rightIterator < secHalf) {
        pData[mergeIterator] = right[rightIterator];
        rightIterator++;
        mergeIterator++;
    }

    free(left);
    free(right);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if (l < r) {
        int mid = (l + r) / 2;

        // Sort left and right
        mergeSort(pData, l, mid);
        mergeSort(pData, mid + 1, r);

        merge(pData, l, mid, r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
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
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt",
                          "input2.txt",
                          "input3.txt",
                          "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}