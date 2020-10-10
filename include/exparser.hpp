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

#ifndef __EXPARSER_HPP
#define __EXPARSER_HPP

#include "gretel.hpp"
#include "reader.hpp"

namespace __express { //not exactly ;) (yet!)
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// REMOVE THIS ///
    template <typename __tp>
    inline void print_vector(__tp const &in_vector) {
        if(in_vector.size() == 0) { std::__throw_invalid_argument("Error:void print_vector(__tp const &in_vector) size is 0"); }
        using value_tp = typename __tp::value_type;
        using value_it = typename __tp::const_iterator;
        value_it v_iter = in_vector.begin();
        for(const value_tp &e: in_vector) {
	    ++v_iter;
	    (v_iter != in_vector.end())  ? std::cout << "(" << e << "), " : std::cout << "(" << e << ")\n";
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    template <typename __tp_double, typename __tp_string, expression_type __tp_container,
	      typename __tp_gretel, typename __tp_aux_reader,
	      typename __tp_splitstring>
    struct __exparser {
	using string_type   = __tp_string;
	using reader_type   = __tp_aux_reader;
	using double_type   = __tp_double;
	using greteloader   = __tp_gretel;
	using vx_double     = __tp_container<double_type>;
	using vx_string     = __tp_container<string_type>;
	using split_type    = __tp_splitstring;
    private:
	string_type    _file_name; //full_path should be provided
	char           _delimiter;
	reader_type    _r_reader;
	vx_string      _lines;		
    protected:
	std::size_t   total_size;
	vx_string     dates;
	vx_double     open;
	vx_double     high;
	vx_double     low;
	vx_double     close;
	vx_double     adj_close;
	vx_double     volume;
    public:
	inline constexpr explicit __exparser(string_type const &file_name,
					     char const &delimiter=','): _file_name(file_name),		    
									 _delimiter(delimiter),
									 _r_reader(_file_name),
									 _lines(_r_reader.get_lines()) {
	    if(_lines.size() < 5) {
		std::fprintf(stderr, "FILE(%s) lines size is [%ld]\n", _file_name.c_str(), _lines.size());
		std::__throw_length_error("Error: __exparser() [(perhaps) Not a valid csv file!]");
	    }
	    std::size_t valid_size = 0;
	    for(string_type const &line: _lines) {
		split_type line_split(line, delimiter);
		vx_string const one = line_split.get_vector_string();
		if(one.size() != 7) {
		    std::fprintf(stderr, "%s\n", line.c_str());
		    std::__throw_length_error("Error: __exparser() [line_split.size() != 7]");
		}
		string_type const _date       = one.at(0);
		double_type const _open       = std::atof(one.at(1).c_str());
		double_type const _high       = std::atof(one.at(2).c_str());
		double_type const _low        = std::atof(one.at(3).c_str());
		double_type const _close      = std::atof(one.at(4).c_str());
		double_type const _adj_close  = std::atof(one.at(5).c_str());
		double_type const _volume     = std::atof(one.at(6).c_str());
		if(_open != 0.0) {
		    if(_close != 0.0) {
			dates.push_back(_date);
			open.push_back(_open);
			high.push_back(_high);
			low.push_back(_low);
			close.push_back(_close);
			adj_close.push_back(_adj_close);
			volume.push_back(_volume);
			++valid_size;
		    }
		}
		total_size = valid_size;
	    }
	    //for(vx_double e: {open, high, low, close, adj_close, volume}) { e.shrink_to_fit(); } /* this does nothing */
	    dates.shrink_to_fit(); open.shrink_to_fit();
	    high.shrink_to_fit(); low.shrink_to_fit(); 
	    close.shrink_to_fit(); adj_close.shrink_to_fit(); 
	    volume.shrink_to_fit();
	    _lines.clear();
	    if(total_size < 5) { std::__throw_length_error("Error: __exparser() [Valid line count is too low!]"); }
	}
	inline constexpr vx_string   get_lines() const noexcept(false) { return this->_lines;      }
	inline constexpr std::size_t get_size()  const noexcept(false) { return this->total_size;  }
	// tuning section
	inline constexpr vx_string get_dates()  const noexcept(false) {  return dates;  }
	inline constexpr vx_double get_open()   const noexcept(false) {  return open;   }
	inline constexpr vx_double get_high()        const noexcept(false) {  return high;      }
	inline constexpr vx_double get_low()         const noexcept(false) {  return low;       }
	inline constexpr vx_double get_adj_close()   const noexcept(false) {  return adj_close; }
	inline constexpr vx_double get_close()       const noexcept(false) {  return close;     }
	inline constexpr vx_double get_volume()      const noexcept(false) {  return volume;    }
	
	inline void show() const noexcept(false) {
	    printf("\x1b[95mLines (%ld)\x1b[0m"
		   "\x1b[92m%s [%25s]:\x1b[0m\n", total_size, "Last FIVE Entries for ", _file_name.c_str());
	    for(std::size_t i=total_size-5; i<total_size; ++i) {
		long long int const vd = static_cast<long long int>(volume[i]);
		std::fprintf(stderr,
			     "\x1b[93m[%s]\t%f\t%f\t%f\t%f\t%f\t\t%lld\x1b[0m\n", dates[i].c_str(),
			     open[i], high[i], low[i], close[i], adj_close[i], vd);
	    }
	}
	inline virtual ~__exparser() {
//			dates.clear(); open.clear(); high.clear(); low.clear(); adj_close.clear(); volume.clear();
	}
    };
}

namespace express {
	//signature: express_parser(file_name, delimiter)
	using parser = __express::__exparser<double, std::string, std::vector, gretel::get, reader::reader, reader::split_string>;
}


#endif // EXPARSER_HPP
