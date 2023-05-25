#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create the memory
void createMemory(int memorySize, int pageSize){
  int memory[memorySize/pageSize];
  int i;
  for(i = 0; i < memorySize/pageSize; i++){
    memory[i] = -1;
  }
}

int main(int argc, char* argv[]){
  // Variables
  char* algorithm = argv[1];
  char* inputFile = argv[2];
  int pageSize = atoi(argv[3]); 
  int memorySize = atoi(argv[4]);

  // Print variables
  printf("Algorithm: %s\n", algorithm);
  printf("Input file: %s\n", inputFile);
  printf("Memory size: %d\n", memorySize);
  printf("Page size: %d\n", pageSize);

  // Create memory
  createMemory(memorySize, pageSize);
  

  return 0;
}