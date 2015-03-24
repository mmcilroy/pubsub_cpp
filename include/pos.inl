inline pos::value_type pos::load()
{
    return value_.load();
}

inline void pos::store( pos::value_type v )
{
    value_.store( v );
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.notify_all();
}

inline void pos::wait( pos::value_type v )
{
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.wait( lock, [ this, v ]{
        return value_.load() > v;
    } );
}
