#ifndef QUEUE
#define QUEUE

#include <Arduino.h>
#include <node.h>

template<typename T>
class Queue {
    private:
        int size;
        Node<T>* head;
        Node<T>* tail;
        static Queue<T>* q;
        Queue();
        Node<T>* _pop();
        static SemaphoreHandle_t sem;

    public:
        static Queue<T>* getInstance();
        Queue<T>* push(T val);
        Node<T>* pop();
        bool isEmpty();
        void clear();
};

#include <queue.tpp>

#endif