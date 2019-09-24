#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Atsc2 Receive
# Generated: Tue Oct  2 11:43:18 2018
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import dtv
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import atsc2
import sip
import sys
import time
from gnuradio import qtgui


class atsc2_receive(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Atsc2 Receive")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Atsc2 Receive")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "atsc2_receive")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.tuning_offset = tuning_offset = 0
        self.center_freq = center_freq = 195e6
        self.vga_gain = vga_gain = 18
        self.radio_freq = radio_freq = center_freq+tuning_offset
        self.processing_rate = processing_rate = 6.25e6
        self.lna_gain = lna_gain = 14
        self.hw_sample_rate = hw_sample_rate = 6.25e6
        self.freq_min = freq_min = -90
        self.amp = amp = 0

        ##################################################
        # Blocks
        ##################################################
        self._lna_gain_range = Range(0, 21, 1, 14, 200)
        self._lna_gain_win = RangeWidget(self._lna_gain_range, self.set_lna_gain, 'LNA Gain', "counter_slider", float)
        self.top_grid_layout.addWidget(self._lna_gain_win, 0,3,1,1)
        self._freq_min_range = Range(-140, 70, 1, -90, 200)
        self._freq_min_win = RangeWidget(self._freq_min_range, self.set_freq_min, 'Low dB Display Cutoff', "counter_slider", float)
        self.top_grid_layout.addWidget(self._freq_min_win, 3,0,1,5)
        self._center_freq_tool_bar = Qt.QToolBar(self)
        self._center_freq_tool_bar.addWidget(Qt.QLabel('Center Frequency'+": "))
        self._center_freq_line_edit = Qt.QLineEdit(str(self.center_freq))
        self._center_freq_tool_bar.addWidget(self._center_freq_line_edit)
        self._center_freq_line_edit.returnPressed.connect(
        	lambda: self.set_center_freq(eng_notation.str_to_num(str(self._center_freq_line_edit.text().toAscii()))))
        self.top_grid_layout.addWidget(self._center_freq_tool_bar, 0,2,1,1)
        self.u = uhd.usrp_source(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.u.set_samp_rate(hw_sample_rate)
        self.u.set_center_freq(center_freq, 0)
        self.u.set_gain(lna_gain, 0)
        self.u.set_antenna('RX2', 0)
        self.qtgui_freq_sink_x_0_0 = qtgui.freq_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	center_freq, #fc
        	hw_sample_rate, #bw
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0_0.set_y_axis(freq_min, -10)
        self.qtgui_freq_sink_x_0_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0_0.enable_grid(True)
        self.qtgui_freq_sink_x_0_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0_0.enable_control_panel(False)

        if not True:
          self.qtgui_freq_sink_x_0_0.disable_legend()

        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0_0.set_plot_pos_half(not True)

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_0_win, 1,0,2,5)
        self.dtv_atsc_viterbi_decoder_0 = dtv.atsc_viterbi_decoder()
        self.dtv_atsc_sync_0 = dtv.atsc_sync(11.8385e6)
        self.dtv_atsc_rx_filter_0 = dtv.atsc_rx_filter(processing_rate,1.1)
        self.dtv_atsc_rs_decoder_0 = dtv.atsc_rs_decoder()
        self.dtv_atsc_fs_checker_0 = dtv.atsc_fs_checker()
        self.dtv_atsc_fpll_0 = dtv.atsc_fpll(11.8385e6)
        self.dtv_atsc_equalizer_0 = dtv.atsc_equalizer()
        self.dtv_atsc_derandomizer_0 = dtv.atsc_derandomizer()
        self.dtv_atsc_depad_0 = dtv.atsc_depad()
        self.dtv_atsc_deinterleaver_0 = dtv.atsc_deinterleaver()
        self.dc_blocker_xx_0 = filter.dc_blocker_ff(4096, True)
        self.atsc2_stream_server_0 = atsc2.tcp_sink(8080)
        self.analog_agc_xx_0 = analog.agc_ff(1e-5, 4.0, 1.0)
        self.analog_agc_xx_0.set_max_gain(65536)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_agc_xx_0, 0), (self.dtv_atsc_sync_0, 0))
        self.connect((self.dc_blocker_xx_0, 0), (self.analog_agc_xx_0, 0))
        self.connect((self.dtv_atsc_deinterleaver_0, 0), (self.dtv_atsc_rs_decoder_0, 0))
        self.connect((self.dtv_atsc_depad_0, 0), (self.atsc2_stream_server_0, 0))
        self.connect((self.dtv_atsc_derandomizer_0, 0), (self.dtv_atsc_depad_0, 0))
        self.connect((self.dtv_atsc_equalizer_0, 0), (self.dtv_atsc_viterbi_decoder_0, 0))
        self.connect((self.dtv_atsc_fpll_0, 0), (self.dc_blocker_xx_0, 0))
        self.connect((self.dtv_atsc_fs_checker_0, 0), (self.dtv_atsc_equalizer_0, 0))
        self.connect((self.dtv_atsc_rs_decoder_0, 0), (self.dtv_atsc_derandomizer_0, 0))
        self.connect((self.dtv_atsc_rx_filter_0, 0), (self.dtv_atsc_fpll_0, 0))
        self.connect((self.dtv_atsc_sync_0, 0), (self.dtv_atsc_fs_checker_0, 0))
        self.connect((self.dtv_atsc_viterbi_decoder_0, 0), (self.dtv_atsc_deinterleaver_0, 0))
        self.connect((self.u, 0), (self.dtv_atsc_rx_filter_0, 0))
        self.connect((self.u, 0), (self.qtgui_freq_sink_x_0_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "atsc2_receive")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_tuning_offset(self):
        return self.tuning_offset

    def set_tuning_offset(self, tuning_offset):
        self.tuning_offset = tuning_offset
        self.set_radio_freq(self.center_freq+self.tuning_offset)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        Qt.QMetaObject.invokeMethod(self._center_freq_line_edit, "setText", Qt.Q_ARG("QString", eng_notation.num_to_str(self.center_freq)))
        self.u.set_center_freq(self.center_freq, 0)
        self.set_radio_freq(self.center_freq+self.tuning_offset)
        self.qtgui_freq_sink_x_0_0.set_frequency_range(self.center_freq, self.hw_sample_rate)

    def get_vga_gain(self):
        return self.vga_gain

    def set_vga_gain(self, vga_gain):
        self.vga_gain = vga_gain

    def get_radio_freq(self):
        return self.radio_freq

    def set_radio_freq(self, radio_freq):
        self.radio_freq = radio_freq

    def get_processing_rate(self):
        return self.processing_rate

    def set_processing_rate(self, processing_rate):
        self.processing_rate = processing_rate

    def get_lna_gain(self):
        return self.lna_gain

    def set_lna_gain(self, lna_gain):
        self.lna_gain = lna_gain
        self.u.set_gain(self.lna_gain, 0)


    def get_hw_sample_rate(self):
        return self.hw_sample_rate

    def set_hw_sample_rate(self, hw_sample_rate):
        self.hw_sample_rate = hw_sample_rate
        self.u.set_samp_rate(self.hw_sample_rate)
        self.qtgui_freq_sink_x_0_0.set_frequency_range(self.center_freq, self.hw_sample_rate)

    def get_freq_min(self):
        return self.freq_min

    def set_freq_min(self, freq_min):
        self.freq_min = freq_min
        self.qtgui_freq_sink_x_0_0.set_y_axis(self.freq_min, -10)

    def get_amp(self):
        return self.amp

    def set_amp(self, amp):
        self.amp = amp


def main(top_block_cls=atsc2_receive, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
