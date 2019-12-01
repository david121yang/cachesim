#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "Cache.h"

using namespace std;

/*
 * Verifies if number is a power of 2 for input.
 *
 * Parameters:
 *    num: number to verify.
 * Return:
 *    true if num is a power of 2, false otherwise.
 */
bool powerOf2(int num) {
  if (num == 1) {
    return true;
  }
  if(num % 2 == 0) {
    return powerOf2(num/2);
  }
  else {
    return false;
  }
}


int main(int argc, char *argv[]) {
  int sets, blocks, bytes;
  //if false, no write allocate, write back, and fifo
  bool writeAllocate, writeThrough, lru;
  if (argc != 7) {
    fprintf(stderr, "Invalid format\n");
    return 1;
  }
  sets = atoi(argv[1]);
  blocks = atoi(argv[2]);
  bytes = atoi(argv[3]);
  if(bytes < 4 || !powerOf2(sets) || !powerOf2(blocks) || !powerOf2(bytes)) {
    fprintf(stderr, "Invalid inputs\n");
    return 1;
  }

  if(strcmp(argv[4], "write-allocate")==0) {
    writeAllocate = true;
  }
  else if(strcmp(argv[4], "no-write-allocate")==0) {
    writeAllocate = false;
  }
  else {
    fprintf(stderr, "Invalid inputs\n");
    return 1;
  }
  if(strcmp(argv[5], "write-through")==0) {
    writeThrough = true;
  }
  else if(strcmp(argv[5], "write-back")==0) {
    writeThrough = false;
  }
  else {
    fprintf(stderr, "Invalid inputs\n");
    return 1;
  }
  if(!writeThrough && !writeAllocate) {
    fprintf(stderr, "Cannot not write allocate and write back\n");
    return 1;
  }
  
  if(strcmp(argv[6], "lru") == 0) {
    lru = true;
  }
  else if(strcmp(argv[6], "fifo") == 0) {
    lru = false;
  }
  else {
    fprintf(stderr, "Invalid inputs\n");
    return 1;
  }
  Cache c = Cache(sets, blocks, bytes, writeAllocate, writeThrough, lru);

  while (cin && !cin.eof()) {
    bool load;
    char loadStore;
    long data;
    int trashMan;
    string s1;
    cin >> loadStore >> std::hex >> data >> trashMan;
    if(loadStore == 'l') {
      load = true;
    }
    else load = false;
    if(cin.eof()) break;
    
    c.addData(load, data);    
  }
  
  c.printResults();
  return 0;
}
