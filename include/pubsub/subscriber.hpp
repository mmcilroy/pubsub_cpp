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
