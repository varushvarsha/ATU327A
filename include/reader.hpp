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

#ifndef __READER_HPP
#define __READER_HPP 1

#include "__defines.hpp"
// #include <ext/new_allocator.h>
// #include <ext/bitmap_allocator.h>
// #include <ext/pool_allocator.h>

namespace __csv_ops {
    namespace __reader {	
	template <expression_type __tp_holder, typename __tp_string>
	struct __split_string {
	public:
	    using    string_type =   __tp_string;
	    using  vector_string =   __tp_holder<__tp_string>;
	    using      char_type =     char;
	private:
	    string_type     _in_string;
	    string_type     _delimiter;
	    vector_string   _vr_string;
	public:
	    inline constexpr __split_string(string_type const &in_string,
					    string_type const &delimiter): _in_string(in_string),
									   _delimiter(delimiter),
									   _vr_string() {
		string_type _buffer = "";
		std::size_t const inst_size = _in_string.size();
		std::size_t const dlim_size = _delimiter.size();
		std::size_t _offset_counter  = 0;
		std::size_t _fi = 0; std::size_t _li = 0; bool _last = false;
		for(std::size_t i=0; i<inst_size-dlim_size+1; ++i) {
		    for(std::size_t j=0; j<dlim_size; ++j) {
			if(_in_string.at(i+j) == _delimiter.at(j)) { ++_offset_counter; }
			else if(i+j+1 == inst_size) { _offset_counter = dlim_size; _last = true; }
			
			if(_offset_counter == dlim_size) {
			    _buffer.clear();
			    _offset_counter = 0;
			    _li = i;
			    if(_last == false) {
				for(std::size_t l=_fi; l<_li; ++l) _buffer+=_in_string.at(l);
			    } else {
				for(std::size_t l=_fi; l<inst_size; ++l) _buffer+=_in_string.at(l);
			    }
			    _fi = i+dlim_size;
			    _vr_string.push_back(_buffer);
			}
			
		    }
		    _offset_counter = 0;
		}
		_vr_string.shrink_to_fit();
	    }
	    inline constexpr __split_string(string_type const &in_string, char_type const &delimiter):
		__split_string(in_string, string_type("").operator+=(delimiter)) {}
	    inline constexpr bool exists()          const noexcept(false) { return _vr_string.size() > 1; }
	    inline constexpr std::size_t how_many() const noexcept(false) { return _vr_string.size(); }
	    inline constexpr vector_string get_vector_string() const noexcept(false) { return _vr_string; }
	};
	using split_string = __split_string<std::vector, std::string>; //split_string shorthand
	template <expression_type __tp_holder, typename __tp_string, typename __tp_fstream, typename __tp_char_traits>
	struct __reader {
	public:
	    using string_type         = __tp_string;
	    using string_v_type       = __tp_holder<__tp_string>;
	    using stream_type         = __tp_fstream;
	    using char_trtype         = __tp_char_traits;
	private:
	    string_type           _file_address;
	    string_v_type         _file_op_buffer;
	public:
	    inline constexpr __reader() = default;
	    inline constexpr __reader(string_type const &file_address): _file_address(file_address),
									_file_op_buffer() {
		    stream_type _fstream(file_address, std::ios::in);
		    if(!_fstream) { std::__throw_invalid_argument("Error: __reader() [File Not Found!]"); }
		    string_type _buffer = "";
		    int c = _fstream.rdbuf()->sgetc();
		    while(c != char_trtype::eof()) {
			if(c != '\n') {
			    _buffer+=c;			    
			} else {
			    _file_op_buffer.push_back(_buffer);
			    _buffer.clear();
			}
			c = _fstream.rdbuf()->snextc();
		    }
		    _file_op_buffer.shrink_to_fit();
		    _fstream.close();
		}
	    inline constexpr string_v_type get_lines() const noexcept(false) { return this->_file_op_buffer;  }
	    inline virtual ~__reader() {
	    }
	};
	using in_reader = __reader<std::vector, std::string, std::basic_ifstream<char>, std::char_traits<char>>;
    }
}

namespace reader {
    using  split_string  =   __csv_ops::__reader::split_string;
    using  reader        =   __csv_ops::__reader::in_reader;
}


#endif // __READER_HPP
