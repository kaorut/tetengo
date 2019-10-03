/*!
    \brief A double array.

    Copyright (C) 2019 kaoru
 */

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <list>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/trie/double_array.h>
#include <tetengo/trie/double_array.hpp>

struct tetengo_trie_doublearray;


namespace
{
    const std::vector<std::uint32_t> expected_empty_base_check_array_empty{
        //                  BASE  CHECK  BYTECHECK
        0x000000FF, // [ 0]    0,    -1,        -1
    };

    /*
              \0
        [ 0]+---[ 1]
            |
            |' '      \0
            +---[ 2]----[ 3]
    */

    const std::vector<std::pair<std::string, std::int32_t>> expected_values0{ { "", 42 }, { " ", 24 } };

    const std::vector<std::uint32_t> expected_base_check_array0{
        //                  BASE  CHECK  BYTECHECK
        0x000001FF, // [ 0]    1,    -1,        -1
        0x00002A00, // [ 1]   42,     0,         0
        0x000000FF, // [ 2]    0,    -1,        -1
        0x000000FF, // [ 3]    0,    -1,        -1
        0x000000FF, // [ 4]    0,    -1,        -1
        0x000000FF, // [ 5]    0,    -1,        -1
        0x000000FF, // [ 6]    0,    -1,        -1
        0x000000FF, // [ 7]    0,    -1,        -1
        0x000000FF, // [ 8]    0,    -1,        -1
        0x000000FF, // [ 9]    0,    -1,        -1
        0x000000FF, // [10]    0,    -1,        -1
        0x000000FF, // [11]    0,    -1,        -1
        0x000000FF, // [12]    0,    -1,        -1
        0x000000FF, // [13]    0,    -1,        -1
        0x000000FF, // [14]    0,    -1,        -1
        0x000000FF, // [15]    0,    -1,        -1
        0x000000FF, // [16]    0,    -1,        -1
        0x000000FF, // [17]    0,    -1,        -1
        0x000000FF, // [18]    0,    -1,        -1
        0x000000FF, // [19]    0,    -1,        -1
        0x000000FF, // [20]    0,    -1,        -1
        0x000000FF, // [21]    0,    -1,        -1
        0x000000FF, // [22]    0,    -1,        -1
        0x000000FF, // [23]    0,    -1,        -1
        0x000000FF, // [24]    0,    -1,        -1
        0x000000FF, // [25]    0,    -1,        -1
        0x000000FF, // [26]    0,    -1,        -1
        0x000000FF, // [27]    0,    -1,        -1
        0x000000FF, // [28]    0,    -1,        -1
        0x000000FF, // [29]    0,    -1,        -1
        0x000000FF, // [30]    0,    -1,        -1
        0x000000FF, // [31]    0,    -1,        -1
        0x000000FF, // [32]    0,    -1,        -1
        0x00002220, // [33]   34,     0,        32
        0x00001800, // [34]   24,    33,         0
    };

    /*
              S       E       T       A       \0
        [ 0]+---[ 1]----[ 2]----[ 4]----[ 5]----[ 6]
            |
            | U       T       I       G       O       S       I       \0
            +---[ 3]----[ 7]+---[ 8]----[ 9]----[10]----[11]----[12]----[13]
                            |
                            | O       \0
                            +---[14]----[15]
    */

    const std::vector<std::pair<std::string, std::int32_t>> expected_values3{ { "UTIGOSI", 24 },
                                                                              { "UTO", 2424 },
                                                                              { "SETA", 42 } };

