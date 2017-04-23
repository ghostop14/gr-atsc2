/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cppunit/TextTestRunner.h>
#include <cppunit/XmlOutputter.h>

#include <gnuradio/unittests.h>
#include "qa_atsc2.h"
#include <iostream>
#include <fstream>

#include "tcp_sink_impl.h"
#include <boost/algorithm/string/replace.hpp>

using namespace gr::atsc2;

int
main (int argc, char **argv)
{
	std::string host = "127.0.0.1";
	int port=0;

	if (argc > 1) {
		// 1 is the file name
		if (strcmp(argv[1],"--help")==0) {
			std::cout << std::endl;
			std::cout << "Usage: --port=<port>" << std::endl;
			std::cout << "Creates an atsc server listener on the specified port and sends the header with a blank page.";
			std::cout << std::endl;
			exit(0);
		}

		for (int i=1;i<argc;i++) {
			std::string param = argv[i];

			if (param.find("--port") != std::string::npos) {
				boost::replace_all(param,"--port=","");
				port=atoi(param.c_str());
			}

		}
	}

	if (port==0) {
		std::cout << "ERROR: Please specify a port." << std::endl;
		exit(1);
	}

	std::cout << "Testing atsc streaming server on port " << port << std::endl;

	tcp_sink_impl *testTCP=NULL;

	testTCP = new tcp_sink_impl(port,true);

	delete testTCP;

	std::cout << "Done." << std::endl;

	return 0;

}
