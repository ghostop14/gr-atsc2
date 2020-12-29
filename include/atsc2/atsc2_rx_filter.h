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


#ifndef INCLUDED_ATSC2_ATSC2_RX_FILTER_H
#define INCLUDED_ATSC2_ATSC2_RX_FILTER_H

#include <atsc2/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
  namespace atsc2 {

    /*!
     * \brief <+description of block+>
     * \ingroup atsc2
     *
     */
    class ATSC2_API atsc2_rx_filter : virtual public gr::hier_block2
    {
     public:
      typedef std::shared_ptr<atsc2_rx_filter> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of atsc2::atsc2_rx_filter.
       *
       * To avoid accidental use of raw pointers, atsc2::atsc2_rx_filter's
       * constructor is in a private implementation
       * class. atsc2::atsc2_rx_filter::make is the public interface for
       * creating new instances.
       */
      static sptr make(float rate,float sps);
    };

  } // namespace atsc2
} // namespace gr

#endif /* INCLUDED_ATSC2_ATSC2_RX_FILTER_H */

