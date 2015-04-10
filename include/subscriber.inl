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
