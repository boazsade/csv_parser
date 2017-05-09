#pragma once
#include "utils/utility.hpp"
#include <cstddef>
#include <stdexcept>

namespace csv
{

template<typename Stream, int Size>
struct source : uds::convert2bool
{
    using stream_type = Stream;
    using char_type = typename Stream::char_type;
    using traits_type = typename Stream::traits_type;

    source() = default;

    explicit source(stream_type& s) : in_stream(&s)
    {
    }

    source(const source&) = default;
    source& operator = (const source&) = default;

    bool assign(stream_type& s)
    {
        in_stream = &s;
        return s.good();
    }

    char_type get()
    {
        if (!error()) {
            if (empty()) {
                if (eof()) {    // cannot read any more
                    return static_cast<char_type>(-1);
                } else {        // we need to re-fill the buffer
                    index = 0;
                    in_stream->read(internal_buffer, Size);
                    if (in_stream->gcount() != 0) {
                        in_read = in_stream->gcount();
                        if (in_read != index) {
                            return internal_buffer[index++];
                        } else {
                            return static_cast<char_type>(-1);
                        }
                    } else {
                        in_read = 0;    // now we would know that we do'nt have data
                    }
                }
            } else {    // we have data in the buffer return it
                return internal_buffer[index++];
            }
        } else {
            throw std::runtime_error{"invalid state - cannot read"};
            return static_cast<char_type>(-1);
        }
#if 0
        if (in_stream) {
            return in_stream.get();
        } else {
            return static_cast<char_type>(-1);
        }
#endif
    }

    bool eof() const
    {
        return empty() && in_stream && in_stream->eof();
    }

    bool error() const
    {
        return in_stream == nullptr || in_stream->bad();
    }

    bool good() const
    {
        return !(error() || eof());
    }

    operator boolean_type () const
    {
        return good() ? &uds::convert2bool::placeholder : (boolean_type)nullptr;
    }

    bool empty() const
    {
        return in_read == index;
    }

private:
    stream_type* in_stream = nullptr;
    char_type internal_buffer[Size];
    int index = 0;
    int in_read = 0;
};


}   // end of namespace csv

