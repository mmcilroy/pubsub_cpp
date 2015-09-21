#pragma once

#include <atomic>
#include <condition_variable>
#include <thread>

namespace pubsub {

class sequence
{
public:
    typedef uint_fast64_t value_type;

    sequence();

    value_type load( std::memory_order o ) const;

    void store( value_type v, std::memory_order o );

protected:
    std::atomic_uint_fast64_t value_;
};

class blocking_sequence : public sequence
{
public:
    void store( value_type v, std::memory_order o );

    void wait( value_type v );

private:
    std::condition_variable cond_;
    std::mutex mut_;
};

class yielding_sequence : public sequence
{
public:
    void wait( value_type v );
};

#include "sequence.inl"

}
