/*
 * Header file for universally functional Cache class.
 * Cache stores parameters like hit and miss data, as well
 * as parameters for input taken in by the command line arguments.
 * CSF Assignment 4
 * David Yang
 * dyang41@jhu.edu
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

/*
 * Block class used for storing data points in the cache.
 * 
 * Private variables:
 *    valid: whether tag is valid or not
 *    tag: tag to identify position in memory
 *    offset: part of memory address read in, albeit unused.
 *    dirty: used for write-back cache when evicting.
 *    priority: order in which to evict 
 */
class Block {
 public:
  bool valid;
  int tag;
  int offset;
  bool dirty;
  int priority;
  /*
   * Empty constructor function for Block, represents empty cache slot.
   */
  Block() {
    tag = 0;
    offset = 0;
    priority = 0;
    valid = false;
    dirty = false;
  }
  /*
   * Constructor function for Block, for a valid cache slot. Tag is valid.
   *
   * Parameters:
   *   tagVal: binary tag value in integer form
   *   offsetVal: offset value in int form
   */
  Block(int tagVal, int offsetVal){
    tag = tagVal;
    offset = offsetVal;
    priority = 0;
    valid = true;
    dirty = false;
  }
};

class Cache {
 public:
  /*
   * Constructor function for class. Takes in run parameters, sets all the hit/miss/cycle data
   * to zero, and resizes the vector array.
   *
   * Parameters:
   *   Matching input with command line arguments, in the specified order.
   */
  Cache(int nSets, int nBlocks, int nBytes, bool alloc, bool thru, bool lrufifo) {
    sets = nSets;
    blocks = nBlocks;
    bytes = nBytes;
    writeAllocate = alloc;
    writeThrough = thru;
    lru = lrufifo;

    loadHits = 0;
    loadMisses = 0;
    storeHits = 0;
    storeMisses = 0;
    cycles = 0;

    //resizes cache 2d array, to contain sets containing the right number of blocks.
    cache.resize(sets);
    for(unsigned int i = 0; i < cache.size(); i++) {
      cache.at(i).resize(blocks);
    }
  }

  /*
   * Reads in a preprocessed form of a line of the trace file, and modifies
   * the data a bit more to be inserted into the array.
   *
   * Parameters:
   *    load: true if the character was l, false if it was s.
   *    data: int form of the 32 bit address taken in.
   */
  void addData(bool load, long data);

  /*
   * Takes block in and finds spot for cache to hold block.
   *
   * Parameters:
   *    load: true if the character was l, false if it was s.
   *    index: index value for cache,  to find relevant set.
   *    block: Block to add to cache.
   */
  void addToCache(bool load, int index, Block block);

  /*
   * Called when the set at index is filled, it will evict the block with
   * highest priority and fill it with block.
   * 
   * Parameters:
   *    load: true if the character was l, false if it was s.
   *    index: index value for cache,  to find relevant set.
   *    block: Block to add to cache.
   */
  void evict(bool load, int index, Block block);

  /*
   * Prints out the end result of the miss/hit/cycles data.
   */
  void printResults();

  /*
   * Input parameters from command line.
   */
  int sets, blocks, bytes;
  //if the booleans are false, they are equal to
  //no write allocate, write back, and fifo, respectively
  bool writeAllocate, writeThrough, lru;

 private:
  /*
   * Function to do everything needed for both a load/store hit.
   *
   * Parameters:
   *    load: if load, true, if false, store.
   */
  void hit(bool load);

  /*
   * Function to do everything needed for both a load/store miss.
   *
   * Parameters:
   *    load: if load, true, if false, store.
   */
  void miss(bool load);

  /*
   * Creates block. Extra steps are taken depending on
   * input parameters if it is a store.
   *
   * Parameters:
   *    load: if load, true, if false, store.
   *    tag: tag int value.
   *    offset: offset int value.
   */
  Block createBlock(bool load, int tag, int offset);

  //vector containing vector, used as a 2d array.
  std::vector<std::vector<Block>> cache;
  
  //hit/miss/cycle data
  int loadHits;
  int loadMisses;
  int storeHits;
  int storeMisses;
  int cycles;

};
