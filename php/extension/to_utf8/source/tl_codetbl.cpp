#include "tl_codetbl.h"

//unsigned short	_G_map_gbk_2_ucs[ 64 * 1024 ] = TO_UTF8_G(_G_map_gbk_2_ucs);
//unsigned short	_G_map_ucs_2_gbk[ 64 * 1024 ] = TO_UTF8_G(_G_map_ucs_2_gbk);

namespace encoding
{
	inline void  unicode_to_gbk(unsigned short uc,unsigned char *b1,unsigned char*b2)
	{
		*b1 = *b2 = 0;
		unsigned short ret = _G_map_ucs_2_gbk[uc];

		if ( ret == 0 )
		{
			*b1 = 0x20;
		}
		else if( ret < 0x80 && ret != 0)
			*b1 = (unsigned char) ret;
		else if ( ret == 0xfffd )
			*b1 = '&';
		else
		{
			*b1 = ret >> 8;
			*b2 = ret & 0x00FF;
		}
	}

	int	conv_gbk_2_ucs(const char* ptr_buf_src, int len_src, unsigned short* ptr_wbuf_dst, int len_dst_limit)
	{
		int	len_dst = 0;
		int	off_scan ; 

		unsigned char	ch_lo, ch_hi;
		for( off_scan = 0 ; off_scan < len_src ; off_scan ++)
		{
			if(len_dst >= len_dst_limit) 
				return len_dst;

			ch_lo = ptr_buf_src[ off_scan ] ;

			if(ch_lo <= 0x80)
				ptr_wbuf_dst[ len_dst ++ ] = _G_map_gbk_2_ucs[ ch_lo ];

			else if( off_scan < len_src - 1 && (ch_hi = ptr_buf_src[ off_scan + 1 ]) > 0x3f)
				ptr_wbuf_dst[ len_dst ++ ] = _G_map_gbk_2_ucs[ *(unsigned short*) ( ptr_buf_src + off_scan ++ ) ];

			else 
			{	
				ptr_wbuf_dst[ len_dst ++ ] = 0xfffd; 
				if( ch_lo > 0x80 )
					off_scan ++;	
			}

		}	
		return len_dst;
	}

	int	conv_ucs_2_gbk(const unsigned short* ptr_wbuf_src, int len_src, char* ptr_buf_dst, int len_dst_limit)
	{
		int	off_scan; 
		int	len_dst = 0;
		for( off_scan = 0 ; off_scan < len_src ; off_scan ++ )
		{
			if( len_dst >= len_dst_limit )
				return len_dst;

			unsigned short	uc = _G_map_ucs_2_gbk[ ptr_wbuf_src[ off_scan ] ] ;


			if( uc <= 0x80 ) 
			{
				if ( uc == 0 && ptr_wbuf_src[off_scan] ) uc = 0x20;
				ptr_buf_dst [ len_dst ++ ] = (char) uc;
			}
			else if( uc == 0xfffd )
				ptr_buf_dst [ len_dst ++ ] = '&';

			else
			{
				if(len_dst >= len_dst_limit - 1) 
					return -1;
				*(unsigned short*) (ptr_buf_dst + len_dst ) = uc;
				len_dst += 2;
			}
		}
		return len_dst;
	}
	/**********************************************************************
	 * U-0000 每 U-007F:  0xxxxxxx  
	 * U-0080 每 U-07FF:  110xxxxx 10xxxxxx  
	 * U-0800 每 U-FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
	 * *******************************************************************/

