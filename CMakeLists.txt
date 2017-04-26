cmake_minimum_required(VERSION 3.7)
project(MyDSPv2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp fir_ring_buffer/ring_node.cpp fft/fft.cpp fir_filter/fir_filter.cpp fir_ring_buffer/fir_ring_buffer.cpp lp_fir_filter/lp_fir_filter.cpp hp_fir_filter/hp_fir_filter.cpp hp_fir_filter/hp_fir_filter.h bs_fir_filter/bs_fir_filter.cpp bs_fir_filter/bs_fir_filter.h bp_fir_filter/bp_fir_filter.cpp bp_fir_filter/bp_fir_filter.h parametric_mid_fir_filter/parametric_mid_fir_filter.cpp parametric_mid_fir_filter/parametric_mid_fir_filter.h fir_equalizer/fir_equalizer.cpp fir_equalizer/fir_equalizer.h)

add_executable(MyDSPv2 ${SOURCE_FILES})