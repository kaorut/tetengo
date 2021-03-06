/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <string_view>
#include <vector>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class node;


    vocabulary::vocabulary() = default;

    vocabulary::~vocabulary() = default;

    std::vector<entry_view> vocabulary::find_entries(const std::string_view& key) const
    {
        return find_entries_impl(key);
    }

    connection vocabulary::find_connection(const node& from, const entry_view& to) const
    {
        return find_connection_impl(from, to);
    }


}
