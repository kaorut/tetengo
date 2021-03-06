/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP_)
#define TETENGO_TRIE_STORAGE_HPP_

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

#include <tetengo/trie/storage.hpp>


struct tetengo_trie_storage_tag
{
private:
    const tetengo::trie::storage* p_cpp_storage_referred;

public:
    std::unique_ptr<tetengo::trie::storage> p_cpp_storage_owned;

    std::size_t element_value_size;

    tetengo_trie_storage_tag(const tetengo::trie::storage* const p_cpp_storage, const std::size_t element_value_size) :
    p_cpp_storage_referred{ p_cpp_storage },
        p_cpp_storage_owned{},
        element_value_size{ element_value_size }
    {}

    tetengo_trie_storage_tag(
        std::unique_ptr<tetengo::trie::storage>&& p_cpp_storage,
        const std::size_t                         element_value_size) :
    p_cpp_storage_referred{},
        p_cpp_storage_owned{ std::move(p_cpp_storage) },
        element_value_size{ element_value_size }
    {}

    const tetengo::trie::storage* p_cpp_storage() const
    {
        assert(!p_cpp_storage_referred || !p_cpp_storage_owned);
        return p_cpp_storage_referred ? p_cpp_storage_referred : p_cpp_storage_owned.get();
    }
};


#endif
