/*! \file
    \brief A reader.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <tetengo/json/reader.hpp>


namespace tetengo::json
{
    reader::~reader() = default;

    bool reader::has_next() const
    {
        return has_next_impl();
    }

    char reader::get() const
    {
        return get_impl();
    }

    void reader::next()
    {
        next_impl();
    }


}
