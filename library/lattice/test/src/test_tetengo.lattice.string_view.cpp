/*! \file
    \brief A string view.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <string>

#include <boost/core/ignore_unused.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/string_view.h>


namespace
{
    constexpr char to_c(unsigned char uc)
    {
        return uc;
    }

    const std::string string_value{ to_c(0xE3), to_c(0x81), to_c(0xBF), to_c(0xE3), to_c(0x81),
                                    to_c(0x9A), to_c(0xE3), to_c(0x81), to_c(0xBB) };

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(string_view)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const tetengo_lattice_stringView_t string_view{ string_value.c_str(), string_value.length() };
    boost::ignore_unused(string_view);
}

BOOST_AUTO_TEST_CASE(p_head)
{
    BOOST_TEST_PASSPOINT();

    const tetengo_lattice_stringView_t string_view{ string_value.c_str(), string_value.length() };

    BOOST_TEST(string_view.p_head == string_value.c_str());
}

BOOST_AUTO_TEST_CASE(length)
{
    BOOST_TEST_PASSPOINT();

    const tetengo_lattice_stringView_t string_view{ string_value.c_str(), string_value.length() };

    BOOST_TEST(string_view.length == string_value.length());
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
