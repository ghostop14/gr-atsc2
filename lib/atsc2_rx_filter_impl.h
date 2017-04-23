/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_ATSC2_ATSC2_RX_FILTER_IMPL_H
#define INCLUDED_ATSC2_ATSC2_RX_FILTER_IMPL_H

#include <atsc2/atsc2_rx_filter.h>
#include "pfb_arb_resampler_ccf_impl.h"

//#define ATSC_CHANNEL_BW 6.0e6
#define ATSC_CHANNEL_BW 6000000.0
// # ~10.76 Mbaud
// #define ATSC_SYMBOL_RATE 4.5e6/286*684
#define ATSC_SYMBOL_RATE 4500000/286*684
// # filter kernel extends over 2N+1 symbols
#define ATSC_RRC_SYMS 8

namespace gr {
  namespace atsc2 {

    class atsc2_rx_filter_impl : public atsc2_rx_filter
    {
     private:
        int nfilts;
        double output_rate; // # Desired oversampled sample rate
        double gain;
        double filter_rate;
        double symbol_rate; // # One-sided bandwidth of sideband
        double excess_bw;     // #1.0-(0.5*ATSC_SYMBOL_RATE/ATSC_CHANNEL_BW) // # ~10.3%
        int ntaps;

        double interp;
        std::vector<float> rrc_taps;

        // boost::shared_ptr<gr::basic_block> *pfb;
        boost::shared_ptr<gr::filter::pfb_arb_resampler_ccf_impl> pfb;

     public:
      bool stop();

      atsc2_rx_filter_impl(float input_rate,float sps);
      ~atsc2_rx_filter_impl();

      // Where all the action really happens
    };

  } // namespace atsc2
} // namespace gr

#endif /* INCLUDED_ATSC2_ATSC2_RX_FILTER_IMPL_H */

