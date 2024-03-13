#ifndef VECTOR_H
#define VECTOR_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

// include other helpers if needed.

template <typename T>
class vector
{
private:
    int max_size_, size_, head_, tail_;
    T *ptr;
    T &getValAtIdx(int);
    const T &getValAtIdx(int) const;

public:
    // constructor
    vector();
    vector(int max_size);
    vector(const vector&);
    vector(vector&&) noexcept;
    void copy(vector<T> v);
    void push(T value);
    void pop();
    void fill(T value);
    void print();
    const int size() const;
    const int max_size() const;
    // void *operator new(size_t size);
    void operator delete(void *p);
    // void *operator new[](size_t size);
    void operator delete[](void *p);
    T operator*(const vector<T>& u) const;
    vector<T> operator*(const T& s) const;
    vector<T> operator+(const vector<T>& u) const;
    vector operator+(const T& s) const;
    bool operator>(const T s) const;
    bool operator<(const T s) const;
    vector<T>& operator=(const vector<T>&);
    const T &operator[](int idx) const;
    T &operator[](int idx);
    ~vector();
};

#include "./vectorDefinitions.h"

#endif