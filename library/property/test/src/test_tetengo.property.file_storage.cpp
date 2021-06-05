/*! \file
    \brief A file storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <boost/core/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/platform_dependent/propertyX.hpp>
#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/storage.hpp>


namespace
{
    const std::string_view json1{
        // clang-format off
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": {\n"
        "    \"delta\": \"echo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json2{
        // clang-format off
        "{\n"
        "  \"\\\"\\b\\f\\n\\r\\t\": \"\xE3\x81\x82\\\\\\/\xF0\x9F\x98\x80\"\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json3_with_comment{
        // clang-format off
        "# comment1\n"
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": { # comment2\n"
        "    \"delta\": \"echo\"\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json4_escaped{
        // clang-format off
        "# comment1\n"
        "{\n"
        "  \"\\\"\\\\\\/\\b\\f\\n\\r\\t\": \"\\u3042\\uD83d\\uDe00\"\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json11_empty{};

    const std::string_view json12_syntax_error{
        // clang-format off
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": {\n"
        "    \"delta\": <>\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    const std::string_view json13_unsupported_syntax{
        // clang-format off
        "{\n"
        "  \"alpha\": true,\n"
        "  \"bravo\": 42,\n"
        "  \"charlie\": {\n"
        "    \"delta\": [24, \"echo\"],\n"
        "    \"foxtrot\": 4242\n"
        "  }\n"
        "}\n"
        // clang-format on
    };

    std::filesystem::path generic_path()
    {
        std::string name = "test_tetengo.property.file_storage";
        return std::filesystem::path{ name };
    }

    class test_file : private boost::noncopyable
    {
    public:
        test_file(const std::filesystem::path& path, const std::string_view& content) : m_path{ path }
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(m_path);
            ensure_directory_created(native_path);
            write_content(native_path, content);
        }

        ~test_file()
        {
            const auto native_top_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_top_path(m_path);
            std::error_code ec{};
            std::filesystem::remove_all(native_top_path, ec);
        }

        const std::filesystem::path& path() const
        {
            return m_path;
        }

    private:
        static void ensure_directory_created(const std::filesystem::path& native_path)
        {
            if (!native_path.has_parent_path() || std::filesystem::exists(native_path.parent_path()))
            {
                return;
            }
            std::filesystem::create_directories(native_path.parent_path());
        }

        static void write_content(const std::filesystem::path& native_path, const std::string_view& content)
        {
            std::ofstream stream{ native_path };
            stream.write(std::data(content), std::size(content));
        }

        const std::filesystem::path m_path;
    };

    bool file_content_equal_to(const std::filesystem::path& path, const std::string_view& expected)
    {
        std::string content{};
        {
            const auto native_path =
                tetengo::platform_dependent::property_set_file_path::instance().to_native_path(path);
            std::ifstream stream{ native_path };
            while (stream)
            {
                std::vector<char> read(1024, '\0');
                stream.read(std::data(read), std::size(read));
                content.append(std::data(read), static_cast<std::size_t>(stream.gcount()));
            }
        }

        return content == expected;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(property)
BOOST_AUTO_TEST_SUITE(file_storage)


BOOST_AUTO_TEST_CASE(save)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_file                              file{ generic_path(), "" };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());
        BOOST_REQUIRE(p_storage);

        p_storage->set_bool("alpha", true);
        p_storage->set_uint32("bravo", static_cast<std::uint32_t>(42));
        p_storage->set_string((std::filesystem::path{ "charlie" } / "delta").string(), std::string{ "echo" });

        p_storage->save();

        BOOST_TEST(file_content_equal_to(generic_path(), json1));
    }
    {
        const test_file                              file{ generic_path(), "" };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());
        BOOST_REQUIRE(p_storage);

        p_storage->set_string("\"\b\f\n\r\t", std::string{ "\xE3\x81\x82\\/\xF0\x9F\x98\x80" });

        p_storage->save();

        BOOST_TEST(file_content_equal_to(generic_path(), json2));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(file_storage_loader)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::property::file_storage_loader loader{};
    }
}

BOOST_AUTO_TEST_CASE(load)
{
    BOOST_TEST_PASSPOINT();

    {
        const test_file                              file{ generic_path(), json3_with_comment };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_bool("alpha"));
        BOOST_TEST(*p_storage->get_bool("alpha"));
        BOOST_REQUIRE(p_storage->get_uint32("bravo"));
        BOOST_TEST(*p_storage->get_uint32("bravo") == 42U);
        BOOST_REQUIRE(p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
        BOOST_TEST(*p_storage->get_string(std::filesystem::path{ "charlie" } / "delta") == "echo");
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" }));
    }
    {
        const test_file                              file{ generic_path(), json4_escaped };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_string("\"\\/\b\f\n\r\t"));
        constexpr std::string_view expected{ "\xE3\x81\x82\xF0\x9F\x98\x80" };
        BOOST_TEST(*p_storage->get_string("\"\\/\b\f\n\r\t") == expected);
    }
    {
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load("NONEXISTENT_FILE");

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("alpha"));
        BOOST_CHECK(!p_storage->get_uint32("bravo"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
    }
    {
        const test_file                              file{ generic_path(), json11_empty };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("alpha"));
        BOOST_CHECK(!p_storage->get_uint32("bravo"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
    }
    {
        const test_file                              file{ generic_path(), json12_syntax_error };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_CHECK(!p_storage->get_bool("alpha"));
        BOOST_CHECK(!p_storage->get_uint32("bravo"));
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
    }
    {
        const test_file                              file{ generic_path(), json13_unsupported_syntax };
        const tetengo::property::file_storage_loader loader{};
        const auto                                   p_storage = loader.load(file.path());

        BOOST_REQUIRE(p_storage);
        BOOST_REQUIRE(p_storage->get_bool("alpha"));
        BOOST_TEST(*p_storage->get_bool("alpha"));
        BOOST_REQUIRE(p_storage->get_uint32("bravo"));
        BOOST_TEST(*p_storage->get_uint32("bravo") == 42U);
        BOOST_CHECK(!p_storage->get_string(std::filesystem::path{ "charlie" } / "delta"));
        BOOST_REQUIRE(p_storage->get_uint32(std::filesystem::path{ "charlie" } / "foxtrot"));
        BOOST_TEST(*p_storage->get_uint32(std::filesystem::path{ "charlie" } / "foxtrot") == 4242U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
