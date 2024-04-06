/*****************************************************************************
 * This file is for the RingQueue class and definitions
 * 
 ****************************************************************************/
#pragma once

#include "config/config.h"

/**
 * @brief A ring queue of constant size
 * 
 * @note Intended use is for caching data while the SD card is not available
 * 
 * @note This structure operates like a standard queue. Data is added to the back and removed from the front.
 * 
 * @tparam T Datatype to be stored
 */
template<class T, int RING_QUEUE_LENGTH>
class RingQueue{
public:
  RingQueue();
  void enqueue(T data);
  T peek();
  void dequeue();
  bool isEmpty();
private:
  uint32_t head;
  uint32_t length;
  T dataArray[RING_QUEUE_LENGTH];
};

/**
 * @brief Construct a new Ring Queue< T>:: Ring Queue object
 * 
 * @tparam T Datatype to be stored
 */
template<class T, int RING_QUEUE_LENGTH>
RingQueue<T, RING_QUEUE_LENGTH>::RingQueue(){//each new queue is initialized with 0 data
  head = 0;
  length = 0;
}

/**
 * @brief Check if RingQueue is empty
 * 
 * @tparam T Type of data stored
 * @return true RingQueue is empty
 * @return false RingQueue is NOT empty
 */
template<class T, int RING_QUEUE_LENGTH>
bool RingQueue<T, RING_QUEUE_LENGTH>::isEmpty(){
  return length == 0;//if length is 0 it's empty otherwise it isn't
}

/**
 * @brief Add one element to the data queue
 * 
 * @tparam T Type of data stored
 * @param data Value of data to be queued
 */
template<class T, int RING_QUEUE_LENGTH>
void RingQueue<T, RING_QUEUE_LENGTH>::enqueue(T data){
  if(length >= RING_QUEUE_LENGTH) //if the array is full just ignore
    return;
  int nextLocation = (head + length) % RING_QUEUE_LENGTH; 
  /*we use modulus to wrap around EX: [6, 7, 8, 9, NULL, NULL, NULL, 0, 1, 2, 3, 4, 5]
  in this case the "head" is 7, and the length is 10. (7 + 10) mod 13 == 4,
  so 4 is the next available location for data*/
  dataArray[nextLocation] = data;
  length++;
}
/**
 * @brief Peek at next data at the front of the queue
 * 
 * @warning This function should not be called while the queue is empty
 * 
 * @tparam T Type of data
 * @return T Data at front of queue
 */
template<class T, int RING_QUEUE_LENGTH>
T RingQueue<T, RING_QUEUE_LENGTH>::peek(){//unintended behaviour if called when queue is empty
  return dataArray[head];
}

/**
 * @brief Delete element from front of queue
 * 
 * @tparam T Type of data stored
 */
template<class T, int RING_QUEUE_LENGTH>
void RingQueue<T, RING_QUEUE_LENGTH>::dequeue(){
  if(length == 0)
    return;
  length--;
  head++;
}
