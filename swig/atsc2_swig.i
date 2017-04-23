/* -*- c++ -*- */

#define ATSC2_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "atsc2_swig_doc.i"

%{
#include "atsc2/atsc2_rx_filter.h"
#include "atsc2/atsc2_fpll.h"
#include "atsc2/tcp_sink.h"
%}


%include "atsc2/atsc2_rx_filter.h"
GR_SWIG_BLOCK_MAGIC2(atsc2, atsc2_rx_filter);

%include "atsc2/atsc2_fpll.h"
GR_SWIG_BLOCK_MAGIC2(atsc2, atsc2_fpll);

%include "atsc2/tcp_sink.h"
GR_SWIG_BLOCK_MAGIC2(atsc2, tcp_sink);
