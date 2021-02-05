/*
Copyright (C) 2021  Varush Varsha

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

#ifndef __HANSEL_HPP
#define __HANSEL_HPP  1


#include <curl/curl.h>
#include <iostream>

namespace __hansel {
    template <typename __tp> 
    struct __has_unicode {
    public:
        using string_type = __tp;
    private:
        string_type     _in_string;
        string_type     _op_string;
        bool            _ret_value;
    public:
        inline constexpr __has_unicode() = default;
        inline constexpr __has_unicode(string_type const &in_string): _in_string(in_string), _op_string(""), _ret_value(false) {
	    /*
	     *  refer to this construction which fails on raw string literals
	     *    std::string const _krumb = R"(z\u002f\u00df\u6c34\U0001f34c)";
	     *    std::stringstream ss_stream(_krumb);
	     *    ss_stream.imbue(std::locale("en_US.UTF-8"));
	     *    std::string const decoded_string = ss_stream.str();
	     */
            std::size_t i = 0; std::size_t k = 1;
            for(i=0; i<_in_string.size(); i=i+k) {
                char const e = _in_string.at(i);
                if(e == '\\') {
                    _op_string += '/';
                    _ret_value  = true;
                    k = 6;
                }
                else { _op_string += e; k = 1; }
            }
        }
        inline constexpr bool value() const noexcept(false) {
            return _ret_value;
        }
        inline constexpr string_type decode() const noexcept(false) {
            return this->_op_string;
        }
    };


    /*________________________________________________________________________________________________________*/

    template <typename __tp, typename __tp_unicode>
    struct __get_web_page_op_cookie_string {
    public:
        using string_type        =  __tp;
        using unicode_type       =  __tp_unicode;
        typedef struct curl_slist s_curl_slist;
    private:
        static std::size_t _string_feeder(void *contents, std::size_t size, std::size_t nmemb, void *userp) {
            reinterpret_cast<std::string*>(userp)->append(reinterpret_cast<char*>(contents), size*nmemb);
            return size*nmemb;
        }
        string_type      _instrument_name;
        string_type      _op_buffer;
        string_type      _cookie;
        string_type      _crumb;
    protected:
        CURL *curl;
        CURLcode response;
        s_curl_slist *cookies;
        s_curl_slist *nc;
    public:
        inline constexpr __get_web_page_op_cookie_string() = delete;
        inline constexpr __get_web_page_op_cookie_string(string_type const &url): _instrument_name(url),
										  _op_buffer(""),
										  _cookie(""),
										  _crumb("") {
            /* N.B.
	     * it is imperative to do all this stuff in this single constructor;
	     * as attempts to modularize the code would result in callback complexities
	     * when used in threaded application.
	     *
	     * For an engaging discussion on this problem and its prescribed solution as boost.fiber
	     * by Nat Goodspeed, please refer to his (https://www.youtube.com/watch?v=e-NUmyBou8Q)
	     * intresting presentation on this subject. */

            string_type const web_url = "https://in.finance.yahoo.com/quote/"+_instrument_name+"?p="+_instrument_name;
            string_type cookie;

            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, web_url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _string_feeder);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_op_buffer);
            curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

            curl_easy_perform(curl);
            response = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
            if(response != CURLE_OK) {
		unload();
                string_type const err_string = "Error: __get_web_page_op_cookie_string() [" +
                                               string_type(curl_easy_strerror(response)) + "]";
                std::__throw_invalid_argument(err_string.c_str());
            }

            nc = cookies;

            while(nc) {
                char *w = nc->data;
                cookie  = w;
                nc      = nc->next;
                if(cookie.size() != 0) { break; }
            }

            cookie.replace(0, 37, "");
            _cookie = cookie;

            string_type const web_string = _op_buffer;
            std::size_t inner_index = 0;
            for(std::size_t i=500000; i<web_string.size()-55; ++i) { /* couldn't get satisfactory results with std::regex on threading */
                if((web_string.at(i) == '}') && (web_string.at(i+1) == '}') && (web_string.at(i+2) == ',') &&
                        (web_string.at(i+3) == '"') && (web_string.at(i+4) == 'C') && (web_string.at(i+5) == 'r') &&
                        (web_string.at(i+6) == 'u') &&
                        (web_string.at(i+12) == 'r') && (web_string.at(i+15) == ':') &&
                        (web_string.at(i+16) == '{') && (web_string.at(i+25) == '"')) {
                    inner_index = i+26;
                    break;
                }
            }

            if(inner_index == 0) {
		unload();
		std::__throw_logic_error("Error: Not parsed correctly");
	    }

            string_type _krumb = "";
            for(std::size_t i=inner_index; i<web_string.size(); ++i) {
                char const e = web_string.at(i);
                if(e != '"') {
                    _krumb += e;
                } else { break; }
            }
            unicode_type krumb(_krumb);
            _crumb = krumb.decode();            
        }
        inline constexpr string_type get_cookie()      const noexcept(false) { return this->_cookie; }
        inline constexpr string_type get_crumb()       const noexcept(false) { return this->_crumb;  }
        inline constexpr string_type _get_op_buffer()  const noexcept(false) { return this->_op_buffer; }
        //for comapatibilities
        inline constexpr string_type get_cookie_value() const noexcept(false) { return this->get_cookie(); }
        inline constexpr string_type get_crumb_value()  const noexcept(false) { return this->get_crumb();  }
	inline constexpr void unload() {
	    curl_slist_free_all(cookies);
	    if(nc) { curl_slist_free_all(nc); }
            curl_easy_cleanup(curl);
	    curl_global_cleanup();	    
	}	
	inline virtual ~__get_web_page_op_cookie_string() {
	    unload();
	}
    };
}

namespace hansel {
    using has_unicode           = __hansel::__has_unicode<std::string>; //has_unicode shorthand
    using page_cookie_string    = __hansel::__get_web_page_op_cookie_string<std::string, has_unicode>; // page_cookie_string shorthand

    using cookie_crumb_string   = hansel::page_cookie_string;
    using generate_cookie_crumb = hansel::page_cookie_string;
}


#endif // HANSEL_HPP
