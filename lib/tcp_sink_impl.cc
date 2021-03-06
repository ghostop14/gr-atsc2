/* -*- c++ -*- */
/* 
 * Copyright 2017 ghostop14.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "tcp_sink_impl.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;

namespace gr {
namespace atsc2 {

tcp_sink::sptr tcp_sink::make(int port)
{
	return gnuradio::make_block_sptr<tcp_sink_impl>(port);
}

/*
 * The private constructor
 */
tcp_sink_impl::tcp_sink_impl(int port,bool sendDebugContentType)
: gr::sync_block("ts_sink",
		gr::io_signature::make(1, 1, sizeof(char)),
		gr::io_signature::make(0, 0, 0)),bSendDebugContentType(sendDebugContentType),
		d_itemsize(1), d_veclen(1),d_port(port) // ,d_acceptor(d_io_service)
{
	d_block_size = 1;

	connect(true);
}

size_t tcp_sink_impl::netDataAvailable() {
	// Get amount of data available
	boost::asio::socket_base::bytes_readable command(true);
	tcpsocket->io_control(command);
	size_t bytes_readable = command.get();

	return bytes_readable;
}


void tcp_sink_impl::connect(bool initialConnection) {
	std::cout << "TCP Source waiting for connection on port " << d_port << std::endl;
	if (initialConnection) {
		/*
			d_acceptor.open(d_endpoint.protocol());
			d_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
			d_acceptor.bind(d_endpoint);
		 */
		d_acceptor = new boost::asio::ip::tcp::acceptor(d_io_service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),d_port));
	}
	else {
		d_io_service.reset();
	}

	// d_acceptor.listen();

	// switched to match boost example code with a copy/temp pointer
	if (tcpsocket) {
		delete tcpsocket;
	}
	tcpsocket = NULL; // new boost::asio::ip::tcp::socket(d_io_service);
	// This will block while waiting for a connection
	// d_acceptor.accept(*tcpsocket, d_endpoint);
	bConnected = false;


	// Good full example:
	// http://www.boost.org/doc/libs/1_36_0/doc/html/boost_asio/example/echo/async_tcp_echo_server.cpp
	// Boost tutorial
	// http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html

	boost::asio::ip::tcp::socket *tmpSocket = new boost::asio::ip::tcp::socket(d_io_service);
	d_acceptor->async_accept(*tmpSocket,
			boost::bind(&tcp_sink_impl::accept_handler, this, tmpSocket, // This will make a ptr copy // boost::ref(tcpsocket),  // << pass by reference
					boost::asio::placeholders::error));

	if (initialConnection) {
		d_io_service.run();
	}
	else {
		d_io_service.run();
	}

}

void tcp_sink_impl::SendInitialHTMLHeader() {
	int bytesAvailable=0;

	while (bytesAvailable==0) {
		bytesAvailable = netDataAvailable();
		sleep(1);
	}

	// We won't be here unless we get data.  The above loop WILL block.
	boost::asio::streambuf read_buffer;
	int bytesRead = boost::asio::read(*tcpsocket, read_buffer,boost::asio::transfer_exactly(bytesAvailable), ec);
	read_buffer.commit(bytesRead);

	const char *readData = boost::asio::buffer_cast<const char*>(read_buffer.data());

	std::string requestString = readData;

	if (requestString.find("GET") == std::string::npos) {
		// doesn't look like an HTML request.  Shut down.
		std::cout << "ERROR: Invalid connection.  Closing session with client." << std::endl;
		tcpsocket->close();
		delete tcpsocket;
		tcpsocket = NULL;
		connect(false);

		return;
	}

	ptime now = second_clock::universal_time();

	time_facet *facet = new time_facet("%a, %d %b %Y %H:%M:%S GMT");
	std::ostringstream stream;
	stream.imbue(std::locale(stream.getloc(), facet));
	stream << now; // dtnow;
	std::string dateString = stream.str();

	//delete facet;

	std::string htmlReply;

	std::string htmlReplyHeader="HTTP/1.1 200 OK\n";
	if (bSendDebugContentType) {
		htmlReplyHeader += "Content-Type: text/html; charset=iso-8859-1\n";
		htmlReply = "<HTML><BODY><P>This is a test page</P></BODY><HTML>\n";
		htmlReplyHeader += "Content-Length: " + std::to_string(htmlReply.length()) + "\n";
	}
	else
		htmlReplyHeader += "Content-Type: video/mp2t\n";

	htmlReplyHeader += "Connection: keep-alive\n";
	htmlReplyHeader += "Date: " + dateString + "\n"; // "Thu, 20 Apr 2017 11:21:43 GMT\n";
	htmlReplyHeader += "Cache-Control: no-store, no-cache, max-age=0\n";
	htmlReplyHeader += "Last-Modified: " + dateString + "\n"; // "Thu, 20 Apr 2017 10:51:53 GMT\n";
	htmlReplyHeader += "Server: Unknown\n";
	htmlReplyHeader += "ETag: \"" + generateETag() + "\"\n\n";

	if (bSendDebugContentType) {
		htmlReplyHeader += htmlReply;
	}


	ec.clear();

	int bytesWritten = boost::asio::write(*tcpsocket, boost::asio::buffer((const void *)htmlReplyHeader.c_str(), htmlReplyHeader.length()),ec);

	if (bytesWritten < htmlReplyHeader.length()) {
		std::cout << "Error writing HTML header before stream." << std::endl;
	}
}

