/*
 * Copyright 2016 Mark McIlroy.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
