#pragma once

#include "subscriber.hpp"

#include <memory>
#include <vector>
#include <mutex>
#include <thread>

template< typename T >
class publisher
{
friend class subscriber< T >;
public:
    publisher( size_t );

    subscriber< T >& subscribe();

    template< typename F >
    void publish( size_t, F func );

private:
    subscriber< T >& subscribe( pos& );

    T& at( pos::value_type );

    pos::value_type tail();

    std::mutex mut_;

    size_t size_;

    std::unique_ptr< T[] > data_;

    pos head_;

    typedef std::vector< std::unique_ptr< subscriber< T > > > subscriber_list;
    subscriber_list tail_;
};

#include "publisher.inl"