    const std::vector<std::uint32_t> expected_base_check_array3{
        //                  BASE  CHECK  BYTECHECK
        0xFFFFAEFF, // [ 0]  -82,    -1,        -1
        0xFFFFBD53, // [ 1]  -67,     0,        83
        0xFFFFB045, // [ 2]  -80,     1,        69
        0xFFFFB355, // [ 3]  -77,     0,        85
        0xFFFFC454, // [ 4]  -60,     2,        84
        0x00000641, // [ 5]    6,     4,        65
        0x00002A00, // [ 6]   42,     5,         0
        0xFFFFBF54, // [ 7]  -65,     3,        84
        0xFFFFC249, // [ 8]  -62,     7,        73
        0xFFFFBB47, // [ 9]  -69,     8,        71
        0xFFFFB84F, // [10]  -72,     9,        79
        0xFFFFC353, // [11]  -61,    10,        83
        0x00000D49, // [12]   13,    11,        73
        0x00001800, // [13]   24,    12,         0
        0x00000F4F, // [14]   15,     7,        79
        0x00097800, // [15] 2424,    14,         0
    };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(trie)
BOOST_AUTO_TEST_SUITE(double_array)


BOOST_AUTO_TEST_CASE(key_terminator)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::key_terminator() == '\0');

    // TODO: C style API
}

BOOST_AUTO_TEST_CASE(vacant_check_value)
{
    BOOST_TEST_PASSPOINT();

    BOOST_TEST(tetengo::trie::double_array::vacant_check_value() == 0xFF);

    // TODO: C style API
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        BOOST_TEST(double_array_.base_check_array() == expected_empty_base_check_array_empty);
    }
    {
        tetengo_trie_doublearray* const p_double_array = tetengo_trie_doublearray_create();
        BOOST_SCOPE_EXIT(p_double_array)
        {
            tetengo_trie_doublearray_destroy(p_double_array);
        }
        BOOST_SCOPE_EXIT_END;
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values0 };

        BOOST_TEST(double_array_.base_check_array() == expected_base_check_array0);
    }
    {
        // TODO: C style API
    }

    {
        std::list<std::pair<std::string, std::int32_t>> expected_values3_as_list{ expected_values3.begin(),
                                                                                  expected_values3.end() };

        std::vector<const std::pair<std::string, std::int32_t>*> expected_values3_as_ptr_vector{};
        expected_values3_as_ptr_vector.reserve(expected_values3_as_list.size());
        std::transform(
            expected_values3_as_list.begin(),
            expected_values3_as_list.end(),
            std::back_inserter(expected_values3_as_ptr_vector),
            [](const auto& e) { return &e; });

        const tetengo::trie::double_array double_array_{ std::move(expected_values3_as_ptr_vector) };

        BOOST_TEST(double_array_.base_check_array() == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }

    {
        std::list<std::pair<std::string, std::int32_t>> expected_values3_as_list{ expected_values3.begin(),
                                                                                  expected_values3.end() };

        const tetengo::trie::double_array double_array_{ expected_values3_as_list.begin(),
                                                         expected_values3_as_list.end() };

        BOOST_TEST(double_array_.base_check_array() == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        BOOST_TEST(double_array_.base_check_array() == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(base_check_array)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        const auto& base_check_array = double_array_.base_check_array();

        BOOST_TEST(base_check_array == expected_empty_base_check_array_empty);
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        const auto& base_check_array = double_array_.base_check_array();

        BOOST_TEST(base_check_array == expected_base_check_array3);
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(find)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        {
            const auto found = double_array_.find("SETA");
            BOOST_CHECK(!found);
        }
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        {
            const auto found = double_array_.find("SETA");
            BOOST_REQUIRE(found);
            BOOST_TEST(*found == 42);
        }
        {
            const auto found = double_array_.find("UTIGOSI");
            BOOST_REQUIRE(found);
            BOOST_TEST(*found == 24);
        }
        {
            const auto found = double_array_.find("UTO");
            BOOST_REQUIRE(found);
            BOOST_TEST(*found == 2424);
        }
        {
            const auto found = double_array_.find("SUIZENJI");
            BOOST_CHECK(!found);
        }
    }
    {
        // TODO: C style API
    }
}

BOOST_AUTO_TEST_CASE(get_enumerator)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::trie::double_array double_array_{};

        const auto enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }

    {
        const tetengo::trie::double_array double_array_{ expected_values3 };

        const auto enumerator = double_array_.get_enumerator();
    }
    {
        // TODO: C style API
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
