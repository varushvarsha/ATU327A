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

#include "hansel_and_gretel.hpp"

int main(int argc, char* argv[]) {
    std::string const file_name = "DWARKESH.csv";
    println(file_name);
    
    entry_guard(2, "No arguments are given. Either "
    "\n$ ./main AMZN XOM AAPL CVX <...>\nOR \n$ ./main resources/nyse.csv\n");

    agr::parsing_args mgr(argc, argv);
    std::vector<std::string> const instrument_names = mgr.get_instrument_names();
    std::size_t const length = instrument_names.size();

    // count the cores
    std::size_t const      cores = std::thread::hardware_concurrency();
    std::size_t const count_size = 16*cores;
    std::vector<std::thread> feedbook;

    for(std::size_t i=0; i<length; ++i) {
        if((i+1) % count_size == 0) {
            // join
            std::for_each(feedbook.begin(), feedbook.end(), std::mem_fn(&std::thread::join));
            feedbook.clear();
        }
        // load
        feedbook.push_back(std::thread([=](){
	    // println(instrument_names.at(i), "loaded");
					   printf("\r\x1b[93m%12ld\x1b[0m loading \x1b[94m%s\x1b[0m", i, instrument_names.at(i).c_str());
					   std::fflush(stdout);
					   try {
					       gretel::get w(instrument_names.at(i));
					       w.execute();
					   } catch(std::exception const &ex) {
					       println("\x1b[91mEXCEPTION\x1b[0m", instrument_names.at(i), ex.what());
					   }
				       }));
    }
    // remainder/low load
    if(feedbook.size() != 0) {
        std::for_each(feedbook.begin(), feedbook.end(), std::mem_fn(&std::thread::join));
    }
    
    // display what has been downloaded    
    for(std::string const &e: instrument_names) {
	std::fflush(stdout);
	std::fflush(stderr);
	try {
	    express::parser omega("./CSVD/"+e+".csv");	    
	    omega.show();
	} catch(std::exception const &ex) {
	    println("Error: ", e, ex.what());
	}
    }


    return 0;
}
