#pragma once

#include "sequence.hpp"

template< typename T >
class publisher;

template< typename T >
class subscriber
{
friend class publisher< T >;
public:
    subscriber( publisher< T >&, sequence& );

    subscriber< T >& subscribe();

    template< typename F >
    void dispatch( F func );

private:
    publisher< T >& pub_;
    sequence& head_;
    sequence tail_;
};

#include "subscriber.inl"
