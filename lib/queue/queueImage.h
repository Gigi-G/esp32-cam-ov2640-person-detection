#ifndef QUEUE
#define QUEUE

#include <nodeImage.h>

template<typename T>
class QueueImage {
    private:
        int size;
        Node<T>* head;
        Node<T>* tail;

    public:
        QueueImage();
        QueueImage<T>* push(T val);
        Node<T>* pop();
        bool isEmpty();
};

#include <queueImage.tpp>

#endif