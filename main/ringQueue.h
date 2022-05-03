/*****************************************************************************
 * This file is for the RingQueue class and definitions
 * 
 ****************************************************************************/
#include "settings.h"

#ifndef RINGQUEUE_H
#define RINGQUEUE_H

template<typename T>
class RingQueue{
public:
  RingQueue();
  void enqueue(T data);
  T peek();
  void dequeue();
  bool isEmpty();
private:
  unsigned int head;
  unsigned int length;
  T dataArray[RING_QUEUE_LENGTH];
};

template<typename T>
RingQueue<T>::RingQueue(){//each new queue is initialized with 0 data
  head = 0;
  length = 0;
}

template<typename T>
bool RingQueue<T>::isEmpty(){
  return length == 0;//if length is 0 it's empty otherwise it isn't
}

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

template<typename T>
T RingQueue<T>::peek(){//unintended behaviour if called when queue is empty
  return dataArray[head];
}

template<typename T>
void RingQueue<T>::dequeue(){
  if(length == 0)
    return;
  length--;
  head++;
}

#endif
