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

template< typename T, typename S >
inline subscriber< T, S >::subscriber( publisher< T, S >& pub, S& head ) :
    pub_( pub ),
    head_( head )
{
}

template< typename T, typename S >
template< typename F >
inline void subscriber< T, S >::dispatch( F func )
{
    bool done = false;

    while( !done )
    {
        typename S::value_type a;
        typename S::value_type t = tail_.load( std::memory_order_relaxed );

        while( ( a = head_.load( std::memory_order_acquire ) - t ) < 1 ) {
            head_.wait( t );
        }

        while( !done && a-- ) {
            done = func( pub_.at( t++ ), a );
        }

        tail_.store( t, std::memory_order_relaxed );
    }
}

template< typename T, typename S >
subscriber< T, S >& subscriber< T, S >::subscribe()
{
    return pub_.subscribe( tail_ );
}
