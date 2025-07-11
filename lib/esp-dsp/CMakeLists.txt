set(srcs            "modules/common/misc/dsps_pwroftwo.cpp"
                    "modules/common/misc/aes3_tie_log.c"
                    "modules/dotprod/float/dsps_dotprod_f32_ae32.S"
                    "modules/dotprod/float/dsps_dotprod_f32_m_ae32.S"
                    "modules/dotprod/float/dsps_dotprode_f32_ae32.S"
                    "modules/dotprod/float/dsps_dotprode_f32_m_ae32.S"
                    "modules/dotprod/float/dsps_dotprod_f32_ansi.c"
                    "modules/dotprod/float/dsps_dotprode_f32_ansi.c"
                    "modules/dotprod/float/dsps_dotprod_f32_aes3.S"
                    "modules/dotprod/float/dsps_dotprod_f32_arp4.S"
                    "modules/dotprod/float/dsps_dotprode_f32_arp4.S"

                    "modules/dotprod/fixed/dsps_dotprod_s16_ae32.S"
                    "modules/dotprod/fixed/dsps_dotprod_s16_m_ae32.S"
                    "modules/dotprod/fixed/dsps_dotprod_s16_ansi.c"
                    "modules/dotprod/fixed/dsps_dotprod_s16_arp4.S"

                    "modules/dotprod/float/dspi_dotprod_f32_ansi.c"
                    "modules/dotprod/float/dspi_dotprod_off_f32_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_s16_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_u16_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_s8_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_u8_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_off_s16_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_off_u16_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_off_s8_ansi.c"
                    "modules/dotprod/fixed/dspi_dotprod_off_u8_ansi.c"

                    "modules/dotprod/fixed/dspi_dotprod_s16_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_u16_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_s16_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_u16_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_s8_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_u8_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_u8_aes3.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_s8_aes3.S"

                    "modules/dotprod/fixed/dspi_dotprod_s16_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_s8_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_u16_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_u8_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_s16_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_u16_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_s8_arp4.S"
                    "modules/dotprod/fixed/dspi_dotprod_off_u8_arp4.S"

                    "modules/matrix/mul/float/dspm_mult_3x3x1_f32_ae32.S"
                    "modules/matrix/mul/float/dspm_mult_3x3x3_f32_ae32.S"
                    "modules/matrix/mul/float/dspm_mult_4x4x1_f32_ae32.S"
                    "modules/matrix/mul/float/dspm_mult_4x4x4_f32_ae32.S"
                    "modules/matrix/mul/float/dspm_mult_f32_ae32.S"
                    "modules/matrix/mul/float/dspm_mult_f32_aes3.S"
                    "modules/matrix/mul/float/dspm_mult_f32_ansi.c"
                    "modules/matrix/mul/float/dspm_mult_f32_arp4.S"
                    "modules/matrix/mul/float/dspm_mult_ex_f32_ansi.c"
                    "modules/matrix/mul/float/dspm_mult_ex_f32_ae32.S"
                    "modules/matrix/mul/float/dspm_mult_ex_f32_arp4.S"
                    "modules/matrix/mul/float/dspm_mult_ex_f32_aes3.S"
                    "modules/matrix/mul/fixed/dspm_mult_s16_ae32.S"
                    "modules/matrix/mul/fixed/dspm_mult_s16_m_ae32_vector.S"
                    "modules/matrix/mul/fixed/dspm_mult_s16_m_ae32.S"
                    "modules/matrix/mul/fixed/dspm_mult_s16_ansi.c"
                    "modules/matrix/mul/fixed/dspm_mult_s16_aes3.S"
                    "modules/matrix/mul/fixed/dspm_mult_s16_arp4.S"
                    "modules/matrix/add/float/dspm_add_f32_ansi.c"
                    "modules/matrix/add/float/dspm_add_f32_ae32.S"
                    "modules/matrix/addc/float/dspm_addc_f32_ansi.c"
                    "modules/matrix/addc/float/dspm_addc_f32_ae32.S"
                    "modules/matrix/mulc/float/dspm_mulc_f32_ansi.c"
                    "modules/matrix/mulc/float/dspm_mulc_f32_ae32.S"
                    "modules/matrix/sub/float/dspm_sub_f32_ansi.c"
                    "modules/matrix/sub/float/dspm_sub_f32_ae32.S"
                    "modules/matrix/mat/mat.cpp"

                    "modules/math/mulc/float/dsps_mulc_f32_ansi.c"
                    "modules/math/addc/float/dsps_addc_f32_ansi.c"
                    "modules/math/mulc/fixed/dsps_mulc_s16_ansi.c"
                    "modules/math/mulc/fixed/dsps_mulc_s16_ae32.S"
                    "modules/math/add/float/dsps_add_f32_ansi.c"
                    "modules/math/add/fixed/dsps_add_s16_ansi.c"
                    "modules/math/add/fixed/dsps_add_s16_ae32.S"
                    "modules/math/add/fixed/dsps_add_s16_aes3.S"
                    "modules/math/add/fixed/dsps_add_s8_ansi.c"
                    "modules/math/add/fixed/dsps_add_s8_aes3.S"

                    "modules/math/sub/float/dsps_sub_f32_ansi.c"
                    "modules/math/sub/fixed/dsps_sub_s16_ansi.c"
                    "modules/math/sub/fixed/dsps_sub_s16_ae32.S"
                    "modules/math/sub/fixed/dsps_sub_s16_aes3.S"
                    "modules/math/sub/fixed/dsps_sub_s8_ansi.c"
                    "modules/math/sub/fixed/dsps_sub_s8_aes3.S"

                    "modules/math/mul/float/dsps_mul_f32_ansi.c"
                    "modules/math/mul/fixed/dsps_mul_s16_ansi.c"
                    "modules/math/mul/fixed/dsps_mul_s16_ae32.S"
                    "modules/math/mul/fixed/dsps_mul_s16_aes3.S"
                    "modules/math/mul/fixed/dsps_mul_s8_ansi.c"
                    "modules/math/mul/fixed/dsps_mul_s8_aes3.S"

                    "modules/math/mulc/float/dsps_mulc_f32_ae32.S"
                    "modules/math/addc/float/dsps_addc_f32_ae32.S"
                    "modules/math/add/float/dsps_add_f32_ae32.S"
                    "modules/math/sub/float/dsps_sub_f32_ae32.S"
                    "modules/math/mul/float/dsps_mul_f32_ae32.S"
                    "modules/math/sqrt/float/dsps_sqrt_f32_ansi.c"

                    "modules/fft/float/dsps_fft2r_fc32_ae32_.S"
                    "modules/fft/float/dsps_fft2r_fc32_aes3_.S"
                    "modules/fft/float/dsps_fft2r_fc32_arp4.S"
                    "modules/fft/float/dsps_fft2r_fc32_ansi.c"
                    "modules/fft/float/dsps_fft2r_fc32_ae32.c"
                    "modules/fft/float/dsps_bit_rev_lookup_fc32_aes3.S"
                    "modules/fft/float/dsps_fft4r_fc32_ansi.c"
                    "modules/fft/float/dsps_fft4r_fc32_ae32.c"
                    "modules/fft/float/dsps_fft4r_fc32_ae32_.S"
                    "modules/fft/float/dsps_fft4r_fc32_aes3_.S"
                    "modules/fft/float/dsps_fft4r_fc32_arp4.S"
                    "modules/fft/float/dsps_fft2r_bitrev_tables_fc32.c"
                    "modules/fft/float/dsps_fft4r_bitrev_tables_fc32.c"
                    "modules/fft/fixed/dsps_fft2r_sc16_ae32.S"
                    "modules/fft/fixed/dsps_fft2r_sc16_ansi.c"
                    "modules/fft/fixed/dsps_fft2r_sc16_aes3.S"
                    "modules/fft/fixed/dsps_fft2r_sc16_arp4.S"

                    "modules/dct/float/dsps_dct_f32.c"
                    "modules/dct/float/dsps_dctiv_f32.c"
                    "modules/dct/float/dsps_dstiv_f32.c"
                    "modules/support/snr/float/dsps_snr_f32.cpp"
                    "modules/support/sfdr/float/dsps_sfdr_f32.cpp"
                    "modules/support/misc/dsps_d_gen.c"
                    "modules/support/misc/dsps_h_gen.c"     
                    "modules/support/misc/dsps_tone_gen.c"
                    "modules/support/cplx_gen/dsps_cplx_gen.c"
                    "modules/support/cplx_gen/dsps_cplx_gen.S"
                    "modules/support/cplx_gen/dsps_cplx_gen_init.c"
                    "modules/support/mem/esp32s3/dsps_memset_aes3.S"
                    "modules/support/mem/esp32s3/dsps_memcpy_aes3.S"
                    "modules/support/view/dsps_view.cpp"
                    "modules/windows/hann/float/dsps_wind_hann_f32.c"
                    "modules/windows/blackman/float/dsps_wind_blackman_f32.c"
                    "modules/windows/blackman_harris/float/dsps_wind_blackman_harris_f32.c"
                    "modules/windows/blackman_nuttall/float/dsps_wind_blackman_nuttall_f32.c"
                    "modules/windows/nuttall/float/dsps_wind_nuttall_f32.c"
                    "modules/windows/flat_top/float/dsps_wind_flat_top_f32.c"
                    "modules/conv/float/dsps_conv_f32_ansi.c"
                    "modules/conv/float/dspi_conv_f32_ansi.c"
                    "modules/conv/float/dsps_conv_f32_ae32.S"
                    "modules/conv/float/dsps_corr_f32_ansi.c"
                    "modules/conv/float/dsps_corr_f32_ae32.S"
                    "modules/conv/float/dsps_ccorr_f32_ansi.c"
                    "modules/conv/float/dsps_ccorr_f32_ae32.S"
                    "modules/iir/biquad/dsps_biquad_f32_ae32.S"
                    "modules/iir/biquad/dsps_biquad_sf32_ae32.S"
                    "modules/iir/biquad/dsps_biquad_f32_aes3.S"
                    "modules/iir/biquad/dsps_biquad_f32_arp4.S"
                    "modules/iir/biquad/dsps_biquad_sf32_arp4.S"
                    "modules/iir/biquad/dsps_biquad_f32_ansi.c"
                    "modules/iir/biquad/dsps_biquad_sf32_ansi.c"
                    "modules/iir/biquad/dsps_biquad_gen_f32.c"
                    "modules/fir/float/dsps_fir_f32_ae32.S"
                    "modules/fir/float/dsps_fir_f32_aes3.S"
                    "modules/fir/float/dsps_fird_f32_ae32.S"
                    "modules/fir/float/dsps_fird_f32_aes3.S"
                    "modules/fir/float/dsps_fird_f32_arp4.S"
                    "modules/fir/float/dsps_fir_f32_ansi.c"
                    "modules/fir/float/dsps_fir_init_f32.c"
                    "modules/fir/float/dsps_fird_f32_ansi.c"
                    "modules/fir/float/dsps_fird_init_f32.c"
                    "modules/fir/fixed/dsps_fird_init_s16.c"
                    "modules/fir/fixed/dsps_fird_s16_ansi.c"
                    "modules/fir/fixed/dsps_fird_s16_ae32.S"
                    "modules/fir/fixed/dsps_fir_s16_m_ae32.S"
                    "modules/fir/fixed/dsps_fird_s16_aes3.S"
                    "modules/fir/fixed/dsps_fird_s16_arp4.S"
                    "modules/fir/float/dsps_firmr_init_f32.c"
                    "modules/fir/float/dsps_firmr_f32_ansi.c"
                    "modules/fir/fixed/dsps_firmr_init_s16.c"
                    "modules/fir/fixed/dsps_firmr_s16_ansi.c"
                    "modules/fir/resampler/dsps_resampler_mr.c"
                    "modules/fir/resampler/dsps_resampler_ph.c"
# EKF files
                    "modules/kalman/ekf/common/ekf.cpp"
                    "modules/kalman/ekf_imu13states/ekf_imu13states.cpp"
                    )



