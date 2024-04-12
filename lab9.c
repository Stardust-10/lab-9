#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 23

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct node *recordArray[MAXSIZE];
};

struct node {
	struct RecordType *nodeRecord;
	struct node *nextNode;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0; i<hashSz; ++i)
	{
		if(pHashArray[i].recordArray == NULL)
			continue;

		// if index is occupied with any records, print all
		else if (pHashArray[i].recordArray != NULL) {

			struct node *walker = pHashArray->recordArray[i];
			printf("index %d -> ", i);

			while(walker != NULL) {
				printf("%d, %c, %d", walker->nodeRecord->id, walker->nodeRecord->name, walker->nodeRecord->order);

				if(walker->nextNode != NULL) {
					printf(" -> ");
				}

				walker = walker->nextNode;
			}
		}
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	struct HashType *pHT = malloc(sizeof(struct HashType)); //Pointer to hash table array
	
	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	for(int i = 0; i<MAXSIZE; i++) {
		pHT->recordArray[i] = NULL;
	}

	for(int i = 0; i<=recordSz; i++) {

		int currRecKey = hash(pRecords[i].id);

		//If array index NULL, makes a node at the index and fills information.
		if(pHT->recordArray[currRecKey] == NULL) {
			pHT->recordArray[currRecKey] = malloc(sizeof(struct node));
			pHT->recordArray[currRecKey]->nodeRecord = &pRecords[i];
			pHT->recordArray[currRecKey]->nextNode = NULL;
		}

		//if a node is present, traverse to the end of the list and add a node
		else {
			struct node *walker = pHT->recordArray[currRecKey];

			while(walker->nextNode != NULL){
				walker = walker->nextNode;
			}

			//Creates a temp node to contain data.
			struct node *temp = malloc(sizeof(struct node));
				temp->nodeRecord = &pRecords[i];
				temp->nextNode = NULL;
				walker->nextNode = temp;
		}
	}

	displayRecordsInHash(pHT, MAXSIZE);

	// Your hash implementation

	//Create hash table w/ 23 spaces (there are 31 items)
	//HashType stores the 23 arrays with linked lists.
	//Access index with hash function
	//Create node if index points to null
	//if index has a node, traverse to the end and add a new node
	//print out table
}