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
inline publisher< T, S >::publisher( size_t n ) :
    size_( 1<<n ),
    avail_( 0 ),
    data_( new T[ size_ ] )
{
}

template< typename T, typename S >
template< typename F >
inline void publisher< T, S >::publish( size_t n, F func )
{
    typename S::value_type h = head_.load( std::memory_order_relaxed );

    if( avail_ < n )
    {
        while( ( avail_ = size_ - ( h - tail() ) ) < n ) {
            std::this_thread::yield();
        }
    }

    for( int i=0; i<n; i++ ) {
        func( at( h+i ), i );
    }

    head_.store( h+n, std::memory_order_release );

    avail_ -= n;
}

template< typename T, typename S >
inline T& publisher< T, S >::at( typename S::value_type v )
{
    return data_[ v & size_-1 ];
}

template< typename T, typename S >
inline typename S::value_type publisher< T, S >::tail()
{
    typename S::value_type tail_min = std::numeric_limits< typename S::value_type >::max();
    for( auto it = tail_.begin(); it != tail_.end(); ) {
        tail_min = std::min( (*it)->tail_.load( std::memory_order_relaxed ), tail_min ); it++;
    }

    return tail_min;
}

template< typename T, typename S >
inline subscriber< T, S >& publisher< T, S >::subscribe()
{
    tail_.push_back( std::unique_ptr< subscriber< T, S > >(
        new subscriber< T, S >( *this, head_ ) ) );

    return *tail_.back();
}

template< typename T, typename S >
inline subscriber< T, S >& publisher< T, S >::subscribe( S& p )
{
    tail_.push_back( std::unique_ptr< subscriber< T, S > >(
        new subscriber< T, S >( *this, p ) ) );

    return *tail_.back();
}
