// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_YAON_PARSER_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_YAON_PARSER_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/detail/yaon_parser_read.hpp>
#include <boost/property_tree/detail/yaon_parser_write.hpp>
#include <boost/property_tree/detail/yaon_parser_error.hpp>

#include <fstream>
#include <string>
#include <locale>

namespace boost { namespace property_tree { namespace yaon_parser
{

    /**
     * Read YAON from a the given stream and translate it to a property tree.
     * @note Clears existing contents of property tree.  In case of error the
     *       property tree unmodified.
     * @note Items of YAON arrays are translated into ptree keys with empty
     *       names. Members of objects are translated into named keys.
     * @note YAON data can be a string, a numeric value, or one of literals
     *       "null", "true" and "false". During parse, any of the above is
     *       copied verbatim into ptree data string.
     * @throw yaon_parser_error In case of error deserializing the property
     *                          tree.
     * @param stream Stream from which to read in the property tree.
     * @param[out] pt The property tree to populate.
     */
    template<class Ptree>
    void read_yaon(std::basic_istream<
                       typename Ptree::key_type::value_type
                   > &stream,
                   Ptree &pt)
    {
        read_yaon_internal(stream, pt, std::string());
    }

    /**
     * Read YAON from a the given file and translate it to a property tree.
     * @note Clears existing contents of property tree.  In case of error the
     *       property tree unmodified.
     * @note Items of YAON arrays are translated into ptree keys with empty
     *       names. Members of objects are translated into named keys.
     * @note YAON data can be a string, a numeric value, or one of literals
     *       "null", "true" and "false". During parse, any of the above is
     *       copied verbatim into ptree data string.
     * @throw yaon_parser_error In case of error deserializing the property
     *                          tree.
     * @param filename Name of file from which to read in the property tree.
     * @param[out] pt The property tree to populate.
     * @param loc The locale to use when reading in the file contents.
     */
    template<class Ptree>
    void read_yaon(const std::string &filename,
                   Ptree &pt,
                   const std::locale &loc = std::locale())
    {
        std::basic_ifstream<typename Ptree::key_type::value_type>
            stream(filename.c_str());
        if (!stream)
            BOOST_PROPERTY_TREE_THROW(yaon_parser_error(
                "cannot open file", filename, 0));
        stream.imbue(loc);
        read_yaon_internal(stream, pt, filename);
    }

    /**
     * Translates the property tree to YAON and writes it the given output
     * stream.
     * @note Any property tree key containing only unnamed subkeys will be
     *       rendered as YAON arrays.
     * @pre @e pt cannot contain keys that have both subkeys and non-empty data.
     * @throw yaon_parser_error In case of error translating the property tree
     *                          to YAON or writing to the output stream.
     * @param stream The stream to which to write the YAON representation of the
     *               property tree.
     * @param pt The property tree to tranlsate to YAON and output.
     * @param pretty Whether to pretty-print. Defaults to true for backward
     *               compatibility.
     */
    template<class Ptree>
    void write_yaon(std::basic_ostream<
                        typename Ptree::key_type::value_type
                    > &stream,
                    const Ptree &pt,
                    bool pretty = true)
    {
        write_yaon_internal(stream, pt, std::string(), pretty);
    }

    /**
     * Translates the property tree to YAON and writes it the given file.
     * @note Any property tree key containing only unnamed subkeys will be
     *       rendered as YAON arrays.
     * @pre @e pt cannot contain keys that have both subkeys and non-empty data.
     * @throw yaon_parser_error In case of error translating the property tree
     *                          to YAON or writing to the file.
     * @param filename The name of the file to which to write the YAON
     *                 representation of the property tree.
     * @param pt The property tree to translate to YAON and output.
     * @param loc The locale to use when writing out to the output file.
     * @param pretty Whether to pretty-print. Defaults to true and last place
     *               for backward compatibility.
     */
    template<class Ptree>
    void write_yaon(const std::string &filename,
                    const Ptree &pt,
                    const std::locale &loc = std::locale(),
                    bool pretty = true)
    {
        std::basic_ofstream<typename Ptree::key_type::value_type>
            stream(filename.c_str());
        if (!stream)
            BOOST_PROPERTY_TREE_THROW(yaon_parser_error(
                "cannot open file", filename, 0));
        stream.imbue(loc);
        write_yaon_internal(stream, pt, filename, pretty);
    }

} } }

namespace boost { namespace property_tree
{
    using yaon_parser::read_yaon;
    using yaon_parser::write_yaon;
    using yaon_parser::yaon_parser_error;
} }

#endif
