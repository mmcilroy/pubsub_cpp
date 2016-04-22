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

#include "subscriber.hpp"

#include <memory>
#include <vector>

namespace pubsub {

template< typename T, typename S=yielding_sequence >
class publisher
{
friend class subscriber< T, S >;
public:
    publisher( size_t );

    subscriber< T, S >& subscribe();

    template< typename F >
    void publish( size_t, F func );

private:
    subscriber< T, S >& subscribe( S& );

    T& at( typename S::value_type );

    typename S::value_type tail();

    typename S::value_type size_;

    typename S::value_type avail_;

    std::unique_ptr< T[] > data_;

    S head_;

    typedef std::vector< std::unique_ptr< subscriber< T, S > > > subscriber_list;
    subscriber_list tail_;
};

#include "publisher.inl"

}
