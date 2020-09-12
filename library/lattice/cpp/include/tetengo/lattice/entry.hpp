/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_LATTICE_ENTRY_HPP)
#define TETENGO_LATTICE_ENTRY_HPP

#include <any>
#include <string>
#include <string_view>


namespace tetengo::lattice
{
#if !defined(DOCUMENTATION)
    namespace temp
    {
        std::any empty_std_any();

        bool std_any_has_value(const std::any& any_);


    }
#endif


    /*!
        \brief A template of an entry.

        \tparam Key   A key type.
        \tparam Value A value type.
    */
    template <typename Key, typename Value>
    class basic_entry
    {
    public:
        // types

        //! The key type.
        using key_type = Key;

        //! The value type.
        using value_type = Value;


        // static functions

        /*!
            \brief Returns the BOS/EOS (Beginning/End of Sequence) entry.

            \return The BOS/EOS entry.
        */
        [[nodiscard]] static const basic_entry& bos_eos();


        // constructors

        /*!
            \brief Creates an entry.

            \param key   A key.
            \param value A value.
            \param cost  A cost.
        */
        basic_entry(key_type key, value_type value, int cost);

        /*!
            \brief Copies an entry.

            \tparam K A key type.
            \tparam V A value type.

            \param another Another entry.
        */
        template <typename K, typename V>
        basic_entry(const basic_entry<K, V>& another);


        // functions

        /*!
            \brief Returns the key.

            \return The key.
        */
        [[nodiscard]] const key_type& key() const;

        /*!
            \brief Returns the value.

            \return The value.
        */
        [[nodiscard]] const value_type& value() const;

        /*!
            \brief Returns the cost.

            \return The cost.
        */
        [[nodiscard]] int cost() const;


    private:
        // variables

        key_type m_key;

        value_type m_value;

        int m_cost;
    };


    /*!
        \brief An entry.
    */
    using entry = basic_entry<std::string, std::any>;

    /*!
        \brief An entry view.
    */
    using entry_view = basic_entry<std::string_view, const std::any*>;


}


#endif
