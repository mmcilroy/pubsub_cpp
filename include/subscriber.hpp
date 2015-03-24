#pragma once

#include "pos.hpp"

template< typename T >
class publisher;

template< typename T >
class subscriber
{
friend class publisher< T >;
public:
    subscriber( publisher< T >&, pos& );

    subscriber< T >& subscribe();

    template< typename F >
    void dispatch( F func );

private:
    publisher< T >& pub_;
    pos& head_;
    pos tail_;
};

#include "subscriber.inl"
