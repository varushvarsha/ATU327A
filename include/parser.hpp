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

#ifndef __PARSER_HPP
#define __PARSER_HPP  1

/*** this is deprecated ***/
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include "gretel.hpp"
#include "reader.hpp"

namespace __parser {
	template <typename __tp_double, typename __tp_string, typename __tp_v_string,
	      typename __tp_v_double, typename __tp_gretel, typename __tp_ifstream>
	struct __parser {
    public:
	using double_type   = __tp_double;
	using string_type   = __tp_string;
	using vector_string = __tp_v_string;
	using vector_double = __tp_v_double;
    private:
        string_type w_string;
    protected:
        int total_size;
        vector_string dates;
        vector_double open;
        vector_double high;
        vector_double low;
        vector_double unadj_close;
        vector_double close;
        vector_double volume;
    public:
    inline __parser() = default;
    inline __parser(string_type const &constring) noexcept(false): w_string(constring) {
	    std::for_each(w_string.begin(), w_string.end(), [](char &xIn){ xIn = std::toupper(xIn); });
	    const string_type file_path = "./CSVD/"+w_string+".csv"; /* modify this to direct the path where csv files are stored! */
	    __tp_ifstream op(file_path, std::ios::in);
	    if(!op.is_open()) {
		/* try to download the file */
			const string_type err_string = "ERROR: File [" + w_string + "] Could NOT be found!";
			try {
				printf("%s\n", err_string.c_str());
				printf("\x1b[96mTrying to download [%s]... \x1b[0m", w_string.c_str());
				__tp_gretel writer(w_string);
				writer.execute();
				printf("\x1b[92mFile [%s] downloaded successfully!\x1b[0m\n", w_string.c_str());
				op = __tp_ifstream(file_path, std::ios::in);
			} catch(std::exception const &ex) {
				puts(ex.what());
				throw ex;
			}
		}
	    
	    int counter = 0;
	    while(op.good()) {
		if(counter != 0) {
			string_type line;
			std::getline(op, line, '\n');
			if(line.size() > 5) {
				vector_string ops = split_string(line, ',');
				if((std::atof(ops[1].c_str()) != 0) && (std::atof(ops[5].c_str()) != 0)) {
					dates.emplace_back(                 ops[0]         );
					open.emplace_back(        std::atof(ops[1].c_str()));
					high.emplace_back(        std::atof(ops[2].c_str()));
					low.emplace_back(         std::atof(ops[3].c_str()));
					unadj_close.emplace_back( std::atof(ops[4].c_str()));
					close.emplace_back(       std::atof(ops[5].c_str()));
					volume.emplace_back(      std::atof(ops[6].c_str()));
				}
			}
		} ++counter;
		}
		op.close();

	    dates.shrink_to_fit(); open.shrink_to_fit(); high.shrink_to_fit(); low.shrink_to_fit();
	    close.shrink_to_fit(); volume.shrink_to_fit(); unadj_close.shrink_to_fit();
	    total_size = static_cast<int>(dates.size());
	    if(total_size == 0) {
		const string_type err_string = "ERROR: File [" + w_string + " ] Size is ZERO!";
		std::__throw_domain_error(err_string.c_str());
	    }
	}
	
	inline __parser(__parser const &obj) = default;
	inline __parser(__parser      &&obj) = default;

	inline int get_length() const noexcept(false) { return static_cast<int>(this->total_size); }

	inline vector_string split_string(string_type line, char delimiter) const noexcept(false) {
	    // specific to the constructor, otherwise pointless
	    vector_string vp;
	    string_type feedback;
	    std::istringstream sStream(line);
	    while(std::getline(sStream, feedback, delimiter)) { vp.emplace_back(feedback); }
	    return vp;
	}
    
    inline vector_string get_dates()  const {  return dates;  }
    inline vector_double get_open()        const {  return open;   }
    inline vector_double get_high()        const {  return high;   }
    inline vector_double get_low()         const {  return low;    }
    inline vector_double get_UNAdjclose()  const {  return unadj_close; }
    inline vector_double get_close()       const {  return close;  }
    inline vector_double get_volume()      const {  return volume; }