	int	conv_utf8_2_ucs(const char* ptr_buf_src, int len_src, unsigned short* ptr_wbuf_dst, int len_dst_limit)
	{
		int     len_dst = 0;
		int     off_scan;
		unsigned char   ch0, ch1, ch2;

		for( off_scan = 0 ; off_scan < len_src - 2; )
		{       
			if( len_dst >= len_dst_limit )
				return len_dst; 

			ch0 = ptr_buf_src[ off_scan ++ ];
			if( ch0 < 0x80 )
			{       
				ptr_wbuf_dst[ len_dst ++ ] = ch0;
				continue;
			}       

			ch1 = ptr_buf_src[ off_scan ++ ];

			if( ( ch0 & 0xe0 ) == 0xc0 )
			{       
				ptr_wbuf_dst [ len_dst ++ ] = ((ch1 & 0xc0) == 0x80) ? ((ch0 & 0x1f) << 6) | (ch1 & 0x3f) : 0xfffd; 
				continue;
			}       

			ch2 = ptr_buf_src[ off_scan ++ ];

			if( (ch0 & 0xe0) == 0xe0 && ( ch1 & 0xc0 ) == 0x80 && (ch2 & 0xc0 ) == 0x80 )
				ptr_wbuf_dst [ len_dst ++ ] = ((ch0 & 0xf ) << 12 ) | ( (ch1 & 0x3f) << 6 ) | (ch2 & 0x3f);
			else    
				ptr_wbuf_dst [ len_dst ++ ] = 0xfffd; 



		}       

		if( off_scan > len_src ) 
			return 0;

		while( off_scan < len_src)
		{
			ch0 = ptr_buf_src[ off_scan ++ ] ;
			if( ch0 < 0x80 )
			{       
				ptr_wbuf_dst [ len_dst ++ ] = ch0;
				continue;
			}       

			if( off_scan >= len_src )
				break;

			ch1 = ptr_buf_src[ off_scan ++ ];
			if( ( ch0 & 0xe0 ) == 0xc0 )
			{
				ptr_wbuf_dst [ len_dst ++ ] = ((ch1 & 0xc0) == 0x80) ? ((ch0 & 0x1f) << 6) | (ch1 & 0x3f) : 0xfffd;
				continue;
			}

			if( off_scan >= len_src )
				break;

			ch2 = ptr_buf_src[ off_scan ++ ];
			if( (ch0 & 0xe0) == 0xe0 && ( ch1 & 0xc0 ) == 0x80 && (ch2 & 0xc0 ) == 0x80 )
				ptr_wbuf_dst [ len_dst ++ ] = ((ch0 & 0xf ) << 12 ) | ( (ch1 & 0x3f) << 6 ) | (ch2 & 0x3f);
			else
				ptr_wbuf_dst [ len_dst ++ ] = 0xfffd;
		}
		return len_dst;

	}
	/**********************************************************************
	 * U-0000 每 U-007F:  0xxxxxxx  
	 * U-0080 每 U-07FF:  110xxxxx 10xxxxxx  
	 * U-0800 每 U-FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
	 * *******************************************************************/
	int	conv_utf8_4_ucs(const char* ptr_buf_src, int len_src, unsigned int* ptr_wbuf_dst, int len_dst_limit)
	{
		int     len_dst = 0;
		int     off_scan;
		unsigned char   ch0, ch1, ch2;

		for( off_scan = 0 ; off_scan < len_src - 2; )
		{       
			if( len_dst >= len_dst_limit )
				return len_dst; 

			ch0 = ptr_buf_src[ off_scan ++ ];
			if( ch0 < 0x80 )
			{       
				ptr_wbuf_dst[ len_dst ++ ] = ch0;
				continue;
			}       

			ch1 = ptr_buf_src[ off_scan ++ ];

			if( ( ch0 & 0xe0 ) == 0xc0 )
			{       
				ptr_wbuf_dst [ len_dst ++ ] = ((ch1 & 0xc0) == 0x80) ? ((ch0 & 0x1f) << 6) | (ch1 & 0x3f) : 0xfffd; 
				continue;
			}       

			ch2 = ptr_buf_src[ off_scan ++ ];

			if( (ch0 & 0xe0) == 0xe0 && ( ch1 & 0xc0 ) == 0x80 && (ch2 & 0xc0 ) == 0x80 )
				ptr_wbuf_dst [ len_dst ++ ] = ((ch0 & 0xf ) << 12 ) | ( (ch1 & 0x3f) << 6 ) | (ch2 & 0x3f);
			else    
				ptr_wbuf_dst [ len_dst ++ ] = 0xfffd; 



		}       

		if( off_scan > len_src ) 
			return 0;

		while( off_scan < len_src)
		{
			ch0 = ptr_buf_src[ off_scan ++ ] ;
			if( ch0 < 0x80 )
			{       
				ptr_wbuf_dst [ len_dst ++ ] = ch0;
				continue;
			}       

			if( off_scan >= len_src )
				break;

			ch1 = ptr_buf_src[ off_scan ++ ];
			if( ( ch0 & 0xe0 ) == 0xc0 )
			{
				ptr_wbuf_dst [ len_dst ++ ] = ((ch1 & 0xc0) == 0x80) ? ((ch0 & 0x1f) << 6) | (ch1 & 0x3f) : 0xfffd;
				continue;
			}

			if( off_scan >= len_src )
				break;

			ch2 = ptr_buf_src[ off_scan ++ ];
			if( (ch0 & 0xe0) == 0xe0 && ( ch1 & 0xc0 ) == 0x80 && (ch2 & 0xc0 ) == 0x80 )
				ptr_wbuf_dst [ len_dst ++ ] = ((ch0 & 0xf ) << 12 ) | ( (ch1 & 0x3f) << 6 ) | (ch2 & 0x3f);
			else
				ptr_wbuf_dst [ len_dst ++ ] = 0xfffd;
		}
		return len_dst;

	}