void tcp_sink_impl::accept_handler(boost::asio::ip::tcp::socket * new_connection,
		const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "ATSC2 Streaming Server Connection established." << std::endl;
		// Accept succeeded.
		tcpsocket = new_connection;

		boost::asio::socket_base::keep_alive option(true);
		tcpsocket->set_option(option);
		bConnected = true;

		SendInitialHTMLHeader();
	}
	else {
		std::cout << "ATSC2 streaming server error code " << error << " accepting boost TCP session." << std::endl;

		// Boost made a copy so we have to clean up
		delete new_connection;

		// safety settings.
		bConnected = false;
		tcpsocket = NULL;
	}
}

std::string tcp_sink_impl::generateETag() {
	std::string retVal;

	int min=0;
	int max=9;

	unsigned randomNumber;

	for (int i=0;i<6;i++) {
		randomNumber = ( rand() % (max - min + 1) ) + min;
		retVal += std::to_string(randomNumber);
	}

	retVal += "-";

	for (int i=0;i<3;i++) {
		randomNumber = ( rand() % (max - min + 1) ) + min;
		retVal += std::to_string(randomNumber);
	}

	retVal += "-";

	for (int i=0;i<12;i++) {
		randomNumber = ( rand() % (max - min + 1) ) + min;
		retVal += std::to_string(randomNumber);
	}

	return retVal;
}
/*
 * Our virtual destructor.
 */
tcp_sink_impl::~tcp_sink_impl()
{
	stop();
}

bool tcp_sink_impl::stop() {
	if (tcpsocket) {
		tcpsocket->close();
		delete tcpsocket;
		tcpsocket = NULL;
	}

	d_io_service.reset();
	d_io_service.stop();

	if (d_acceptor) {
		delete d_acceptor;
		d_acceptor=NULL;
	}
	return true;
}

void tcp_sink_impl::sendBlankPage() {
	std::string htmlReply = "<HTML><BODY><P>This is a test page</P></BODY><HTML>\n";

	int bytesWritten = boost::asio::write(*tcpsocket, boost::asio::buffer((const void *)htmlReply.c_str(), htmlReply.length()),ec);

}
int
tcp_sink_impl::work_test(int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items)
{
	gr::thread::scoped_lock guard(d_mutex);

	// read was blocking
	// checkForDisconnect();

	const char *in = (const char *) input_items[0];
	unsigned int noi = noutput_items * d_block_size;
	int bytesWritten;
	int bytesRemaining = noi;
	// send it.
	// We could have done this async, however with guards and waits it has the same effect.
	// It just doesn't get detected till the next frame.
	// boost::asio::write(*tcpsocket, boost::asio::buffer(d_buf.get(), data_len),ec);
	ec.clear();

	while ((bytesRemaining > 0) && (!ec)) {
		bytesWritten = boost::asio::write(*tcpsocket, boost::asio::buffer((const void *)in, noi),ec);
		bytesRemaining -= bytesWritten;
	}

	// writes happen a lot faster then reads.  To the point where it's overflowing the receiving buffer.
	// So this delay is to purposefully slow it down.
	//usleep(10);

	return noutput_items;
}

void tcp_sink_impl::checkForDisconnect() {
	// See https://sourceforge.net/p/asio/mailman/message/29070473/

	int bytesRead;

	char buff[1];
	bytesRead = tcpsocket->receive(boost::asio::buffer(buff),tcpsocket->message_peek, ec);
	if ((boost::asio::error::eof == ec) ||(boost::asio::error::connection_reset == ec)) {
		std::cout << "Disconnect detected." << std::endl;
		tcpsocket->close();
		delete tcpsocket;
		tcpsocket = NULL;

		exit(1);
	}
	else {
		if (ec) {
			std::cout << "Socket error " << ec << " detected." << std::endl;
		}
	}
}


int
tcp_sink_impl::work(int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items)
{
	gr::thread::scoped_lock guard(d_mutex);

	if (!bConnected)
		return noutput_items;

	const char *in = (const char *) input_items[0];
	unsigned int noi = noutput_items * d_block_size;
	int bytesWritten;
	int bytesRemaining = noi;
	// send it.
	// We could have done this async, however with guards and waits it has the same effect.
	// It just doesn't get detected till the next frame.
	// boost::asio::write(*tcpsocket, boost::asio::buffer(d_buf.get(), data_len),ec);
	ec.clear();

	while ((bytesRemaining > 0) && (!ec)) {
		bytesWritten = boost::asio::write(*tcpsocket, boost::asio::buffer((const void *)in, noi),ec);
		bytesRemaining -= bytesWritten;

		if (ec == boost::asio::error::connection_reset || ec == boost::asio::error::broken_pipe) {
			// see http://stackoverflow.com/questions/3857272/boost-error-codes-reference for boost error codes


			// Connection was reset
			bConnected = false;
			bytesRemaining = 0;

			std::cout << "ATSC2 streaming server client disconnected." << std::endl;
			connect(false);  // start waiting for another connection
		}

	}

	// writes happen a lot faster then reads.  To the point where it's overflowing the receiving buffer.
	// So this delay is to purposefully slow it down.
	//usleep(100);

	return noutput_items;
}
} /* namespace grnet */
} /* namespace gr */

