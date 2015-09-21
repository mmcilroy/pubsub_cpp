#pragma once

#include "sequence.hpp"

namespace pubsub {

template< typename T, typename S >
class publisher;

template< typename T, typename S=yielding_sequence >
class subscriber
{
friend class publisher< T, S >;
public:
    subscriber( publisher< T, S >&, S& );

    subscriber< T, S >& subscribe();

    template< typename F >
    void dispatch( F func );

private:
    publisher< T, S >& pub_;
    S& head_;
    S  tail_;
};

#include "subscriber.inl"

}
