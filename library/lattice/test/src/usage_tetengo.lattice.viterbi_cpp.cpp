/*! \file
    \brief The usage of Viterbi search

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include "usage_tetengo.lattice.viterbi_cpp.hpp"

// [viterbi]
#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/stl_interfaces/iterator_interface.hpp>

#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace usage_tetengo::lattice
{
    std::unique_ptr<tetengo::lattice::vocabulary> build_vocabulary();
    std::string                                   to_string(const tetengo::lattice::path& path_);

    void viterbi()
    {
        /*
                  (3)    (2)   (4)   (7)    (2)
            [BOS]-----[a:Alpha]---[b:Bravo]-----[EOS]
                 \             \ /(1)          /
                  \(1)          X             /(6)
                   \           / \(5)        /
                    `-[a:Alice]---[b:Bob]---'
                         (1)   (9)  (8)
            Path                         Cost
            [BOS]-[Alice]-[Bravo]-[EOS]  1+1+1+7+2=12
            [BOS]-[Alpha]-[Bravo]-[EOS]  3+2+4+7+2=18
            [BOS]-[Alpha]-[Bob]---[EOS]  3+2+5+8+6=24
            [BOS]-[Alice]-[Bob]---[EOS]  1+1+9+8+6=25
        */
        auto p_vocabulary = build_vocabulary();

        tetengo::lattice::lattice lattice_{ std::move(p_vocabulary) };

        lattice_.push_back("a");
        lattice_.push_back("b");

        const auto eos_and_preceding_costs = lattice_.settle();

        std::vector<std::string>          paths{};
        tetengo::lattice::n_best_iterator first{ lattice_,
                                                 eos_and_preceding_costs.first,
                                                 std::make_unique<tetengo::lattice::constraint>() };
        std::for_each(
            std::move(first), tetengo::lattice::n_best_iterator{}, [&paths](const tetengo::lattice::path& path_) {
                paths.push_back(to_string(path_));
            });

        static const std::vector<std::string> expected{
            "[BOS]-[Alice]-[Bravo]-[EOS] (12)",
            "[BOS]-[Alpha]-[Bravo]-[EOS] (18)",
            "[BOS]-[Alpha]-[Bob]-[EOS] (24)",
            "[BOS]-[Alice]-[Bob]-[EOS] (25)",
        };
        assert(paths == expected);
    }

    std::string value_of(const tetengo::lattice::entry_view& entry)
    {
        return entry.value()->has_value() ? std::any_cast<std::string>(*entry.value()) : std::string{};
    }

    std::string value_of(const tetengo::lattice::node& node_, const bool first)
    {
        if (node_.value().has_value())
        {
            return std::any_cast<std::string>(node_.value());
        }
        else if (first)
        {
            return "BOS";
        }
        else
        {
            return "EOS";
        }
    }

    std::size_t entry_hash(const tetengo::lattice::entry_view& entry)
    {
        return std::hash<std::string_view>{}(entry.key()) ^ std::hash<std::string>{}(value_of(entry));
    }

    bool entry_equal_to(const tetengo::lattice::entry_view& entry1, const tetengo::lattice::entry_view& entry2)
    {
        return entry1.key() == entry2.key() && value_of(entry1) == value_of(entry2);
    }

    std::unique_ptr<tetengo::lattice::vocabulary> build_vocabulary()
    {
        static const std::vector<tetengo::lattice::entry> entries{
            { "a", std::string{ "Alpha" }, 2 },
            { "a", std::string{ "Alice" }, 1 },
            { "b", std::string{ "Bravo" }, 7 },
            { "b", std::string{ "Bob" }, 8 },
        };
        std::vector<std::pair<std::string, std::vector<tetengo::lattice::entry>>> entry_mappings{
            { "a", { entries[0], entries[1] } },
            { "b", { entries[2], entries[3] } },
        };
        std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
            { { tetengo::lattice::entry::bos_eos(), entries[0] }, 3 },
            { { tetengo::lattice::entry::bos_eos(), entries[1] }, 1 },
            { { entries[0], entries[2] }, 4 },
            { { entries[0], entries[3] }, 5 },
            { { entries[1], entries[2] }, 1 },
            { { entries[1], entries[3] }, 9 },
            { { entries[2], tetengo::lattice::entry::bos_eos() }, 2 },
            { { entries[3], tetengo::lattice::entry::bos_eos() }, 6 },
        };
        return std::make_unique<tetengo::lattice::unordered_map_vocabulary>(
            std::move(entry_mappings), std::move(connections), entry_hash, entry_equal_to);
    }

    std::string to_string(const tetengo::lattice::path& path_)
    {
        std::string result{};
        bool        first = true;
        for (const auto& node: path_.nodes())
        {
            if (!first)
            {
                result += "-";
            }
            result += "[" + value_of(node, first) + "]";
            first = false;
        }
        result += " (" + std::to_string(path_.cost()) + ")";
        return result;
    }
}
// [viterbi]
