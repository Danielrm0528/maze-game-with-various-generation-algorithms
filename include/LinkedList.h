#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdexcept>
#include <iostream>

#include "Node.h"
#include "List.h"

using namespace std;

template <typename E>
class LinkedList : public List<E>{
private:
    Node<E> *head;
    Node<E> *tail;
    Node<E> *current;
    int size;
public:
    LinkedList<E>() {
        head = tail = current = new Node<E>();
        size = 0;
    }
    ~LinkedList<E>() {
        clear();
        delete head;
    }
    void insert(E element) {
        current->next = new Node<E>(element,current->next);
        if (current == tail)
            tail = current->next;
        size++;
    }
    void append(E element) {
        tail = tail->next = new Node<E>(element);
        size++;
    }
    E remove() {
        if(size == 0)
            throw runtime_error("List is empty");
        if(current == tail)
            throw runtime_error("No current element");
        Node<E> *temp = current->next;
        E res = temp->element;
        current->next = temp->next;
        delete temp;
        if (current->next == nullptr)
            tail = current;
        size--;
        return res;
    }
    void clear() {
        while(head->next != nullptr) {
            current = head->next;
            head->next = current->next;
            delete current;
        }
        current = tail = head;
        size = 0;
    }
    E getElement() {
        if(size == 0)
            throw runtime_error("List is empty");
        if(current == tail)
            throw runtime_error("No current element");
        return current->next->element;
    }
    void goToStart() {
        current = head;
    }
    void goToEnd() {
        current = tail;
    }
    void goToPos(int pos) {
        if(pos<0 || pos > size)
            throw runtime_error("index out of range");
        current = head;
        for (int i = 0; i<pos; i++) {
            current = current->next;
        }
    }
    void next() {
        if (current->next != nullptr)
            current = current->next;
    }
    void previous() {
        if (current != head) {
            Node<E> *temp = head;
            while(temp->next != current)
                temp = temp->next;
            current = temp;
        }
    }
    bool atStart() {
        return current == head;
    }
    bool atEnd() {
        return current == tail;
    }
    int getPos() {
        int pos = 0;
        Node<E> *temp = head;
        while(temp != current) {
            temp = temp->next;
            pos++;
        }
        return pos;
    }
    int getSize() {
        return size;
    }
    void print() {
        Node<E> *temp = head->next;
        cout << "[ ";
        while (temp != nullptr) {
            cout << temp->element << " ";
            temp = temp->next;
        }
        cout << "]";
    }

    bool contains(E element) {
        Node<E> *temp = head->next;
        while (temp != nullptr) {
            if (temp->element == element) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }


};

#endif // LINKEDLIST_H