	inline vector_double get_normalized_open() const {
	    const double_type divider = open.at(0) == 0.0 ? 1.0 : open.at(0);
	    vector_double normalized_open;
	    std::for_each(open.begin(), open.end(), [&normalized_open, &divider](const double_type &di){
							normalized_open.emplace_back(di/divider);
						    });
	    return normalized_open;
	}
	inline vector_double get_normalized_high() const {
	    const double_type divider = high.at(0) == 0.0 ? 1.0 : high.at(0);
	    vector_double normalized_high;
	    std::for_each(high.begin(), high.end(), [&normalized_high, &divider](const double_type &di){
							normalized_high.emplace_back(di/divider);
						    });
	    return normalized_high;
	}
	inline vector_double get_normalized_low() const {
	    const double_type divider = low.at(0) == 0.0 ? 1.0 : low.at(0);
	    vector_double normalized_low;
	    std::for_each(low.begin(), low.end(), [&normalized_low, &divider](const double_type &di){
						      normalized_low.emplace_back(di/divider);
						  });
	    return normalized_low;
	}
	inline vector_double get_normalized_UNAdjClose() const {
	    const double_type divider = unadj_close.at(0) == 0.0 ? 1.0 : unadj_close.at(0);
	    vector_double normalized_unadj_close;
	    std::for_each(unadj_close.begin(), unadj_close.end(), [&normalized_unadj_close, &divider](const double_type &di){
								      normalized_unadj_close.emplace_back(di/divider);
								  });
	    return normalized_unadj_close;
	}
	inline vector_double get_normalized_close() const {
	    const double_type divider = close.at(0) == 0.0 ? 1.0 : close.at(0);
	    vector_double normalized_close;
	    std::for_each(close.begin(), close.end(), [&normalized_close, &divider](const double_type &di){
							  normalized_close.emplace_back(di/divider);
						      });
	    return normalized_close;	
	}
	inline vector_double get_normalized_volume() const {
	    const double_type divider = volume.at(0) == 0.0 ? 1.0 : volume.at(0);
	    vector_double normalized_volume;
	    std::for_each(volume.begin(), volume.end(), [&normalized_volume, &divider](const double_type &di){
							    normalized_volume.emplace_back(di/divider);
							});
	    return normalized_volume;	
	}

	inline int get_years() const noexcept(false) {
	    const string_type year_x  = string_type(dates.front());
	    const string_type year_y  = string_type(dates.back());
	    string_type n, m;
	    for(std::size_t i=0; i<4; ++i) { n.push_back(year_x[i]); m.push_back(year_y[i]); }
	    const int years = std::atof(m.c_str()) - std::atof(n.c_str());
	    return years;
	}	

	inline void show() const noexcept(false) {
	    printf("\x1b[92m%s [%25s]:\x1b[0m\n", "Last FIVE Entries for ", this->w_string.c_str());
	    for(std::size_t i=open.size()-5; i<open.size(); ++i) {
			long long int const vd = static_cast<long long int>(volume[i]);
			printf("\x1b[93m[%s]\t%f\t%f\t%f\t%f\t%f\t\t%lld\x1b[0m\n", dates[i].c_str(), open[i], high[i], low[i], unadj_close[i], close[i], vd);
	    }
	}
	
	inline double_type last_price() const noexcept(false) { return this->close.back(); }
	inline string_type last_date()  const noexcept(false) { return this->dates.back(); }
	inline virtual ~__parser() noexcept(false) {
//	    dates.clear(); dates.shrink_to_fit();
//	    open.clear(); open.shrink_to_fit();
//	    high.clear(); high.shrink_to_fit();
//	    low.clear(); low.shrink_to_fit();
//	    close.clear(); close.shrink_to_fit();
//	    unadj_close.clear(); unadj_close.shrink_to_fit();
//	    volume.clear(); volume.shrink_to_fit();
	}
    };
}


namespace parser {
    using parse = __parser::__parser<double, std::string, std::vector<std::string>, std::vector<double>, gretel::get, std::ifstream>;
}

#endif // __PARSER_HPP
