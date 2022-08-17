#ifndef NODE
#define NODE

template<typename T>
class Node {
    public:
        Node<T>* next;
        Node<T>* prev;
        T val;
};

#endif