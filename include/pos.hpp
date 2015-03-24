#pragma once

#include <atomic>
#include <condition_variable>

class pos
{
public:
    typedef uint_fast64_t value_type;

    value_type load();

    void store( value_type v );

    void wait( value_type v );

private:
    std::atomic< value_type > value_ = { 0 };
    std::condition_variable cond_;
    std::mutex mut_;
};

#include "pos.inl"
