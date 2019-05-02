#ifndef EX5_MTM_ARRAY_H
#define EX5_MTM_ARRAY_H

#include <iostream>
#include <utility>
#include <stdexcept>
#include <cassert>
#include <vector>

using std::ostream;
using std::istream;
using std::endl;
using std::cout;
using std::vector;

template <class T, int SIZE>
class Array
{
    T* data[SIZE];
    int size;
public:
    class mException{};
    explicit Array():size(0){}
    Array(const Array<T,SIZE>& a):data(),size(a.size){
        for(int i=0;i<size;i++){
            data[i]=a.data[i];
        }
    }
    ~Array(){}
    int get_size() const{
        return size;
    }
    Array& operator=(const Array& a){
        if(this== &a){
            return *this;
        }
        size = a.size;
        for(int i=0;i<size;i++)
        {
            data[i] = a.data[i];
        }
        return *this;
    }
    T& operator[](int index)throw (mException){
        if(index<0 || index>=size) throw std::out_of_range("Index out of range!");
        return data[index];
    }
    const T& operator[](int index) const throw(mException){
        if (index<0 || index>=size) throw std::out_of_range("Index out of range!");
        return data[index];
    }
    const Array operator++(int) //postfix
    {
        Array temp = *this;
        ++*this;
        return temp;
    }
    Array& operator++()
    {
        for(int i=0;i<size;i++)
            data[i]++;
        return *this;
    }
    friend ostream& operator<<(ostream& os, const Array& a)
    {
        os << "(";
        for(int i=0;i<a.size;i++)
        {
            os << a.data[i];
            if(i<a.size-1)
                os <<  " ";
        }
        os << ")" << endl;
        return os;
    }
    friend istream& operator>>(istream& is,Array& a)
    {
        cout << "enter " << a.size << " digits to fill the array" << endl;
        for(int i=0;i<a.size;i++) is >> a.data[i];
        return is;
    }

    friend class iterator;
    friend class const_iterator;
    // TODO: Need to complete interface of iterator classes (x++,++x, ->, operator==, operator!=), as well
    // as that of array itself (operator [], operator [] const, ctor, copy ctor, dtor, operator=),
    // and implement everything.


    class iterator
    {
        const Array* array;
        int index;
        friend class Array;
    public:
        iterator(Array<T,SIZE>* array,int size): array(array),index(size) { }
         T& operator*() {
            return array->data[index];
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++*this;
            return temp;
        }
        iterator& operator++() {
            ++index;
            return *this;
        }
        T& operator->()  {
            assert(index >= 0 && index < array->get_size());
            return (array->data[index]);
        }
        friend bool operator==(const iterator& it1,const iterator& it2) {
            return it1.index == it2.index;
        }
        friend bool operator!=(const iterator& it1, const iterator& it2) {
            return !(it1 == it2);
        }
    };

    iterator begin(){
        return iterator(this,0);
    }
    iterator end(){
        return iterator(this,size);
    }

    class const_iterator
    {
        const Array* array;
        int index;
        friend class Array;
    public:
        const_iterator(const Array* array,int size): array(array),index(size) { }
        const T& operator*() const {
            return array->data[index];
        }
        const const_iterator operator++(int) {
            const_iterator temp = *this;
            ++*this;
            return temp;
        }
        const const_iterator& operator++() {
            ++index;
            return  *this;
        }
        const T& operator->() const  {
            assert(index >= 0 && index < array->get_size());
            return (array->data[index]);
        }
        friend bool operator==(const const_iterator& it1,const const_iterator& it2) {
            return it1.index == it2.index;
        }
        friend bool operator!=(const const_iterator& it1, const const_iterator& it2) {
            return !(it1 == it2);
        }
    };

    const_iterator begin() const{
        return const_iterator(this,0);
    }
    const_iterator end() const{
        return const_iterator(this,size);
    }

};
























































