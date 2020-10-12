/*! \file
    \brief An east Asian width list.

    This file is generated by tools/make_east_asian_width_cpp.py.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TEXT_EASTASIANWIDTH_HPP_)
#define TETENGO_TEXT_EASTASIANWIDTH_HPP_

#include <cstddef>


namespace tetengo::text
{
    enum class east_asian_width_class_type
    {
        ambiguous,
        fullwidth,
        halfwidth,
        narrow,
        wide,
        neutral,
    };

    enum class grapheme_break_property_type
    {
        cr,
        lf,
        control,
        extend,
        zwj,
        regional,
        prepend,
        spacing_mark,
        l,
        v,
        t,
        lv,
        lvt,
        other,
    };


    struct character_property_map_element_type
    {
        char32_t code_point;

        east_asian_width_class_type class_;

        grapheme_break_property_type grapheme;
    };


    extern const std::size_t character_property_map_size;

    extern const character_property_map_element_type character_property_map[];


}


#endif
