#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Page{
  unsigned id;
  int lastAccess;
  int reference;
  int referenceBit;
  int altered;
  int entry;
} Page;

// Global variables for arguments
char* algorithm;
int (*algorithmFunction)(Page[], int);
char* inputFile;
int pageSize;
int memorySize;
int debug;

int lru(Page* memory, int numberOfPages){
  int i = 0;
  int min = memory[0].lastAccess;
  int index = 0;

  while(i < numberOfPages){
    if (memory[i].lastAccess < min){
      min = memory[i].lastAccess;
      index = i;
    }
    i++;
  }

  return index;
}

int fifo(Page* memory, int numberOfPages){
  int i = 0;
  int min = memory[0].entry;
  int index = 0;

  while(i < numberOfPages){
    if (memory[i].entry < min){
      min = memory[i].entry;
      index = i;
    }
    i++;
  }

  return index;
}

int secondChance(Page* memory, int numberOfPages){
  int i = 0;
  int index = -1;

  while(index == -1){
    if (memory[i].referenceBit == 0){
      index = i;
      break;
    }
    else{
      memory[i].referenceBit = 0;
    }
    i++;

    if (i == numberOfPages){
      i = 0;
    }
  }

  return index;
}

int randomAlg(Page* memory, int numberOfPages){
  int index = rand() % (numberOfPages);

  return index;
}

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

  algorithmFunction = NULL;

  if (strcmp(algorithm, "lru") == 0){
    algorithmFunction = &lru;
  }
  else if (strcmp(algorithm, "fifo") == 0){
    algorithmFunction = &fifo;
  }
  else if (strcmp(algorithm, "random") == 0){
    algorithmFunction = &randomAlg;
  }  
}

int main(int argc, char* argv[]){
  // Parse arguments
  parseArgs(argc, argv);

  // Print variables
  printf("Algorithm: %s\n", algorithm);
  printf("Input file: %s\n", inputFile);
  printf("Page size: %d KB\n", pageSize);
  printf("Memory size: %d KB\n", memorySize);
  printf("Debug: %d\n", debug);

  // Initialize memory
  int numberOfPages = memorySize/pageSize;
  Page* memory = malloc(sizeof(Page) * numberOfPages);

  int tmp = pageSize * 1024;
  int s = 0;

  while(tmp > 1){
    tmp = tmp >> 1;
    s++;
  }

  printf("S: %d\n", s);

  unsigned addr;
  char rw;
  FILE* file = fopen(inputFile, "r");
  int i = 0;
  int time = 0;

  while(fscanf(file, "%x %c", &addr, &rw) != EOF){
    // check if page is in memory
    int k = 0;
    int found = 0;
    unsigned id = addr >> s;

    // search for page
    while(k < numberOfPages){
      if (memory[k].id == 0){
        break;
      }
      if(memory[k].id == id){
        found = 1;
        break;
      }
      k++;
    }

    // update page
    if (found == 1){
      memory[k].lastAccess = time;
      memory[k].reference++;
      if(rw == 'W'){
        memory[k].altered++;
      }
    }
    // add page to memory
    else{
      Page* page = malloc(sizeof(Page));
      page->id = id;
      page->lastAccess = 0;
      page->reference = 1;
      page->referenceBit = 1;
      page->entry = time;
      if (rw == 'W'){
        page->altered = 1;
      }
      else{
        page->altered = 0;
      }

      // add page to memory if there is space
      if(i < numberOfPages){
        memory[i] = *page;
        i++;
      }
      // replace existing page if there is no space
      else{
        int index = algorithmFunction(memory, numberOfPages);
        memory[index] = *page;
      }
      free(page);
    }

    time++;
  }

  //print memory
  int j = 0;

  while(j < numberOfPages){
    printf("Page id: %x\n", memory[j].id);
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