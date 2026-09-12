/* ============================================================================
 * NetrdorvOS - XML 解析器实现
 * ============================================================================ */

#include <system/xml.h>
#include <lib/string.h>

int xml_find_text(const char *xml, size_t len,
                  const char *tag, const char **out, size_t *out_len)
{
    char open[64], close[64];
    size_t tlen = strlen(tag);
    if (tlen >= 30) return -1;

    /* <tag> */
    open[0] = '<'; strcpy(open + 1, tag); open[tlen+1] = '>'; open[tlen+2] = 0;
    /* </tag> */
    close[0] = '<'; close[1] = '/'; strcpy(close + 2, tag); close[tlen+2] = '>'; close[tlen+3] = 0;

    size_t olen = tlen + 2;
    size_t clen = tlen + 3;

    const char *p = xml;
    const char *end = xml + len;

    while (p < end - olen) {
        if (p[0] == '<' && strncmp(p, open, olen) == 0) {
            p += olen;
            const char *start = p;
            const char *e = strstr(p, close);
            if (!e || e > end) return -1;
            *out = start;
            *out_len = (size_t)(e - start);
            return 0;
        }
        p++;
    }
    return -1;
}
