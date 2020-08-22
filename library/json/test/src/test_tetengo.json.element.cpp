/*! \file
    \brief An element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/json/element.h>
#include <tetengo/json/element.hpp>


namespace
{
    using element_type = tetengo::json::element;


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(element)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::string,
                                         element_type::type_category_type::structure_open },
                "test",
                std::unordered_map<std::string, std::string>{}),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::string,
                                         element_type::type_category_type::primitive },
                "test",
                std::unordered_map<std::string, std::string>{ { "key", "value" } }),
            std::invalid_argument);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::object,
                                         element_type::type_category_type::primitive },
                std::string{},
                std::unordered_map<std::string, std::string>{}),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::object,
                                         element_type::type_category_type::structure_open },
                "test",
                std::unordered_map<std::string, std::string>{}),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::object,
                                         element_type::type_category_type::structure_open },
                std::string{},
                std::unordered_map<std::string, std::string>{ { "key", "value" } }),
            std::invalid_argument);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::member,
                                         element_type::type_category_type::structure_open },
                std::string{},
                std::unordered_map<std::string, std::string>{ { "key", "value" } }),
            std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(
            const element_type element(
                element_type::type_type{ element_type::type_name_type::member,
                                         element_type::type_category_type::structure_close },
                std::string{},
                std::unordered_map<std::string, std::string>{ { "name", "value" } }),
            std::invalid_argument);
    }

    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_element);
    }
    {
        const auto* const p_element = tetengo_json_element_create(nullptr, "test", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, nullptr, nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_string(),
                                                tetengo_json_element_typeCategory_primitive() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "key", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "test", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_primitive() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const auto* const                 p_element = tetengo_json_element_create(&type, "test", nullptr, 0);
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_object(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "key", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "test", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_SCOPE_EXIT(p_element)
        {
            tetengo_json_element_destroy(p_element);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureOpen() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "key", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
    {
        const tetengo_json_element_type_t                           type{ tetengo_json_element_typeName_member(),
                                                tetengo_json_element_typeCategory_structureClose() };
        const std::vector<tetengo_json_element_attributeKeyValue_t> attributes{ { "name", "value" } };
        const auto* const p_element = tetengo_json_element_create(&type, "", attributes.data(), attributes.size());
        BOOST_TEST(!p_element);
    }
}

BOOST_AUTO_TEST_CASE(type)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_CHECK(
            element.type().name == element_type::type_name_type::string &&
            element.type().category == element_type::type_category_type::primitive);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_CHECK(
            element.type().name == element_type::type_name_type::object &&
            element.type().category == element_type::type_category_type::structure_close);
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };

        BOOST_CHECK(
            element.type().name == element_type::type_name_type::member &&
            element.type().category == element_type::type_category_type::structure_open);
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.value() == "test");
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.value().empty());
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };

        BOOST_TEST(element.value().empty());
    }
}

BOOST_AUTO_TEST_CASE(attributes)
{
    BOOST_TEST_PASSPOINT();

    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::string,
                                                             element_type::type_category_type::primitive },
                                    "test",
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.attributes().empty());
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::object,
                                                             element_type::type_category_type::structure_close },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{} };

        BOOST_TEST(element.attributes().empty());
    }
    {
        const element_type element{ element_type::type_type{ element_type::type_name_type::member,
                                                             element_type::type_category_type::structure_open },
                                    std::string{},
                                    std::unordered_map<std::string, std::string>{ { "name", "value" } } };

        BOOST_TEST(element.attributes().size() == 1U);
        BOOST_CHECK(element.attributes().find("name") != element.attributes().end());
        BOOST_TEST(element.attributes().find("name")->second == "value");
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
