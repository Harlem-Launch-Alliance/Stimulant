/*****************************************************************************
 * This file is for the RingQueue class and definitions
 * 
 ****************************************************************************/
#include "settings.h"

#ifndef RINGQUEUE_H
#define RINGQUEUE_H

/**
 * @brief A ring queue of constant size
 * 
 * Intended use for caching data while the SD card is not available
 * 
 * This structure operates like a standard queue. Data is added to the back and removed from the front.
 * 
 * @tparam T datatype to be stored
 */
template<typename T>
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
 * @tparam T datatype to be stored
 */
template<typename T>
RingQueue<T>::RingQueue(){//each new queue is initialized with 0 data
  head = 0;
  length = 0;
}

/**
 * @brief Check if RingQueue is empty
 * 
 * @tparam T type of data stored
 * @return true RingQueue is empty
 * @return false RingQueue is NOT empty
 */
template<typename T>
bool RingQueue<T>::isEmpty(){
  return length == 0;//if length is 0 it's empty otherwise it isn't
}

/**
 * @brief add one element to the data queue
 * 
 * @tparam T type of data stored
 * @param data value of data to be queued
 */
template<typename T>
void RingQueue<T>::enqueue(T data){
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
 * @warning this function should not be called while the queue is empty
 * 
 * @tparam T type of data
 * @return T data at front of queue
 */
template<typename T>
T RingQueue<T>::peek(){//unintended behaviour if called when queue is empty
  return dataArray[head];
}

/**
 * @brief delete element from front of queue
 * 
 * @tparam T type of data
 */
template<typename T>
void RingQueue<T>::dequeue(){
  if(length == 0)
    return;
  length--;
  head++;
}

#endif