	/**********************************************************************
	 * U-0000 每 U-007F:  0xxxxxxx  
	 * U-0080 每 U-07FF:  110xxxxx 10xxxxxx  
	 * U-0800 每 U-FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
	 **********************************************************************/
	int	conv_ucs_2_utf8(const unsigned short* ptr_wbuf_src, int len_src, char* ptr_buf_dst, int len_dst_limit)
	{
		int	len_dst = 0;
		int	off_scan;

		unsigned short	uc;
		for( off_scan = 0 ; off_scan < len_src ; off_scan ++ )
		{
			if(len_dst >= len_dst_limit)
				return len_dst;

			uc = ptr_wbuf_src[ off_scan ];
			if( uc < 0x80 ) 
			{
				ptr_buf_dst[ len_dst ++ ] = (char)uc;
				continue;
			}

			if( uc < 0x800 )
			{
				ptr_buf_dst[ len_dst ++ ] =  ( uc >> 6 ) & 0x1f | 0xc0;
				ptr_buf_dst[ len_dst ++ ] =  ( uc & 0x3f ) | 0x80;
				continue;
			}

			ptr_buf_dst [ len_dst ++ ] = ( uc >> 12 ) | 0xe0;
			ptr_buf_dst [ len_dst ++ ] = ( ( uc >> 6 ) & 0x3f ) | 0x80;
			ptr_buf_dst [ len_dst ++ ] = ( uc & 0x3f ) | 0x80;
		}
		return len_dst;	
	}

