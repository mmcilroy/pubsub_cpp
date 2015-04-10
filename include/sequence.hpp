#pragma once

#include <atomic>
#include <condition_variable>

class sequence
{
public:
    typedef uint_fast64_t value_type;

    sequence();

    // get the current value
    value_type load( std::memory_order o ) const;

    // set a new value
    void store( value_type v, std::memory_order o );

    // wait until value changes to v
    void wait( value_type v );

private:
    std::atomic_uint_fast64_t value_;
    std::condition_variable cond_;
    std::mutex mut_;
};

#include "sequence.inl"
