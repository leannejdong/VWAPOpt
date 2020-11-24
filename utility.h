//
// Created by leanne on 11/23/20.
//

#ifndef VWAPOPT_UTILITY_H
#define VWAPOPT_UTILITY_H

#include <vector>
#include <ostream>
#include <iostream>

using std::vector;
using std::ostream;
using std::cout;


template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &container)
{
    stream << "[ ";

    for (const T &item : container) {
        stream << item << " ";
    }

    stream << "]";
    return stream;
}

#endif//VWAPOPT_UTILITY_H
