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
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.notify_all();
}

inline void sequence::wait( value_type v )
{
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.wait( lock, [ this, v ] {
        return value_.load( std::memory_order_relaxed ) > v;
    } );
}
