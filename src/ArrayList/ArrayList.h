#pragma once

template<class T, int N>
class ArrayList{
public:
    ArrayList();
    ~ArrayList();

    void insert(int pos, T);
    void push_back(T val);
    void pop_back();
    T at(int pos);
    void clear();
private:
    T* data;
    int size;
    int length;
};
