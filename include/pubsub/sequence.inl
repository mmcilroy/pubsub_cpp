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
