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


#ifndef __DEFINES_HPP
#define __DEFINES_HPP 1

/*** caution -- remove this header from production -- ***/
/* header for fast debugging utility, useless trivia and many other pointless things */

#include <cstdio>
#include <string>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <cmath>
#include <fstream>
#include <ostream>
#include <locale>
#include <utility>
#include <functional>
#include <iostream>

// #include <thread>
// #include <mutex>
// #include <future>
// #include <condition_variable>
// #include <atomic>

//freq used types
#define i_int std::size_t
#define d_int int

//mathematical constants
#define m_pi 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#define get_random(arg) ((decltype(arg))(rand()) % (arg)) //random number generation
template <typename __tp>
__attribute__((always_inline)) __tp* get_rarray(int const &__ln)  {
    __tp* arrax_ = new __tp[__ln];
    for(int i=0; i<__ln; ++i) {
	arrax_[i] = (__tp)(get_random(161));
    }
    return arrax_;
}

//C++17, C++20 et el
#define constant constexpr thread_local
#define unuse_suppressor [[maybe_unused]]
#define instantaneous inline static constexpr
#define rapid_claused inline constexpr
#define forcing_inline __attribute__((always_inline))
#define cstring  const char*
#define xstring constexpr const char*

//casts
#define as_double(arg) static_cast<double>(arg)
#define as_int(arg)    static_cast<int>(arg)
#define as_size_t(arg) static_cast<std::size_t>(arg)
#define as_long(arg)   static_cast<long>(arg)

// loops 
#define __for_loop_statement(__ln) for(std::size_t i=0; i<__ln; ++i)
#define __for_k(__ln) for(std::size_t k=0; k<__ln; ++k)
#define __for_i(__ln) for(std::size_t i=0; i<__ln; ++i)
#define __for_j(__ln) for(std::size_t j=0; j<__ln; ++j)

#define execute_for(arg, N) __for_##arg(N)

//input values
#define guard(info) if(argc == 1) { printf("\x1b[95m%s\x1b[0m\n", info); exit(1); }
#define arg_v(k) (int)std::strtod(argv[k], NULL);
#define entry_guard(__ln, __info) if(argc < __ln) { printf("\x1b[95m%s\x1b[0m\n", __info); exit(1); }

// error handling and exceptions
#define __invalid_arg_error(arg) std::__throw_invalid_argument(arg)
#define error_throw(invalid, arg) __##invalid_arg_error(arg)


// TMP
// TMP[OLD]
#define expression_type template <typename> typename
// TMP[OLD/]
#define siexpressn_type template <typename> typename
#define duexpressn_type template <typename, typename> typename
#define trexpressn_type template <typename, typename, typename> typename
#define vrexpressn_type template <typename... > typename
#define definition_type typename
#define configuring     template

//[BEGIN]debugging macros 
#define echo_line std::puts("---------------------------------------------------------------------------------------");
#define print_new_line std::putc('\n', stdout)
#define print(arg) puts(std::string(arg).c_str())
#define print_msg_ld(arg1, arg2) std::fprintf(stdout, "%s ", arg1.c_str(), "%ld\n", arg2)
//[END]  debugging macros [not-to-be-included in production code]

// a handy printing(debug) facility
namespace qrint {
    configuring <definition_type __tp_ostream>
    struct __print {
    public:
    	using ostream_type  = __tp_ostream;
    public:	
    	configuring <definition_type __tp>
    	instantaneous int println(ostream_type &_os, __tp const &h) {
    	    _os << "[\x1b[94;1m" << h << "\x1b[0m]\n";
    	    return 0;
    	}
    	configuring <definition_type __tp, definition_type ... __tp_args>
    	instantaneous int println(ostream_type &_os, __tp const &h, __tp_args const& ...args) {
    	    _os << "[\x1b[95;1m" << h << "\x1b[0m] ";
    	    return println(_os, args...);
    	}
    };
    
    // base case overload /* this is due to Peter Sommerlad. */
    void println(std::ostream &out){ out <<'\n'; }
    // variadic configuring
    configuring <typename HEAD, typename ... T>
    void println(std::ostream & out,HEAD const &h, T const & ... tail){
        out << h; // cut off head
        if (sizeof...(tail)){
            out <<", ";
        }
        println(out,tail...); // recurse on tail...
    }
}

using _writeln = qrint::__print<std::basic_ostream<char>>; //useless trivia: the writeln is a printing function in chapel language

// a trivial wrapper to invoke
template <typename ... __tp_args>
inline constexpr int writeln(__tp_args const &... args) { return _writeln::println(std::cout, args...); }

namespace {
    // another handy printing(debug) facility
    struct __println {
        template <typename __tp_stream, typename __tp>
        inline static constexpr int println(__tp_stream &stream, __tp const &tp) {
            stream << "[\x1b[94;1m" << tp << "\x1b[0m]\n";
            return 0;
        }
        template <typename __tp_stream, typename __tp, typename ... __tp_args>
        inline static constexpr int println(__tp_stream &stream, __tp const &tp, __tp_args const &...args) {
            stream << "[\x1b[95;1m" << tp << "\x1b[0m] ";
            return println(stream, args...);
        }
    };
    
    // another trivial wrappers to invoke
    template <typename ... __tp_args>
    inline constexpr int println(__tp_args const &... args) {
        return __println::println(std::cout, args...);
    }
    
    unuse_suppressor inline constexpr int println() { // the curious case of asymptotically divergent case
        return __println::println(std::cout, "\n");
    }    
}



#endif // __DEFINES_HPP
