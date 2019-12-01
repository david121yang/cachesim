#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "Cache.h"

using namespace std;

/*
 * Takes a 32 byte number, and splits it if it were in binary. Used
 * for creating the tag, index and offset values. Going from right to left
 * returns a substring from rightByte-th digit to left.
 * 
 * Parameters:
 *    num: 32 byte address to be split.
 *    leftByte: leftmost byte, not included in substring
 *    rightByte: rightmost byte, included in substring
 */
int binarySubstring(long num, int leftByte, int rightByte) {
  for(int i = 0; i < rightByte; i++) {
    num /= 2;
  }
  int modNum = 1;
  for(int i = 0; i < leftByte - rightByte; i++) {
    modNum *= 2;
  }
  num %= modNum;
  return num;
}

void Cache::hit(bool load) {
  if(load) {
    loadHits++;
  }
  else {
    storeHits++;
  }
  cycles++;
}

void Cache::miss(bool load) {
  if(load) {
    loadMisses++;
  }
  else {
    storeMisses++;
  }
  //retrieve value from main memory
  cycles += 100 * bytes/4;
}

Block Cache::createBlock(bool load, int tag, int offset){
  Block b = Block(tag, offset);

  //For a store, no matter what, the function writeThrough or writeBack will
  //be carried out. So, I do them in advance, instead of inside the function.
  if(!load) {
    if(writeThrough) {
      cycles += 100;
    }
    else {
      b.dirty = true;
    }
  }
  return b;
}

int count = 0;

void Cache::addData(bool load, long data) {
  int indexBytes = log2(sets);
  int blockBytes = log2(bytes);
  int index = binarySubstring(data, blockBytes + indexBytes, blockBytes);
  int tag = binarySubstring(data, 32, blockBytes + indexBytes);
  int offset = binarySubstring(data, blockBytes, 0);
  Block block = createBlock(load, tag, offset);

  addToCache(load, index, block);
}

void Cache::addToCache(bool load, int index, Block block) {
  //index to store in cache.
  int setPos = -1;
  for (int i = 0; i < blocks; i++) {
    if(!cache[index][i].valid) {
      //miss
      miss(load);
      setPos = i;
      if(load || writeAllocate) {
	for(int j = 0; j < blocks; j++) {
	  cache[index][j].priority++;
	}
	cache[index][i] = block;
	cycles++;
      }
      break;
    }
  
    else if(cache[index][i].tag == block.tag) {
      //hit
      hit(load);
      setPos = i;
      if(lru) {
	int prevPriority = cache[index][i].priority;
	for(int j = 0; j < blocks; j++) {
	  if(cache[index][j].priority < prevPriority) {
	    cache[index][j].priority++;
	  }
	}
	cache[index][i] = block;
      }
      break;
    }
  }
  if(setPos == -1) {
    //failed to find a spot. Miss, and evict.
    evict(load, index, block);
  }
}

void Cache::evict(bool load, int index, Block block) {
  miss(load);
  if(load || writeAllocate) {
    for(int i = 0; i < blocks; i++) {
      if(cache[index][i].priority == blocks - 1) {
	if(cache[index][i].dirty) {
	  cycles += 100 * bytes / 4;
	}
	for(int j = 0; j < blocks; j++) {
	  cache[index][j].priority++;
	}
	cache[index][i] = block;
	cycles++;
	break;
      }
    }
  }
}


void Cache::printResults() {
  cout << "Total loads: " << (loadMisses+loadHits) << endl
       << "Total stores: " << (storeMisses+storeHits) << endl
       << "Load hits: " << loadHits << endl
       << "Load misses: " << loadMisses << endl
       << "Store hits: " << storeHits << endl
       << "Store misses: " << storeMisses << endl
       << "Total cycles: " << cycles << endl;
}
