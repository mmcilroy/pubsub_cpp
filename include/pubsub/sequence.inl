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

inline sequence::sequence()
{
    value_.store( 0 );
}

inline sequence::value_type sequence::load( std::memory_order o ) const
{
    return value_.load( o );
}

inline void sequence::store( value_type v, std::memory_order o )
{
    value_.store( v, o );
}

inline void blocking_sequence::store( value_type v, std::memory_order o )
{
    value_.store( v, o );
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.notify_all();
}

inline void blocking_sequence::wait( value_type v )
{
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.wait( lock, [ this, v ] {
        return value_.load( std::memory_order_relaxed ) > v;
    } );
}

inline void yielding_sequence::wait( value_type v )
{
    while( value_.load( std::memory_order_relaxed ) <= v ) {
        std::this_thread::yield();
    }
}
