/*! \file
    \brief An unordered_map vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/string_view.h>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::string key_mizuho{ to_c(0xE3), to_c(0x81), to_c(0xBF), to_c(0xE3), to_c(0x81),
                                  to_c(0x9A), to_c(0xE3), to_c(0x81), to_c(0xBB) };

    const std::string surface_mizuho{ to_c(0xE7), to_c(0x91), to_c(0x9E), to_c(0xE7), to_c(0xA9), to_c(0x82) };

    const std::string key_sakura{ to_c(0xE3), to_c(0x81), to_c(0x95), to_c(0xE3), to_c(0x81),
                                  to_c(0x8F), to_c(0xE3), to_c(0x82), to_c(0x89) };

    const std::string surface_sakura1{ to_c(0xE6), to_c(0xA1), to_c(0x9C) };

    const std::string surface_sakura2{ to_c(0xE3), to_c(0x81), to_c(0x95), to_c(0xE3), to_c(0x81),
                                       to_c(0x8F), to_c(0xE3), to_c(0x82), to_c(0x89) };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(unordered_map_vocabulary)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::unordered_map<std::string, std::vector<tetengo::lattice::entry>> map{};
        const tetengo::lattice::unordered_map_vocabulary                            vocabulary{ map };
    }
    {
        const std::unordered_map<std::string, std::vector<tetengo::lattice::entry>> map{
            { key_mizuho, { { key_mizuho, surface_mizuho, 42 } } },
            { key_sakura, { { key_sakura, surface_sakura1, 24 }, { key_sakura, surface_sakura2, 2424 } } }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{ map };
    }

    {
        const std::vector<tetengo_lattice_entry> entries_mizuho{
            { { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 }
        };
        const std::vector<tetengo_lattice_entry> entries_sakura{
            { { key_sakura.c_str(), key_sakura.length() }, &surface_sakura1, 24 },
            { { key_sakura.c_str(), key_sakura.length() }, &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_entry_map_element> map{
            { { key_mizuho.c_str(), key_mizuho.length() }, entries_mizuho.data(), entries_mizuho.size() },
            { { key_sakura.c_str(), key_sakura.length() }, entries_sakura.data(), entries_sakura.size() }
        };

        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(map.data(), 2);
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;

        BOOST_TEST(p_vocabulary);
    }
    {
        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(nullptr, 0);

        BOOST_TEST(!p_vocabulary);
    }
    {
        const auto* const p_vocabulary = tetengo_lattice_vocabulary_createUnorderedMapVocabulary(nullptr, 42);

        BOOST_TEST(!p_vocabulary);
    }
}

BOOST_AUTO_TEST_CASE(find_entries)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::unordered_map<std::string, std::vector<tetengo::lattice::entry>> map{};
        const tetengo::lattice::unordered_map_vocabulary                            vocabulary{ map };

        {
            const auto found = vocabulary.find_entries(key_mizuho);
            BOOST_TEST(found.empty());
        }
        {
            const auto found = vocabulary.find_entries(key_sakura);
            BOOST_TEST(found.empty());
        }
    }
    {
        const std::unordered_map<std::string, std::vector<tetengo::lattice::entry>> map{
            { key_mizuho, { { key_mizuho, surface_mizuho, 42 } } },
            { key_sakura, { { key_sakura, surface_sakura1, 24 }, { key_sakura, surface_sakura2, 2424 } } }
        };
        const tetengo::lattice::unordered_map_vocabulary vocabulary{ map };

        {
            const auto found = vocabulary.find_entries(key_mizuho);
            BOOST_TEST_REQUIRE(found.size() == 1U);
            BOOST_TEST(found[0].key() == key_mizuho);
            BOOST_TEST(*std::any_cast<std::string>(found[0].value()) == surface_mizuho);
            BOOST_TEST(found[0].cost() == 42);
        }
        {
            const auto found = vocabulary.find_entries(key_sakura);
            BOOST_TEST_REQUIRE(found.size() == 2U);
            BOOST_TEST(found[0].key() == key_sakura);
            BOOST_TEST(*std::any_cast<std::string>(found[0].value()) == surface_sakura1);
            BOOST_TEST(found[0].cost() == 24);
            BOOST_TEST(found[1].key() == key_sakura);
            BOOST_TEST(*std::any_cast<std::string>(found[1].value()) == surface_sakura2);
            BOOST_TEST(found[1].cost() == 2424);
        }
    }

    {
        const std::vector<tetengo_lattice_entry> entries_mizuho{
            { { key_mizuho.c_str(), key_mizuho.length() }, &surface_mizuho, 42 }
        };
        const std::vector<tetengo_lattice_entry> entries_sakura{
            { { key_sakura.c_str(), key_sakura.length() }, &surface_sakura1, 24 },
            { { key_sakura.c_str(), key_sakura.length() }, &surface_sakura2, 2424 }
        };
        const std::vector<tetengo_lattice_entry_map_element> map{
            { { key_mizuho.c_str(), key_mizuho.length() }, entries_mizuho.data(), entries_mizuho.size() },
            { { key_sakura.c_str(), key_sakura.length() }, entries_sakura.data(), entries_sakura.size() }
        };

        const auto* const p_vocabulary =
            tetengo_lattice_vocabulary_createUnorderedMapVocabulary(map.data(), map.size());
        BOOST_SCOPE_EXIT(p_vocabulary)
        {
            tetengo_lattice_vocabulary_destroy(p_vocabulary);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_vocabulary);

        {
            const auto entry_count = tetengo_lattice_vocabulary_findEntries(p_vocabulary, key_mizuho.c_str(), nullptr);
            BOOST_TEST(entry_count == 1U);

            std::vector<tetengo_lattice_entry> entries{ entry_count };
            const auto                         entry_count_again =
                tetengo_lattice_vocabulary_findEntries(p_vocabulary, key_mizuho.c_str(), entries.data());
            BOOST_TEST_REQUIRE(entry_count_again == 1U);

            BOOST_TEST((std::string{ entries[0].key.p_head, entries[0].key.length } == key_mizuho));
            BOOST_TEST(*reinterpret_cast<const std::string*>(entries[0].p_value) == surface_mizuho);
            BOOST_TEST(entries[0].cost == 42);
        }
        {
            const auto entry_count = tetengo_lattice_vocabulary_findEntries(p_vocabulary, key_sakura.c_str(), nullptr);
            BOOST_TEST(entry_count == 2U);

            std::vector<tetengo_lattice_entry> entries{ entry_count };
            const auto                         entry_count_again =
                tetengo_lattice_vocabulary_findEntries(p_vocabulary, key_sakura.c_str(), entries.data());
            BOOST_TEST_REQUIRE(entry_count_again == 2U);

            BOOST_TEST((std::string{ entries[0].key.p_head, entries[0].key.length } == key_sakura));
            BOOST_TEST(*reinterpret_cast<const std::string*>(entries[0].p_value) == surface_sakura1);
            BOOST_TEST(entries[0].cost == 24);
            BOOST_TEST((std::string{ entries[1].key.p_head, entries[1].key.length } == key_sakura));
            BOOST_TEST(*reinterpret_cast<const std::string*>(entries[1].p_value) == surface_sakura2);
            BOOST_TEST(entries[1].cost == 2424);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
