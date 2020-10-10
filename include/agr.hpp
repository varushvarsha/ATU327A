/*
Copyright (C) 2020  Varush Varsha

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef AGR_HPP
#define AGR_HPP

#include "reader.hpp"

namespace agr {
    template <typename __tp_argc, typename __tp_argv, typename __tp_reader, typename __tp_container, typename __tp_string>
    struct __parsing_args {
    public:
        using argc_type      = __tp_argc;
        using argv_type      = __tp_argv;
        using ar_reader      = __tp_reader;
        using container      = __tp_container;
        using string_type    = __tp_string;
    private:
        argc_type _argc;
        argv_type _argv;
        container names;
    public:
        inline constexpr explicit __parsing_args(argc_type argc, argv_type argv): _argc(argc), _argv(argv), names() {
            if(_argc == 2) {                
                names = _names_list();
            } else {
                for(argc_type d=1; d<_argc; ++d) {
                    names.emplace_back(_argv[d]);
                }
	    }
        }
        inline constexpr container _names_list() const noexcept(false) {
            if(valid_extension()) {
                __csv_ops::__reader::in_reader rdr(_argv[1]);
                return rdr.get_lines();;
            } else {
                return { _argv[1] };
            }
        }
        inline constexpr bool valid_extension() const noexcept(false) {
	    __csv_ops::__reader::split_string sp(_argv[1], '.');
	    string_type const extension = sp.get_vector_string().back();
	    if((extension == "csv") || (extension == "txt") || (extension == "tsv")) { return true; }
	    return false;
        }
        inline constexpr container get_instrument_names() const noexcept(false) { return this->names; }
    };

    using parsing_args = __parsing_args<int, char**, reader::reader, std::vector<std::string>, std::string>;
}


#endif // AGR_HPP
