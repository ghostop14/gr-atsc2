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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "atsc2_fpll_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <gnuradio/sincos.h>

#define CL_TWO_PI 6.28318530717958647692

namespace gr {
  namespace atsc2 {

    atsc2_fpll::sptr
    atsc2_fpll::make(float rate)
    {
      return gnuradio::get_initial_sptr
	(new atsc2_fpll_impl(rate));
    }

    atsc2_fpll_impl::atsc2_fpll_impl(float rate)
      : sync_block("dtv_atsc2_fpll",
                   io_signature::make(1, 1, sizeof(gr_complex)),
                   io_signature::make(1, 1, sizeof(float)))
    {
      d_afc.set_taps(1.0-exp(-1.0/rate/5e-6));
      //d_nco.set_freq((-3e6 + 0.309e6)/rate*2*M_PI);
      //d_nco.set_phase(0.0);
      set_freq((-3e6 + 0.309e6)/rate*2*M_PI);
      set_phase(0.0);
    }

    atsc2_fpll_impl::~atsc2_fpll_impl()
    {
    }

    int
    atsc2_fpll_impl::work(int noutput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *)input_items[0];
      float *out = (float *)output_items[0];

      float i_r,i_i,n_r,n_i,o_r,o_i;
//      float a_cos, a_sin;
      gr_complex result;
      gr_complex filtered;
      float x;

      for (int k = 0; k < noutput_items; k++) {
        // d_nco.step();                  // increment phase
		  phase += phase_inc;
		  if(fabs(phase) > M_PI) {
			while(phase > M_PI)
			  phase -= CL_TWO_PI;

			while(phase < -M_PI)
			  phase += CL_TWO_PI;
		  }

        // d_nco.sincos(&a_sin, &a_cos);  // compute cos and sin
		// Note: these assignments are OPPOSITE a Costas Loop
		n_r = sinf(phase);
		n_i = cosf(phase);

        // Mix out carrier and output I-only signal

        //result = in[k]*gr_complex(a_sin, a_cos);  // gr_complex(real,imag)
        //out[k] = result.real();

        i_r = in[k].real();
        i_i = in[k].imag();
        o_r = (i_r * n_r) - (i_i*n_i);
        o_i = (i_r * n_i) + (i_i * n_r);
        out[k] = o_r;
        result.real(o_r);
        result.imag(o_i);

        // Update phase/freq error
        filtered = d_afc.filter(result);

        // fast_atan2f does too much to flatten
        x = gr::fast_atan2f(filtered.imag(), filtered.real());

        // avoid slamming filter with big transitions
        if (x > limit)
          x = limit;
        else if (x < -limit)
          x = -limit;

        //adjust_phase(alpha*x);
        phase += alpha*x;
        // adjust_freq(beta*x);
        phase_inc += beta*x;
      }

      return noutput_items;
    }

  } /* namespace dtv */
} /* namespace gr */
