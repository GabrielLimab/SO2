#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Page{
  char id[8];
  int lastAccess;
  int reference;
  int altered;
} Page;

// Global variables for arguments
char* algorithm;
char* inputFile;
int pageSize;
int memorySize;
int debug;

void printUsage() {
  fprintf(stderr, "Usage: ./tp2virtual <algorithm> <input file> <page size> <memory size> <debug?>\n");
  fprintf(stderr, "Algorithms: lru, 2a, fifo, random\n");
  fprintf(stderr, "Input file: string\n");
  fprintf(stderr, "Page size: positive integer, recommended range [2, 64]\n");
  fprintf(stderr, "Memory size: positive integer, recommended range [128, 16384]\n");
  fprintf(stderr, "Debug (optional): 0, 1\n");
}

void parseArgs(int argc, char* argv[]){
  if (argc < 5){
    printUsage();
    exit(1);
  }

  if (strcmp(argv[1], "lru") != 0 && strcmp(argv[1], "2a") != 0 && strcmp(argv[1], "fifo") != 0 && strcmp(argv[1], "random") != 0) {
    fprintf(stderr, "Invalid algorithm\n");
    printUsage();
    exit(1);
  }

  if (atoi(argv[3]) <= 0){
    fprintf(stderr, "Invalid page size\n");
    printUsage();
    exit(1);
  }

  if (atoi(argv[4]) <= 0){
    fprintf(stderr, "Invalid memory size\n");
    printUsage();
    exit(1);
  }
  
  if (argc == 6){
    if (strcmp(argv[5], "0") != 0 && strcmp(argv[5], "1") != 0){
      fprintf(stderr, "Invalid debug value\n");
      printUsage();
      exit(1);
    }
    else{
      debug = atoi(argv[5]);
    }
  }
  else{
    debug = 0;
  }

  algorithm = argv[1];
  inputFile = argv[2];
  pageSize = atoi(argv[3]);
  memorySize = atoi(argv[4]);
}

int main(int argc, char* argv[]){
  // Parse arguments
  parseArgs(argc, argv);

  // Print variables
  printf("Algorithm: %s\n", algorithm);
  printf("Input file: %s\n", inputFile);
  printf("Memory size: %d KB\n", memorySize);
  printf("Page size: %d KB\n", pageSize);
  printf("Debug: %d\n", debug);

  Page* memory = malloc(sizeof(Page) * memorySize/pageSize);

  int tmp = pageSize * 1024;
  int s = 0;

  while(tmp > 1){
    tmp = tmp >> 1;
    s++;
  }

  s = s/4;

  printf("S: %d\n", s);

  char addr[8];
  char rw;
  FILE* file = fopen(inputFile, "r");
  int i = 0;
  int time = 0;

  while(fscanf(file, "%s %c", addr, &rw) != EOF){
    // check if page is in memory
    int k = 0;
    int found = 0;
    char id[8];
    strncpy(id, addr, 8-s);

    while(k < memorySize/pageSize){
      if (memory[k].id[0] == '\0'){
        break;
      }
      if(strcmp(memory[k].id, id) == 0){
        found = 1;
        break;
      }
      k++;
    }

    if (found == 1){
      memory[k].lastAccess = time;
      memory[k].reference++;
      if(rw == 'W'){
        memory[k].altered++;
      }
    }
    else{
      Page* page = malloc(sizeof(Page));
      page->lastAccess = 0;
      page->reference = 1;
      if (rw == 'W'){
        page->altered = 1;
      }
      else{
        page->altered = 0;
      }

      // get page id
      strcpy(page->id, id);

      if(i < memorySize/pageSize){
        memory[i] = *page;
        i++;
      }
      else{
        printf("Memory full\n");
        break;
      }
      free(page);
    }

    time++;
  }

  //print memory
  int j = 0;

  while(j < memorySize/pageSize){
    printf("Page id: %s\n", memory[j].id);
    printf("Last access: %d\n", memory[j].lastAccess);
    printf("Reference: %d\n", memory[j].reference);
    printf("Altered: %d\n", memory[j].altered);
    printf("\n");
    j++;
  }

  free(memory);

  fclose(file);

  return 0;
}