	int conv_gbk_2_utf8(const char* ptr_buf_src, int len_src, char* ptr_buf_dst, int len_dst_limit)
	{
		int	len_dst = 0;
		int	off_scan ; 

		unsigned char	ch_lo, ch_hi;
		unsigned short tmp;
		for( off_scan = 0 ; off_scan < len_src ; off_scan ++)
		{
			if(len_dst >= len_dst_limit) 
				return len_dst;

			ch_lo = ptr_buf_src[ off_scan ] ;

			if(ch_lo <= 0x80)
				ptr_buf_dst[ len_dst ++ ] = _G_map_gbk_2_ucs[ ch_lo ];
			else if( off_scan < len_src - 1 && (ch_hi = ptr_buf_src[ off_scan + 1 ]) > 0x3f)
			{
				tmp = _G_map_gbk_2_ucs[ *(unsigned short*) ( ptr_buf_src + off_scan ++ ) ];
				if ( tmp <= 0x7ff )
				{
					if ( len_dst + 2 >= len_dst_limit ) return len_dst;
					ptr_buf_dst [ len_dst ++ ] = ( tmp >> 6 ) | 0xc0;
					ptr_buf_dst [ len_dst ++ ] = ( tmp & 0x3f ) | 0x80;
				}
				else
				{
					if ( len_dst + 3 >= len_dst_limit ) return len_dst;
					ptr_buf_dst [ len_dst ++ ] = ( tmp >> 12 ) | 0xe0;
					ptr_buf_dst [ len_dst ++ ] = ( ( tmp >> 6 ) & 0x3f ) | 0x80;
					ptr_buf_dst [ len_dst ++ ] = ( tmp & 0x3f ) | 0x80;
				}
			}
			else 
			{	
				ptr_buf_dst [ len_dst ++ ] = '?';
				if( ch_lo > 0x80 )
					off_scan ++;
			}
			/*
			   ch_lo = ptr_buf_src[ off_scan ] ;

			   if(ch_lo <= 0x80)
			   ptr_buf_dst[ len_dst ++ ] = _G_map_gbk_2_ucs[ ch_lo ];
			   else if( off_scan < len_src - 1 && (ch_hi = ptr_buf_src[ off_scan + 1 ]) > 0x3f)
			   {
			   if ( len_dst + 3 >= len_dst_limit ) return len_dst;
			   tmp = _G_map_gbk_2_ucs[ *(unsigned short*) ( ptr_buf_src + off_scan ++ ) ];
			   ptr_buf_dst [ len_dst ++ ] = ( tmp >> 12 ) | 0xe0;
			   ptr_buf_dst [ len_dst ++ ] = ( ( tmp >> 6 ) & 0x3f ) | 0x80;
			   ptr_buf_dst [ len_dst ++ ] = ( tmp & 0x3f ) | 0x80;
			   }
			   else 
			   {	
			   ptr_buf_dst [ len_dst ++ ] = '?';
			   if( ch_lo > 0x80 )
			   off_scan ++;	
			   }
			   */

		}	
		return len_dst;
	}

