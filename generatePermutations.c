#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

// 3 6, 4 24, 5 120, 6 720, 7 5040 8 40320, TOO BIG -> 9 362880
#define SET_SIZE 7
#define SET_SIZEF 5040

#define SAME 0
#define LESS -1
#define GREATER 1


int keepRunning = 1;
void intHandler(int dummy) {
    keepRunning = 0;
}



 int setcmp(int ref[], int test[]){
	for (int i=0; i<SET_SIZE; i++){
		if(ref[i]>test[i]){
			return GREATER;
		}else if(ref[i]<test[i]){
			return LESS;
		}
	}
	return SAME;
}

int setcpy(int from[], int to[]){
	for (int i=0; i<SET_SIZE; i++){
		to[i]=from[i];
	}
	return 1;
}

int swap(int set[], int a, int b){
	int temp;
	temp = set[a];
	set[a] = set[b];
	set[b] = temp;
	return 1;
}	

int swapRef(int ref[SET_SIZEF][SET_SIZE], int a, int b){
	int temp[SET_SIZE];
	setcpy(ref[a],temp);
	setcpy(ref[b],ref[a]);
	setcpy(temp,ref[b]);
	return 1;
}	

int printSet(int set[SET_SIZE]){
	for (int i=0; i<SET_SIZE; i++){
		printf("%i ", set[i]);
	}
	printf("\n");
	return 1;
}

int printRef(int ref[SET_SIZEF][SET_SIZE]){
	for (int i=0; i<SET_SIZEF; i++){
		printf("Ref Set %i:", i);
		if(!printSet(ref[i]))
			printf("Error: Ref set %i not printed.", i);
	}
}

//Heap's algorithm
int permutation=0;
void permuteRecursive(int length, int ref[SET_SIZEF][SET_SIZE], int set[SET_SIZE]){
	if(length == 1){
		setcpy(set, ref[permutation]);
		permutation++;
	}else{
		for(int i=0; i<length; i++){
			permuteRecursive(length-1, ref, set);
			if (length%2 == 1){
				swap(set, 0, length-1);
			}else{
				swap(set, i, length-1);
			}
		}
	}
}

int buildSet(int set[SET_SIZE]){
	for (int i=0; i<SET_SIZE; i++){
		set[i]=i+1;
	}
	return 1;
}

int badShuffle(int set[SET_SIZE]){
	for(int i=0; i<SET_SIZE; i++){
		double scaled = (double)random()/RAND_MAX;
		int swapindex = (SET_SIZE)*scaled;
		swap(set, i, swapindex);
	}
	return 1;
}

int goodShuffle(int set[SET_SIZE]){
	for(int i=0; i<SET_SIZE; i++){
		double scaled = (double)random()/RAND_MAX;
		int swapindex = ((SET_SIZE-i)*scaled)+i;
		swap(set, i, swapindex);
	}
	return 1;
}

int kevinShuffle(int set[SET_SIZE], int iterations){
	for(int i=0; i<iterations; i++){
		double scaled = (double)random()/RAND_MAX;
		int swapindex1 = ((SET_SIZE)*scaled);
		scaled = (double)random()/RAND_MAX;
		int swapindex2 = ((SET_SIZE)*scaled);
		swap(set, swapindex1, swapindex2);
	}
	return 1;
}

int setmatch(int ref[SET_SIZEF][SET_SIZE], int set[SET_SIZE]){
	for(int i=0; i<SET_SIZEF; i++){
		if(setcmp(ref[i],set)==SAME){
			return i;
		}
	}
	return -1;
}

int partition(int ref[SET_SIZEF][SET_SIZE], int low, int high){
	int pivot[SET_SIZE];
	setcpy(ref[high], pivot);
	int i = low - 1;
	for(int j=low; j<high; j++){
		if(setcmp(ref[j], pivot)==LESS){
			i++;
			swapRef(ref, i, j);
		}
	}
	if(setcmp(ref[high],ref[i+1])==LESS){
		swapRef(ref, i+1, high);
	}
	return i+1;
}

int sort(int ref[SET_SIZEF][SET_SIZE], int low, int high){
	if(low<high){
		int p = partition(ref, low, high);
		sort(ref, low, p-1);
		sort(ref, p+1, high);
	}
	return 0;
}

int main(int argc, char **argv){
	signal(SIGINT, intHandler);
	
	int set[SET_SIZE];
	buildSet(set);
	//printSet(set);
	
	int ref[SET_SIZEF][SET_SIZE];
	permuteRecursive(SET_SIZE, ref, set);
	//printf("Reference Set Generated with %i permutations\n", permutation);
	//printRef(ref);
	sort(ref, 0, SET_SIZEF-1);
	//printRef(ref);
	
	if(argc==2)  {
		int sequence = atoi(argv[1]);
		printSet(ref[sequence]);
		return 0;
	}
	
	//search for duplicates
	if(0){
		int dups=0;
		int iterations=0;
		for (int i=0; i<SET_SIZEF; i++){
			for (int j=i+1; j<SET_SIZEF; j++){
				if(setcmp(ref[i],ref[j])==SAME){
					printf("Identical Set: %i, %i \n", i, j);
					dups=1;
				}
				iterations++;
			}
		}
		if(!dups) printf("No duplicate sets found in reference set after %i iterations\n", iterations);
	}

	if(1){
#define KEVINSETS 3
		int badmatchcount[SET_SIZEF] = {0};
		int goodmatchcount[SET_SIZEF] = {0};
		int kevinmatchcount[KEVINSETS][SET_SIZEF] = {0};
		int match=0;
		
		while(keepRunning){
		//for(int i=0; i<1000000000; i++){
			//test badShuffle
			buildSet(set);
			badShuffle(set);
			match = setmatch(ref, set);
			badmatchcount[match]++;
			
			//test goodShuffle
			//buildSet(set);
			//goodShuffle(set);
			//match = setmatch(ref, set);
			//goodmatchcount[match]++;
			
			if(0){
				//test kevinShuffle
				for(int j=0;j<KEVINSETS;j++){
					buildSet(set);
					kevinShuffle(set,SET_SIZE*(j+1));
					match = setmatch(ref, set);
					kevinmatchcount[j][match]++;
				}
			}
		}
		
		//printf("Set, Bad, Good");
		if(0){
			for(int j=0;j<KEVINSETS;j++){
				printf(", Kevin%i", j);
			}
		}
		//printf("\n");
		for(int i=0; i<SET_SIZEF; i++){
				//printf("%i, %i, %i", i, badmatchcount[i], goodmatchcount[i]);
				printf("%i", badmatchcount[i]);
				if(0){
					for(int j=0;j<KEVINSETS;j++){
						printf(", %i",kevinmatchcount[j][i]);
					}
				}
				printf("\n");
		}
	}
	return 0;
}