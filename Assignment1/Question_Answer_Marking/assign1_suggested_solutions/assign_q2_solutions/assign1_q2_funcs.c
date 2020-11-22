#include "assign1_q2_funcs.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/wait.h>
#include <unistd.h>

// 4 way merge sort in tutorial 1
int* getSplitIndices(int low, int high)
{
	int step = (high - low) / 4;
	//printf("Step is %d.\n.", step);
	
	// Get 3 mid indices
	int* pMidIndices = (int*) malloc(sizeof(int)*3);
	//printf("The 3 mid indices is:");
	for(int i = 0; i<3; i++){
		int mid_temp = low + (i+1) * step;
		pMidIndices[i] = mid_temp;
		//printf("%d ", pMidIndices[i]);
	}
	//printf("\n");
	return pMidIndices;
}



void merge_4_way(int* array, int low, int mid1, int mid2, int mid3, int high) {
    int n1 = mid1 - low;
    int n2 = mid2 - mid1;
    int n3 = mid3 - mid2;
    int n4 = high - mid3;


    int arr1[n1], arr2[n2], arr3[n3], arr4[n4];
    for (int i = 0; i < n1; ++i) {
        arr1[i] = array[low+i];
    }

    for (int i = 0; i < n2; ++i) {
        arr2[i] = array[mid1+i];
    }

    for (int i = 0; i < n3; ++i) {
        arr3[i] = array[mid2+i];
    }

    for (int i = 0; i < n4; ++i) {
        arr4[i] = array[mid3+i];
    }


    int i = 0, j = 0, k = 0, p = 0, l = low;  

    // choose smaller of the smallest in the three ranges  
    while (l < high) {
        int min_value = INT_MAX;
        int min_index = -1;
        
        if(i < n1 && min_value >= arr1[i]) {
            min_value = arr1[i];
            min_index = 0;
        }

        if(j < n2 && min_value >= arr2[j]) {
            min_value = arr2[j];
            min_index = 1;
        }

        if(k < n3 && min_value >= arr3[k]) {
            min_value = arr3[k];
            min_index = 2;
        }

        if(p < n4 && min_value >= arr4[p]) {
            min_value = arr4[p];
            min_index = 3;
        }


        if(min_index == 0) {
            array[l++] = arr1[i++]; 
        } else if (min_index == 1) {
            array[l++] = arr2[j++]; 
        } else if(min_index == 2) {
            array[l++] = arr3[k++]; 
        } else if(min_index == 3) {
            array[l++] = arr4[p++]; 
        } else {
            printf("error occurs! \n");
            return;
        }
    }  
}  

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
} 
  
/* Performing the merge sort algorithm on the  
given array of values in the rangeof indices  
[low, high). low is minimum index, high is  
maximum index (exclusive) */
void mergesort_4_way_rec(int* array, int low, int high) {
    // If array size is 1 then do nothing  
    if (high - low < 2)  
        return;

    if (high - low == 2) {
        int x = array[low], y = array[low+1];
        if (x > y) swap(&x, &y);
        array[low] = x;
        array[low+1] = y;
        return;
    }  

     if (high - low == 3) {
        int x = array[low], y = array[low+1], z = array[low+2];
        if (x > y) swap(&x, &y);
        if (y > z) swap(&y, &z);
        if (x > y) swap(&x, &y);
        array[low] = x;
        array[low+1] = y;
        array[low+2] = z;
        return;
     }  
  
    // Splitting array into 3 parts  
    int length = high - low;
    int quarter = length / 4;
    int remainder = length % 4;
    int mid1 = low + quarter;
    int mid2 = mid1 + quarter;
    int mid3 = mid2 + quarter;
    if (remainder == 2)
	mid3++;
    if (remainder == 3){
	mid2++;
		mid3 +=2;
    }


    // Sorting 3 arrays recursively  
    mergesort_4_way_rec(array, low, mid1);  
    mergesort_4_way_rec(array, mid1, mid2);  
    mergesort_4_way_rec(array, mid2, mid3);  
    mergesort_4_way_rec(array, mid3, high);  
  
    // Merging the sorted arrays  
    merge_4_way(array, low, mid1, mid2, mid3, high);  
} 




/* from K&R
 - produces a random number between 0 and 32767.*/
int rand_seed=10;
int rand()
{
    rand_seed = rand_seed * 1103515245 +12345;
    return (unsigned int)(rand_seed / 65536) % 32768;
}

int* generateIntArray(int size)
{
	int * array = (int*) malloc(size*sizeof(int));
	for (int i=0; i < size; i++){
        array[i]=rand();
    }
	
	return array;
}

void printArray(int* array, int low, int high)
{
	for(int i=0;i < high - low; i++){
		printf("%d ", array[low + i]);
	}
	printf("\n");
}



void bubble_sort(int *array, int size)
{
	int x,y,t;
	for (x=0; x < size-1; x++)
        for (y=0; y < size-x-1; y++)
            if (array[y] > array[y+1])
            {
                t=array[y];
                array[y]=array[y+1];
                array[y+1]=t;
            }
}

bool verifySortResults(int* array_bubble, int* array_mergesort, int size)
{
	int num_unequal = 0;
	for(int i = 0; i< size; i++){
		if(array_bubble[i] != 	array_mergesort[i])
			num_unequal++;
	}
	
	if(num_unequal!=0){
		printf("The sort result by merge sort is not correct. The number of unequal values: %d.\n", num_unequal);
		return false;
	}
	else{
		printf("The sort result by merge sort is correct, verified by bubble sort.\n");
		return true;
	}
}

