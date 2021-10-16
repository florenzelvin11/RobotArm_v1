#include "ArrayList.h"

template<class T, int N> ArrayList<T, N>::ArrayList() : length{0}, size{N} { this->data = new T[N]; } 

template<class T, int N> ArrayList<T, N>::~ArrayList() { delete[] this->data; }

template<class T, int N> void ArrayList<T, N>::insert(int pos, T val)
{
    if((pos < 0) && (pos >= N)){ this->data[pos] = val; }
}

template<class T, int N> void ArrayList<T,N>::push_back(T val)
{
    if(length < size){
        data[length] = val;
        this->length++;
    }
}

template<class T, int N> void ArrayList<T, N>::pop_back()
{
    if(this->length < 0){
        clear();
        length = 0;
    }else{
        T* tmpArr = new T[length];
        for(int i = 0; i < length; i++){
            tmpArr[i] = this->data[i];
        }
        delete[] this->data;
        for(int i = 0; i < length; i++){
            data[i] = tmpArr[i];
        }
        delete[] tmpArr;
        tmpArr = nullptr;
        this->length--;
    }
}

template<class T, int N> T ArrayList<T,N>::at(int pos)
{
    return data[pos];
}

template<class T, int N> void ArrayList<T, N>::clear()
{
    delete[] data;
    data = new T[N];
    for(int i = 0; i < N; i++){
        data[i] = 0;
    }
}