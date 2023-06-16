#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Page{
  int id;
  int altered;
  int lastAccess; // LRU
  int referenceBit; // 2a
  int entry;  // FIFO
} Page;

// Global variables for arguments
char* algorithm;
int (*algorithmFunction)(Page[], int);
char* inputFile;
int pageSize;
int memorySize;
int debug;
int secondChanceIndex = 0;

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
  int index = -1;

  while(index == -1){
    if (memory[secondChanceIndex].referenceBit == 0){
      index = secondChanceIndex;
      break;
    }
    else{
      memory[secondChanceIndex].referenceBit = 0;
    }
    secondChanceIndex++;

    if (secondChanceIndex == numberOfPages){
      secondChanceIndex = 0;
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
  else if (strcmp(algorithm, "2a") == 0){
    algorithmFunction = &secondChance;
  }
}

int main(int argc, char* argv[]){
  char debugString[100];
  srand(42);

  // Parse arguments
  parseArgs(argc, argv);

  // Initialize memory
  int numberOfPages = memorySize/pageSize;
  Page* memory = malloc(sizeof(Page) * numberOfPages);

  for (int i = 0; i < numberOfPages; i++){
    memory[i].id = -1;
    memory[i].altered = 0;
    memory[i].lastAccess = 0;
    memory[i].referenceBit = 0;
    memory[i].entry = 0;
  }

  int tmp = pageSize * 1024;
  int s = 0;

  while(tmp > 1){
    tmp = tmp >> 1;
    s++;
  }

  int addr;
  char rw;
  FILE* file = fopen(inputFile, "r");
  int i = 0;
  int time = 0;

  int memoryAccesses = 0;
  int pageFaults = 0;
  int diskWrites = 0;

  while(fscanf(file, "%x %c", &addr, &rw) != EOF){
    // check if page is in memory
    int k = 0;
    int found = 0;
    int id = addr >> s;
    
    memoryAccesses++;
  
    while(k < numberOfPages){
      if (memory[k].id == -1){
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

      if(rw == 'W'){
        memory[k].altered = 1;
      }
      memory[k].referenceBit = 1;

      if (debug == 1){
        sprintf(debugString, "Pagina %d foi encontrada na memoria (%c) no tempo %d\n", id, rw, time);
      }
    }
    // add page to memory
    else{
      Page* page = malloc(sizeof(Page));
      page->id = id;
      page->lastAccess = time;
      page->referenceBit = 1;
      page->entry = time;

      pageFaults++;

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

        if (debug == 1){
          sprintf(debugString, "Pagina %d foi adicionada a memoria (%c) no tempo %d\n", id, rw, time);
        }
      }
      // replace existing page if there is no space
      else{
        int index = algorithmFunction(memory, numberOfPages);
        if (memory[index].altered == 1) {
          diskWrites++;
        }
        int oldId = memory[index].id;
        memory[index] = *page;

        if (debug == 1){
          sprintf(debugString, "Pagina %d foi adicionada a memoria (%c) no tempo %d, substituindo a pagina %d\n", id, rw, time, oldId);
        }
      }
      free(page);
    }
    
    if (debug == 1){
      printf("%s", debugString);
    }

    time++;
  }
  
  // print variables
  printf("Executando o simulador...\n");  
  printf("Arquivo de entrada: %s\n", inputFile);
  printf("Tamanho da memoria: %d KB\n", memorySize);
  printf("Tamanho das paginas: %d KB\n", pageSize);
  printf("Tecnica de reposicao: %s\n", algorithm);
  printf("Numero de acessos a memoria: %d\n", memoryAccesses);
  printf("Paginas lidas: %d\n", pageFaults);
  printf("Paginas escritas: %d\n", diskWrites);

  free(memory);

  fclose(file);

  return 0;
}