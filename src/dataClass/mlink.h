#pragma once

template <typename T>
struct Node
{
public:
    T *obj;
    int Key;
    Node *next;
    Node()
    {
        this->Key = 0;
        this->obj = nullptr;
        this->next = nullptr;
    }
    Node(T *obj)
    {
        this->Key = 0;
        this->obj = obj;
        this->next = nullptr;
    }
};

template <typename T>
class mlink
{
private:
    Node<T> *list;

public:
    mlink();
    ~mlink();
    Node<T> *TheHead();
    Node<T> *AddNode(T *node, int key); // 增加节点
    void OutLine(Node<T> *p);           // 释放链
};

template <class T>
mlink<T>::mlink()
{
    this->list = nullptr;
}

template <class T>
mlink<T>::~mlink()
{
    OutLine(this->list);
    return;
}

template <class T>
Node<T> *mlink<T>::TheHead()
{
    return this->list;
}

template <class T>
void mlink<T>::OutLine(Node<T> *p) // 释放链
{
    if (p == nullptr)
        return;
    OutLine(p->next);
    delete p;
    return;
}

template <class T>
Node<T> *mlink<T>::AddNode(T *obj, int key) // 增加节点
{
    Node<T> *last = this->list;
    if (this->list == nullptr) // 空链表
    {
        this->list = new Node<T>;
        last = this->list;
    }
    else
    {
        Node<T> *p = this->list;
        for (p; p != nullptr; p = p->next)
        {
            if (p->Key > key)
                break;
            last = p;
        }
        if (nullptr == p) // 末尾加
        {
            last->next = new Node<T>;
            last = last->next;
        }
        else if (last == this->list) // 头加
        {
            this->list = new Node<T>; // 新的
            this->list->next = last;  // 连接
            last = this->list;        // 更新last
        }
        else
        {
            last->next = new Node<T>;
            last = last->next;
            last->next = p;
        }
    }
    if (last == nullptr)
        return nullptr;
    else
    {
        last->Key = key; // 添加key
        last->obj = obj;
    }

    return last;
}