set(include_dirs                "modules/dotprod/include"
                                "modules/support/include"
                                "modules/support/mem/include"
                                "modules/windows/include"
                                "modules/windows/hann/include"
                                "modules/windows/blackman/include"
                                "modules/windows/blackman_harris/include"
                                "modules/windows/blackman_nuttall/include"
                                "modules/windows/nuttall/include"
                                "modules/windows/flat_top/include"
                                "modules/iir/include"
                                "modules/fir/include"
                                "modules/math/include"
                                "modules/math/add/include"
                                "modules/math/sub/include"
                                "modules/math/mul/include"
                                "modules/math/addc/include"
                                "modules/math/mulc/include"
                                "modules/math/sqrt/include"
                                "modules/matrix/mul/include"
                                "modules/matrix/add/include"
                                "modules/matrix/addc/include"
                                "modules/matrix/mulc/include"
                                "modules/matrix/sub/include"
                                "modules/matrix/include"
                                "modules/fft/include"
                                "modules/dct/include"
                                "modules/conv/include"
                                "modules/common/include"
                                "modules/matrix/mul/test/include"
# EKF files
                                "modules/kalman/ekf/include"
                                "modules/kalman/ekf_imu13states/include"
)

set(priv_include_dirs           "modules/dotprod/float"
                                "modules/dotprod/fixed")

idf_component_register(SRCS ${srcs}
                      INCLUDE_DIRS ${include_dirs}
                      PRIV_INCLUDE_DIRS ${priv_include_dirs})