//
//    class iterator;
//    iterator begin();
//    iterator end();
//    class const_iterator;
//    const_iterator con_begin() const;
//    const_iterator con_end() const;
//};
//    template<class T, int SIZE>
//    class Array<T,SIZE>::iterator {
//        const Array<T,SIZE>* array;
//        int index;
//        iterator(const Array<T,SIZE>* array,int index):array(array),index(index){}
//        friend class Array<T,SIZE>;
//    public:
//        T& begin();
//        T& end();
//        T& operator*() const;
//        iterator& operator++();
//        iterator operator++(int);
//        bool operator==(const iterator& iterator1) const;
//        bool operator!=(const iterator& iterator1) const;
//        iterator(const iterator&) = default;
//        iterator& operator=(const iterator&) = default;
//
//    };
//    template<class T, int SIZE>
//    class Array<T,SIZE>::const_iterator {
//        const Array<T,SIZE>* array;
//        int index;
//        const_iterator(const Array<T,SIZE>* array,int index):array(array),index(index){}
//        friend class Array<T,SIZE>;
//    public:
//        const T& con_begin();
//        const T& con_end();
//        T const& operator*() const;
//        const_iterator& operator++();
//        const_iterator operator++(int);
//        bool operator==(const const_iterator& const_iterator1) const;
//        bool operator!=(const const_iterator& const_iterator1) const;
//        const_iterator(const const_iterator&) = default;
//        const_iterator& operator=(const const_iterator&) = default;
//    };
//    template<class T, int SIZE>
//     Array<T,SIZE>::iterator::iterator(const Array<T,SIZE>* array,int index):array(array),index(index){}
//    template<class T, int SIZE>
//    T& Array<T,SIZE>::iterator::operator*() const{
//        assert (index>=0 && index < Array->get_size);
//        return Array->array[index];
//    }
//    template<class T, int SIZE>
//    class Array<T,SIZE>::iterator& Array<T,SIZE>::iterator::operator++(){
//        ++index;
//        return *this;
//    }
//    template<class T, int SIZE>
//    class Array<T,SIZE>::iterator Array<T,SIZE>::iterator::operator++(int){
//        iterator result=*this;
//        ++*this;
//        return result;
//    }
//    template<class T, int SIZE>
//    bool Array<T,SIZE>::iterator::operator==(const iterator& iterator1) const{
//        assert(Array==iterator1.array);
//        return index==iterator1.index;
//    }
//    template<class T, int SIZE>
//    bool Array<T,SIZE>::iterator::operator!=(const iterator& iterator1) const{
//        return !(*this == iterator1);
//    }
//
//    template<class T, int SIZE>
//    class Array<T,SIZE>::iterator Array<T,SIZE>::end(){
//        return iterator(this,size);
//    }
//
//    template<class T, int SIZE>
//    class Array<T,SIZE>::iterator Array<T,SIZE>::begin(){
//        return iterator(this,0);
//    }
//
//
//
//    template<class T, int SIZE>
//    Array<T,SIZE>::const_iterator::const_iterator(const Array<T,SIZE>* array,int index):array(array),index(index){}
//    template<class T, int SIZE>
//    T& Array<T,SIZE>::const_iterator::operator*() const{
//        assert (index>=0 && index < Array->get_size);
//        return Array->array[index];
//    };
//    template<class T, int SIZE>
//    class Array<T,SIZE>::const_iterator& Array<T,SIZE>::const_iterator::operator++(){
//        ++index;
//        return *this;
//    }
//    template<class T, int SIZE>
//    class Array<T,SIZE>::const_iterator Array<T,SIZE>::const_iterator::operator++(int){
//        const_iterator result=*this;
//        ++*this;
//        return result;
//    }
//    template<class T, int SIZE>
//    bool Array<T,SIZE>::const_iterator::operator==(const const_iterator& const_iterator1) const{
//        assert(Array==const_iterator1.array);
//        return index==const_iterator1.index;
//    };
//    template<class T, int SIZE>
//    bool Array<T,SIZE>::const_iterator::operator!=(const const_iterator& const_iterator1) const{
//        return !(*this == const_iterator1);
//    };
//
//    template<class T, int SIZE>
//    class Array<T,SIZE>::const_iterator Array<T,SIZE>::con_begin() const{
//        return const_iterator(this,0);
//    }
//
//    template<class T, int SIZE>
//    class Array<T,SIZE>::const_iterator Array<T,SIZE>::con_end() const{
//        return const_iterator(this,size);

#endif //EX5_MTM_ARRAY_H

