template<typename T>
Queue<T>* Queue<T>::q = nullptr;

template<typename T>
SemaphoreHandle_t Queue<T>::sem = xSemaphoreCreateMutex();

template<typename T>
Queue<T>::Queue() {
    size = 0;
    head = new Node<T>();
    tail = new Node<T>();
    head->next = tail;
    tail->prev = head;
}

template<typename T>
Queue<T>* Queue<T>::getInstance() {
    xSemaphoreTake(sem, portMAX_DELAY);
    if(Queue<T>::q != nullptr) {
        xSemaphoreGive(sem);
        return Queue<T>::q;
    }
    Queue<T>::q = new Queue<T>();
    xSemaphoreGive(sem);
    return Queue<T>::q;
}

template<typename T>
Queue<T>* Queue<T>::push(T val) {
    xSemaphoreTake(sem, portMAX_DELAY);
    Node<T>* n = new Node<T>();
    n->prev = head;
    n->next = head->next;
    n->val = val;
    head->next->prev = n;
    head->next = n;
    size++;
    xSemaphoreGive(sem);
    return this;
}

template<typename T>
Node<T>* Queue<T>::pop() {
    xSemaphoreTake(sem, portMAX_DELAY);
    if(size <= 0) return nullptr;
    Node<T>* n = _pop();
    xSemaphoreGive(sem);
    return n;
}

template<typename T>
bool Queue<T>::isEmpty() {
    xSemaphoreTake(sem, portMAX_DELAY);
    bool r = size <= 0;
    xSemaphoreGive(sem);
    return r;
}

template<typename T>
void Queue<T>::clear() {
    xSemaphoreTake(sem, portMAX_DELAY);
    while(size > 0) {
        Node<T>* n = _pop();
        if(n->val) {
            delete n->val;
            delete n;
        }
    }
    xSemaphoreGive(sem);
}

template<typename T>
Node<T>* Queue<T>::_pop() {
    Node<T>* n = tail->prev;
    tail->prev = n->prev;
    n->prev->next = tail;
    size--;
    return n;
}