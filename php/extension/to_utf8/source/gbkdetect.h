#ifndef __GBK_DETECT_H
#define __GBK_DETECT_H

inline int is_gb2312_head(unsigned char c)
{
    return c >= 0xa1 && c <= 0xa9 || c >= 0xb0 && c <= 0xf9;
}

inline int is_gb2312_tail(unsigned char c)
{
    return c >= 0xa1;
}

#define MAX_GBK_DETECT_NUM 128
/* r>0: gbk, r=0: not gbk , r<0: latin */
inline int is_gbk_encoding(char *p, char *end, int skip = 20)
{
    int gbk = 0, detect = 0, error = 0, comment = 0;
    while (p < end - 1) {
        if (comment == 0 && p[0] < 0) {
            if (is_gb2312_head((unsigned char)p[0]) &&
                    is_gb2312_tail((unsigned char)p[1])) {
                gbk++;
                p++;
            }
            else error++;
            if (++detect > MAX_GBK_DETECT_NUM) break;
        }
#ifdef __SKIP_HTML_COMMENT
        else if (p[0] == '<' && p[1] == '!' && p[2] == '-') {
            comment = 1;
            p += 2;
        }
        else if (p[0] == '-' && p[1] == '-' && p[2] == '>') {
            comment = 0;
            p += 2;
        }
#endif
        p++;
    }
#ifdef __DEBUG
    printf("detect:%d, error:%d, gbk:%d\n", detect, error, gbk);
#endif
    if (error == 0 || gbk / error >= skip) return gbk;
    if (error && error * 50 > detect * 48) return -1;
    return 0;
}

#endif
