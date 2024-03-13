#ifndef VECTOR_DEFINITIONS_H
#define VECTOR_DEFINITIONS_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

#include <iostream>
#include <new>

template <typename T>
T &vector<T>::getValAtIdx(int idx)
{
    if (idx + 1 > size_ || idx < 0)
    {
        std::cout << "index " << idx << " > size" << size_ << std::endl;
        if (size_ <= 0)
            return ptr[0];
        idx = 0;
    }

    idx += head_;
    if (idx >= max_size_)
        idx -= max_size_;

    return ptr[idx];
}

template <typename T>
const T &vector<T>::getValAtIdx(int idx) const
{
    if (idx + 1 > size_)
    {
        std::cout << "index " << idx << " > size" << size_ << std::endl;
        if (size_ <= 0)
            return ptr[0];
        idx = 0;
    }

    idx += head_;
    if (idx >= max_size_)
        idx -= max_size_;

    return ptr[idx];
}

template <typename T>
vector<T>::vector()
{
    max_size_ = size_ = 0;
    head_ = tail_ = -1;
    ptr = NULL;
}

template <typename T>
vector<T>::vector(int mxs) : max_size_(mxs)
{
    size_ = 0;
    head_ = tail_ = -1;
    ptr = new T[max_size_];
}

template <typename T>
vector<T>::vector(const vector &other) : max_size_(other.max_size_), size_(other.size_), head_(other.head_), tail_(other.tail_)
{
    ptr = new T[max_size_];
    std::copy(other.ptr, other.ptr + max_size_, ptr);
}

template <typename T>
vector<T>::vector(vector &&other) noexcept : max_size_(other.max_size_), size_(other.size_), head_(other.head_), tail_(other.tail_), ptr(other.ptr)
{
    other.ptr = nullptr;
    other.max_size_ = other.size_ = 0;
    other.head_ = other.tail_ = -1;
}

template <typename T>
void vector<T>::copy(vector<T> v)
{
    ptr = new T[v.max_size()];
    this->max_size_ = v.max_size();
    for (int i = 0; i < v.size(); i++)
    {
        push(v[i]);
    }
}

template <typename T>
void vector<T>::push(T value)
{
    if (size_ == 0)
    {
        head_ = tail_ = 0;
        ptr[tail_] = value;
        size_ = 1;
        return;
    }
    if (size_ == max_size_)
        pop();

    tail_++;
    if (tail_ == max_size_)
        tail_ = 0;
    ptr[tail_] = value;
    size_++;
}

template <typename T>
void vector<T>::pop()
{
    if (size_ == 0)
        return;
    if (size_ == 1)
    {
        head_ = tail_ = -1;
        size_ = 0;
        return;
    }

    head_++;
    if (head_ == max_size_)
        head_ = 0;
    size_--;
}

template <typename T>
void vector<T>::fill(T value)
{
    while (size_ > 0)
    {
        pop();
    }
    while (size_ < max_size_)
    {
        push(value);
    }
}

template <typename T>
void vector<T>::print()
{
    if (size_ == 0)
    {
        std::cout << "vector empty" << std::endl;
        return;
    }

    for (int i = 0; i < size_; i++)
    {
        std::cout << getValAtIdx(i) << " ";
    }

    std::cout << std::endl;
}

template <typename T>
const int vector<T>::size() const
{
    return size_;
}

template <typename T>
const int vector<T>::max_size() const
{
    return max_size_;
}
template <typename T>
T vector<T>::operator*(const vector<T> &u) const
{
    T res = 0;
    int len = size_;
    if (u.size() < size_)
        len = u.size();
    for (int i = 0; i < len; i++)
    {
        res += getValAtIdx(i) * u[i];
    }
    return res;
}

template <typename T>
vector<T> vector<T>::operator*(const T &s) const
{
    vector<T> temp(max_size_);

    for (int i = 0; i < size_; i++)
    {
        temp.push(getValAtIdx(i) * s);
    }

    return temp;
}

template <typename T>
vector<T> vector<T>::operator+(const vector<T> &u) const
{
    vector<T> res(max_size_);
    int len = size_;
    if (u.size() < size_)
        len = u.size();
    for (int i = 0; i < len; i++)
    {
        res.push(getValAtIdx(i) + u[i]);
    }
    return res;
}

template <typename T>
vector<T> vector<T>::operator+(const T &s) const
{
    vector<T> temp(max_size_);

    for (int i = 0; i < size_; i++)
    {
        temp.push(getValAtIdx(i) + s);
    }

    return temp;
}

template <typename T>
bool vector<T>::operator>(const T s) const
{
    for (int i = 0; i < size_; i++)
    {
        if (getValAtIdx(i) > s)
            return true;
    }
    return false;
}

template <typename T>
bool vector<T>::operator<(const T s) const
{
    for (int i = 0; i < size_; i++)
    {
        if (getValAtIdx(i) < s)
            return true;
    }
    return false;
}

template <typename T>
vector<T> &vector<T>::operator=(const vector<T> &other)
{
    if (this != &other)
    {
        delete[] ptr;
        max_size_ = other.max_size_;
        size_ = other.size_;
        head_ = other.head_;
        tail_ = other.tail_;
        ptr = new T[max_size_];
        std::copy(other.ptr, other.ptr + max_size_, ptr);
    }
    return *this;
}

template <typename T>
const T &vector<T>::operator[](int idx) const
{
    return getValAtIdx(idx);
}

template <typename T>
T &vector<T>::operator[](int idx)
{
    return getValAtIdx(idx);
}

// template <typename T>
// void *vector<T>::operator new(size_t size)
// {
//     if (size == 0)
//         size++;
//     if (void *p = std::malloc(size))
//         return p;
//     // throw std::bad_alloc{};
//     return NULL;
// }

template <typename T>
void vector<T>::operator delete(void *p)
{
    free(p);
}

// template <typename T>
// void *vector<T>::operator new[](size_t size)
// {
//     if (size == 0)
//         size++;
//     if (void *p = std::malloc(size))
//         return p;
//     // throw std::bad_alloc{};
//     return NULL;
// }

template <typename T>
void vector<T>::operator delete[](void *p)
{
    std::free(p);
}

template <typename T>
vector<T>::~vector()
{
    delete[] ptr;
}

#endif