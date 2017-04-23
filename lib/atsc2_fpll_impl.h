/* -*- c++ -*- */
/*
 * Copyright 2014 Free Software Foundation, Inc.
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

#ifndef INCLUDED_DTV_atsc2_fpll_impl_H
#define INCLUDED_DTV_atsc2_fpll_impl_H

#include <gnuradio/nco.h>
#include <gnuradio/filter/single_pole_iir.h>
#include <gnuradio/analog/agc.h>
#include <stdio.h>
#include "../include/atsc2/atsc2_fpll.h"

namespace gr {
  namespace atsc2 {

    class atsc2_fpll_impl : public atsc2_fpll,protected gr::nco<float,float>
    {
    private:
      // gr::nco<float,float> d_nco;
      gr::filter::single_pole_iir<gr_complex,gr_complex,float> d_afc;

      float limit=M_PI/2.0;
      float alpha = 0.01;
      float beta = alpha*alpha/4.0;

    public:
      atsc2_fpll_impl(float rate);
      ~atsc2_fpll_impl();

      virtual int work(int noutput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } /* namespace dtv */
} /* namespace gr */

#endif /* INCLUDED_DTV_atsc2_fpll_impl_H */
