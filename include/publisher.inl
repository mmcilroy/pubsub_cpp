template< typename T >
inline publisher< T >::publisher( size_t n ) :
    size_( 1<<n ),
    data_( new T[ size_ ] )
{
}

template< typename T >
template< typename F >
inline void publisher< T >::publish( size_t n, F func )
{
    sequence::value_type h = head_.load( std::memory_order_relaxed );

    while( h - tail() > size_ - n ) {
        std::this_thread::yield();
    }

    for( int i=0; i<n; i++ ) {
        func( at( h+i ) );
    }

    head_.store( h+n, std::memory_order_release );
}

template< typename T >
inline T& publisher< T >::at( sequence::value_type v )
{
    return data_[ v & size_-1 ];
}

template< typename T >
inline sequence::value_type publisher< T >::tail()
{
    sequence::value_type tail_min = std::numeric_limits< sequence::value_type >::max();
    for( auto it = tail_.begin(); it != tail_.end(); ) {
        tail_min = std::min( (*it)->tail_.load( std::memory_order_relaxed ), tail_min ); it++;
    }

    return tail_min;
}

template< typename T >
inline subscriber< T >& publisher< T >::subscribe()
{
    tail_.push_back( std::unique_ptr< subscriber< T > >(
        new subscriber< T >( *this, head_ ) ) );

    return *tail_.back();
}

template< typename T >
inline subscriber< T >& publisher< T >::subscribe( sequence& p )
{
    tail_.push_back( std::unique_ptr< subscriber< T > >(
        new subscriber< T >( *this, p ) ) );

    return *tail_.back();
}
