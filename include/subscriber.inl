template< typename T >
inline subscriber< T >::subscriber( publisher< T >& pub, pos& head ) :
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
        pos::value_type a;
        pos::value_type t = tail_.load();

        while( ( a = head_.load() - t ) < 1 ) {
            head_.wait( t );
        }

        while( !done && a-- ) {
            done = func( pub_.at( t++ ), a );
        }

        tail_.store( t );
    }
}

template< typename T >
subscriber< T >& subscriber< T >::subscribe()
{
    return pub_.subscribe( tail_ );
}
