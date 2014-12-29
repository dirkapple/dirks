#ifndef _TL_CODETBL_H
#define _TL_CODETBL_H

//extern unsigned short _G_map_gbk_2_ucs[ 64 * 1024 ];
//extern unsigned short _G_map_ucs_2_gbk[ 64 * 1024 ];

unsigned short * _G_map_gbk_2_ucs;
unsigned short * _G_map_ucs_2_gbk;

namespace encoding
{
	int   conv_gbk_2_utf8(const char* ptr_buf_src, int len_src, char* ptr_buf_dst, int len_dst_limit);
	int   conv_utf8_2_gbk(const char* ptr_buf_src, int len_src, char* ptr_buf_dst, int len_dst_limit);
	int   conv_gbk_2_ucs(const char* ptr_buf_src, int len_src, unsigned short* ptr_wbuf_dst, int len_dst_limit);
	int   conv_ucs_2_gbk(const unsigned short* ptr_wbuf_src, int len_src, char* ptr_buf_dst, int len_dst_limit);
	int   conv_ucs_2_utf8(const unsigned short* ptr_wbuf_src, int len_src, char* ptr_buf_dst, int len_dst_limit);
	int   conv_utf8_2_ucs(const char* ptr_buf_src, int len_src, unsigned short* ptr_wbuf_dst, int len_dst_limit);
	int   conv_utf8_2_gbk(const char* ptr_buf_src, int len_src, char * ptr_wbuf_dst, int len_dst_limit);
	int   conv_utf8_4_ucs(const char* ptr_buf_src, int len_src, unsigned int* ptr_wbuf_dst, int len_dst_limit);
};

#endif
