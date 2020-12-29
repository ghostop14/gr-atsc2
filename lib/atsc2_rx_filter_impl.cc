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
#include "atsc2_rx_filter_impl.h"
#include <gnuradio/filter/firdes.h>

namespace gr {
namespace atsc2 {

atsc2_rx_filter::sptr atsc2_rx_filter::make(float rate, float sps)
{
	return gnuradio::make_block_sptr<atsc2_rx_filter_impl>(rate, sps);
}

/*
 * The private constructor
 */
atsc2_rx_filter_impl::atsc2_rx_filter_impl(float input_rate,float sps)
: gr::hier_block2("atsc2_rx_filter",
		gr::io_signature::make(1, 1, sizeof(gr_complex)),
		gr::io_signature::make(1, 1, sizeof(gr_complex)))
{
	nfilts = 16;
	output_rate = ATSC_SYMBOL_RATE*sps; // # Desired oversampled sample rate
	filter_rate = input_rate*(float)nfilts;
	symbol_rate = ATSC_SYMBOL_RATE/2.0; // # One-sided bandwidth of sideband
	excess_bw = 0.1152;     // #1.0-(0.5*ATSC_SYMBOL_RATE/ATSC_CHANNEL_BW) // # ~10.3%
	ntaps = int((2*ATSC_RRC_SYMS+1)*sps*nfilts);
	interp = output_rate/input_rate;
	gain = nfilts*symbol_rate/filter_rate;
	rrc_taps = gr::filter::firdes::root_raised_cosine(gain,             //# Filter gain
			filter_rate,      //# PFB filter prototype rate
			symbol_rate,      //# ATSC symbol rate
			excess_bw,        //# ATSC RRC excess bandwidth
			ntaps);            //# Length of filter

	// This get_initial_sptr is what gets called in make()
	pfb = gnuradio::get_initial_sptr(new gr::filter::pfb_arb_resampler_ccf_impl(interp, rrc_taps, nfilts));

	// # Connect pipeline
	// self.connect(self, pfb, self)

	// Default heirarchical code
	connect(self(), 0, pfb, 0);
	// connect other blocks
	connect(pfb, 0, self(), 0);
}

bool atsc2_rx_filter_impl::stop() {
	/*
    	if (pfb) {
    		delete pfb;
    		pfb = NULL;
    	}
	 */
	return true;
}
/*
 * Our virtual destructor.
 */
atsc2_rx_filter_impl::~atsc2_rx_filter_impl()
{
	stop();
}


} /* namespace atsc2 */
} /* namespace gr */

