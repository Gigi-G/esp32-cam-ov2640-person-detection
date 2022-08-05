template<typename T>
QueueImage<T>::QueueImage() {
    size = 0;
    head = new Node<T>();
    tail = new Node<T>();
    head->next = tail;
    tail->prev = head;
}

template<typename T>
QueueImage<T>* QueueImage<T>::push(T val) {
    Node<T>* n = new Node<T>();
    n->prev = head;
    n->next = head->next;
    n->val = val;
    head->next->prev = n;
    head->next = n;
    size++;
    return this;
}

template<typename T>
Node<T>* QueueImage<T>::pop() {
    if(size <= 0) return nullptr;
    Node<T>* n = tail->prev;
    tail->prev = n->prev;
    n->prev->next = tail;
    size--;
    return n;
}

template<typename T>
bool QueueImage<T>::isEmpty() {
    return size <= 0;
}