	int	conv_utf8_2_gbk(const char* ptr_buf_src, int len_src, char* ptr_buf_dst, int len_dst_limit)
	{
		int     len_dst = 0;
		int     off_scan;
		unsigned char   ch0, ch1, ch2;
		unsigned short tmp;

		for( off_scan = 0 ; off_scan < len_src - 2; )
		{       
			if( len_dst >= len_dst_limit ) return len_dst; 

			ch0 = ptr_buf_src[ off_scan ++ ];
			if( ch0 < 0x80 )
			{       
				ptr_buf_dst[ len_dst ++ ] = ch0;
				continue;
			}

			ch1 = ptr_buf_src[ off_scan ++ ];

			if( ( ch0 & 0xe0 ) == 0xc0 )
			{       
				if ( len_dst + 2 >= len_dst_limit ) return len_dst;
				tmp = ((ch1 & 0xc0) == 0x80) ? ((ch0 & 0x1f) << 6) | (ch1 & 0x3f) : 0xfffd; 
				unicode_to_gbk(tmp, (unsigned char*)(ptr_buf_dst + len_dst + 1), (unsigned char*)(ptr_buf_dst + len_dst));
				if ( *(ptr_buf_dst + len_dst + 1) != 0 ) len_dst++;
				len_dst++;
				continue;
			}       

			ch2 = ptr_buf_src[ off_scan ++ ];

			if( (ch0 & 0xe0) == 0xe0 && ( ch1 & 0xc0 ) == 0x80 && (ch2 & 0xc0 ) == 0x80 )
			{
				if ( len_dst + 2 >= len_dst_limit ) return len_dst;
				tmp = ((ch0 & 0xf ) << 12 ) | ( (ch1 & 0x3f) << 6 ) | (ch2 & 0x3f); 
				unicode_to_gbk(tmp, (unsigned char*)(ptr_buf_dst + len_dst + 1), (unsigned char*)(ptr_buf_dst + len_dst));
				if ( *(ptr_buf_dst + len_dst + 1) != 0 ) len_dst++;
				len_dst++;
			}
			else    
			{
				if ( len_dst + 2 >= len_dst_limit ) return len_dst;
				unicode_to_gbk(0xfffd, (unsigned char*)(ptr_buf_dst + len_dst + 1), (unsigned char*)(ptr_buf_dst + len_dst));
				if ( *(ptr_buf_dst + len_dst + 1) != 0 ) len_dst++;
				len_dst++;
			}
		}       

		if( off_scan > len_src ) 
			return 0;

		while( off_scan < len_src)
		{
			ch0 = ptr_buf_src[ off_scan ++ ] ;
			if( ch0 < 0x80 )
			{       
				ptr_buf_dst [ len_dst ++ ] = ch0;
				continue;
			}       

			if( off_scan >= len_src )
				break;

			ch1 = ptr_buf_src[ off_scan ++ ];
			if( ( ch0 & 0xe0 ) == 0xc0 )
			{
				if ( len_dst + 2 >= len_dst_limit ) return len_dst;
				tmp = ((ch1 & 0xc0) == 0x80) ? ((ch0 & 0x1f) << 6) | (ch1 & 0x3f) : 0xfffd; 
				unicode_to_gbk(tmp, (unsigned char*)(ptr_buf_dst + len_dst + 1), (unsigned char*)(ptr_buf_dst + len_dst));
				if ( *(ptr_buf_dst + len_dst + 1) != 0 ) len_dst++;
				len_dst++;
				continue;
			}

			if( off_scan >= len_src )
				break;

			ch2 = ptr_buf_src[ off_scan ++ ];
			if( (ch0 & 0xe0) == 0xe0 && ( ch1 & 0xc0 ) == 0x80 && (ch2 & 0xc0 ) == 0x80 )
			{
				if ( len_dst + 2 >= len_dst_limit ) return len_dst;
				tmp = ((ch0 & 0xf ) << 12 ) | ( (ch1 & 0x3f) << 6 ) | (ch2 & 0x3f); 
				unicode_to_gbk(tmp, (unsigned char*)(ptr_buf_dst + len_dst + 1), (unsigned char*)(ptr_buf_dst + len_dst));
				if ( *(ptr_buf_dst + len_dst + 1) != 0 ) len_dst++;
				len_dst++;
			}
			else
			{
				if ( len_dst + 2 >= len_dst_limit ) return len_dst;
				unicode_to_gbk(0xfffd, (unsigned char*)(ptr_buf_dst + len_dst + 1), (unsigned char*)(ptr_buf_dst + len_dst));
				if ( *(ptr_buf_dst + len_dst + 1) != 0 ) len_dst++;
				len_dst++;
			}
		}
		return len_dst;
	}

}
/*
   int	main()
   {
   char	buf[] = "斕a岆a跺a湮a伂a排a";
   unsigned short	wbuf[256];
   char	buf_tmp[256];
   char	buf_utf[256];
   int	len_gbk = 256;
   int	len_ucs = 256;
   int	len_utf = 256;
   len_ucs = conv_gbk_2_ucs(buf, strlen(buf), wbuf, len_ucs);
   len_utf = conv_ucs_2_utf8(wbuf, len_ucs, buf_utf, len_utf);
   len_ucs = conv_utf8_2_ucs(buf_utf, len_utf, wbuf, len_ucs);
   len_gbk = conv_ucs_2_gbk((unsigned short*)wbuf, len_ucs, buf_tmp, len_gbk);
   return 0;	
   }
   */ 
