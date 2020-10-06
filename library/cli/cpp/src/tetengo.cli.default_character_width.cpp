/*! \file
    \brief A default character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/character_width.hpp>
#include <tetengo/cli/default_character_width.hpp>


namespace tetengo::cli
{
    class default_character_width::impl : private boost::noncopyable
    {
    public:
        // static functions

        static const default_character_width& instance()
        {
            static const default_character_width singleton{};
            return singleton;
        }


        // functions

        std::size_t width_of_impl(const class_type class_) const
        {
            switch (class_)
            {
            case class_type::fullwidth:
            case class_type::wide:
                return 2;
            default:
                return 1;
            }
        }
    };


    const default_character_width& default_character_width::instance()
    {
        return impl::instance();
    }

    default_character_width::~default_character_width() = default;

    default_character_width::default_character_width() : m_p_impl{ std::make_unique<impl>() } {}

    std::size_t default_character_width::width_of_impl(const class_type class_) const
    {
        return m_p_impl->width_of_impl(class_);
    }


}