void mergesort4Way4Processes(int* array, int low, int high)
{
	pid_t pids[3];
	int stat;

	int size_array = high - low;
	int* pMidIndices = getSplitIndices(low, high);
	pid_t fpid_1 = fork();
	pids[0]=fpid_1;
	if(fpid_1 == 0){
		// The 1st child process
		mergesort_4_way_rec(array, low ,pMidIndices[0]);
		printf("Process ID: %d; Sorted integers: ", getpid());
		printArray(array, low ,pMidIndices[0]);
		kill(getpid(), SIGTERM);
	}
	else{
		pid_t fpid_2 = fork();
		pids[1]=fpid_2;
		if(fpid_2 == 0){
			// The 2nd child process
			mergesort_4_way_rec(array, pMidIndices[0], pMidIndices[1]);
			printf("Process ID: %d; Sorted integers: ", getpid());
			printArray(array, pMidIndices[0], pMidIndices[1]);
			kill(getpid(), SIGTERM);
		}
		else{
			// Create the 3rd child process
			pid_t fpid_3 = fork();
			pids[2]=fpid_3;
			if(fpid_3 == 0){
				// The 3rd child process
				mergesort_4_way_rec(array, pMidIndices[1], pMidIndices[2]);
				printf("Process ID: %d; Sorted integers: ", getpid());
				printArray(array, pMidIndices[1], pMidIndices[2]);
				kill(getpid(), SIGTERM);
			}
			else{
				// Parent process
				mergesort_4_way_rec(array, pMidIndices[2], high);
				printf("Process ID: %d; Sorted integers: ", getpid());
				printArray(array, pMidIndices[2], high);

				for(int i=0;i<3;i++){
					waitpid(pids[i],  &stat, 0);
				}

				merge_4_way(array, low, pMidIndices[0],  pMidIndices[1], pMidIndices[2], high);
				printf("Process ID: %d; Merged integers: ", getpid());
				printArray(array, low, high);
			}
		}
	}	
}

void recursiveMergesort(int* array, int low, int high, int max_num)
{
	int* pMidIndices = getSplitIndices(low, high);

	pid_t pids[3];
	int stat;

	pid_t fpid_1 = fork();
	pids[0] = fpid_1;
	if(fpid_1 == 0){
		//printf("1st child process ID: %d. Parent ID: %d.\n", getpid(), getppid());
		// printf("The 1st child process.\n");

		if(pMidIndices[0] - low > max_num){
			recursiveMergesort(array, low, pMidIndices[0], max_num);
			kill(getpid(), SIGTERM);
		}
		else{
			mergesort_4_way_rec(array, low ,pMidIndices[0]);
			printf("Process ID: %d; Sorted %d integers: ", getpid(), pMidIndices[0]- low);
			printArray(array, low ,pMidIndices[0]);
			kill(getpid(), SIGTERM);
		}				
	}// The 1st child process
	else{
		pid_t fpid_2 = fork();
		pids[1] = fpid_2;
		if(fpid_2 == 0){
			// printf("The 2nd child process.\n");
			//printf("2nd child process ID: %d. Parent ID: %d.\n", getpid(), getppid());
			if(pMidIndices[1] - pMidIndices[0] > max_num){
				recursiveMergesort(array, pMidIndices[0], pMidIndices[1], max_num);
				kill(getpid(), SIGTERM);				
			}
			else{
				mergesort_4_way_rec(array, pMidIndices[0], pMidIndices[1]);
				printf("Process ID: %d; Sorted %d integers: ", getpid(), pMidIndices[1] - pMidIndices[0]);
				printArray(array, pMidIndices[0], pMidIndices[1]);
				kill(getpid(), SIGTERM);
			}
		}// The 2nd child process
		else{
			// Create the 3rd child process
			pid_t fpid_3 = fork();
			pids[2] = fpid_3;
			if(fpid_3 == 0){
				//printf("3rd child process ID: %d. Parent ID: %d.\n", getpid(), getppid());
				// printf("The 3rd child process.\n");				
				if(pMidIndices[2] - pMidIndices[1]> max_num){
					recursiveMergesort(array, pMidIndices[1], pMidIndices[2], max_num);
					kill(getpid(), SIGTERM);
				}
				else{
					mergesort_4_way_rec(array, pMidIndices[1], pMidIndices[2]);
					printf("Process ID: %d; Sorted %d integers: ", getpid(), pMidIndices[2] - pMidIndices[1]);
					printArray(array, pMidIndices[1], pMidIndices[2]);					
					kill(getpid(), SIGTERM);
				}		
			}// The 3rd child process
			else{
				// Parent process
				//printf("parent  process ID: %d. Parent ID: %d.\n", getpid(), getppid());
				// printf("The parent process.\n");
				//printArray(array, low, high);

				if(high - pMidIndices[2] > max_num){
					//printf("test  ddd.\n\n");
					recursiveMergesort(array, pMidIndices[2], high, max_num);

					for(int i=0;i<3;i++){
						waitpid(pids[i],  &stat, 0);
					}

					merge_4_way(array, low, pMidIndices[0],  pMidIndices[1], pMidIndices[2], high);
					printf("Process ID: %d; Merged %d integers: ", getpid(), high - low);
					printArray(array, low, high);
					return;
				}
				else{
					mergesort_4_way_rec(array, pMidIndices[2], high);
					printf("Process ID: %d; Sorted %d integers: ", getpid(), high - pMidIndices[2]);
					printArray(array, pMidIndices[2], high);
					
					for(int i=0;i<3;i++){
						waitpid(pids[i],  &stat, 0);
					}
					
					merge_4_way(array, low, pMidIndices[0],  pMidIndices[1], pMidIndices[2], high);
					printf("Process ID: %d; Merged %d integers: ", getpid(), high - low);
					printArray(array, low, high);
					return;
				} // // The Parent process
			}// Third fork
		}// Second fork
	}// First fork()	
}	
