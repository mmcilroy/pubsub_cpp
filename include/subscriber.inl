template< typename T >
inline subscriber< T >::subscriber( publisher< T >& pub, sequence& head ) :
    pub_( pub ),
    head_( head )
{
}

template< typename T >
template< typename F >
inline void subscriber< T >::dispatch( F func )
{
    bool done = false;

    while( !done )
    {
        sequence::value_type a;
        sequence::value_type t = tail_.load( std::memory_order_relaxed );

        while( ( a = head_.load( std::memory_order_acquire ) - t ) < 1 ) {
            head_.wait( t );
        }

        while( !done && a-- ) {
            done = func( pub_.at( t++ ), a );
        }

        tail_.store( t, std::memory_order_relaxed );
    }
}

template< typename T >
subscriber< T >& subscriber< T >::subscribe()
{
    return pub_.subscribe( tail_ );
}
