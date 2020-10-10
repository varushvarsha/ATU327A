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

#ifndef __GRETEL_HPP
#define __GRETEL_HPP  1


#include "hansel.hpp"
#include <cstdio>
#include <ctime>
#include <algorithm>

namespace __gretel {
	namespace m_internal {
		template <typename __tp_string>
		inline  __tp_string capitalize(__tp_string const &_in_string) {
			__tp_string in_string(_in_string);
			std::for_each(in_string.begin(), in_string.end(), [](char &e){
				e = ((e < 'a') || (e > 'z')) ? e : e - ('a' - 'A');
			});
			return in_string;
		}

		template <typename __tp>
		struct __check_file_path { // checks the trailing character
			using _string_type = __tp;
			_string_type _in_path;
			inline  constexpr __check_file_path(_string_type const &in_path): _in_path(in_path) {}
			inline  constexpr bool value() const noexcept(false) { return _in_path[_in_path.size()-1] == '/'; }
		};

		template<typename __tp>
		using check_file_path = __check_file_path<__tp>;
	}

	template <typename __tp, typename __tq/*hansel::generate_cookie_crumb*/>
	struct __download {
	public:
		using string_type  = __tp;
		using gccrumb_type = __tq;
	protected:
		CURL *curl;
		FILE *_op_file;
	private:
		static std::size_t _disk_writer(void *contents, std::size_t size, std::size_t nmemb, void *userp) {
			std::fprintf(reinterpret_cast<FILE*>(userp), "%.*s",
			             static_cast<int>(size*nmemb), reinterpret_cast<char*>(contents));
			return size*nmemb;
		}
		string_type    _i_name;
		bool           _write_direct;
		string_type    _op_file_path;
		bool           _display_speed;		
		gccrumb_type   _i_gccrumb;
		string_type    _op_csv_string;
	public:
		inline  constexpr explicit __download(string_type const &instrument_name,
		                                      bool const &write_direct=true,
		                                      string_type const &op_file_path="./",
		                                      bool const &display_speed=true):
		    _i_name(m_internal::capitalize(instrument_name)),
		    _write_direct(write_direct),
		    _op_file_path(op_file_path),
		    _display_speed(display_speed),		    
		    _i_gccrumb(gccrumb_type(_i_name)),
		    _op_csv_string("")
		{
			string_type const _main_url   = _construct_url();
			string_type const _req_cookie = "B="+_i_gccrumb.get_cookie_value();

			m_internal::check_file_path<string_type> delm_check(_op_file_path);
			string_type const opfile_path = delm_check.value() ? _op_file_path : _op_file_path + "/";			
			string_type const   file_path = opfile_path == "./" ? "./CSVD/" + instrument_name + ".csv" :
			                                                      opfile_path + instrument_name + ".csv";

			curl_global_init(CURL_GLOBAL_ALL);
			curl = curl_easy_init();
			curl_easy_setopt(curl, CURLOPT_URL, _main_url.c_str());
			curl_easy_setopt(curl, CURLOPT_COOKIE, _req_cookie.c_str());
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			_op_file = std::fopen(file_path.c_str(), "w");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _disk_writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, _op_file);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, _display_speed ? 0L : 1L);
		}

		inline  constexpr string_type _construct_url() const noexcept(false) {
			std::time_t const  res             = std::time(nullptr);
			string_type const  crumb_value     = string_type(_i_gccrumb.get_crumb_value());
			string_type const  main_url        = "https://query1.finance.yahoo.com/v7/finance/download/"
			                                    +_i_name
			                                    +"?period1=0&period2="+std::to_string(res)
			                                    +"&interval=1d&events=history&crumb="+crumb_value;
			return main_url;
		}

		inline  constexpr void execute() const noexcept(false) {
			CURLcode response = curl_easy_perform(curl);
			if(response != CURLE_OK) {
				string_type const err_string = "Error: __download() " + string_type(curl_easy_strerror(response));
				std::__throw_invalid_argument(err_string.c_str());
			}
		}

		inline virtual ~__download() {
			curl_easy_cleanup(curl); curl_global_cleanup();
			if(_write_direct) { std::fclose(_op_file); }
		}
	};
    
	using download = __download<std::string, hansel::generate_cookie_crumb>; //download shorthand
}

namespace gretel {
	using get = __gretel::download;
}



#endif // GRETEL_HPP
