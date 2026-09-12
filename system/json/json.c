/* ============================================================================
 * NetrdorvOS - 极简 JSON 解析器实现
 * 仅支持查找对象内的 key→value（int/string），不构建 AST
 * ============================================================================ */

#include <system/json.h>
#include <lib/string.h>

void json_init(json_parser_t *p, const char *text, size_t len)
{
    p->cur = text;
    p->end = text + len;
    p->depth = 0;
}

static void skip_ws(const char **c, const char *end)
{
    while (*c < end && (**c == ' ' || **c == '\t' ||
                        **c == '\n' || **c == '\r'))
        (*c)++;
}

json_err_t json_get_int(const char *json, size_t len,
                        const char *key, s64 *out)
{
    const char *c = json, *e = json + len;
    size_t klen = strlen(key);
    skip_ws(&c, e);
    if (c >= e || *c != '{') return JSON_ERR_UNEXPECTED;
    c++;
    while (c < e) {
        skip_ws(&c, e);
        if (*c == '}') break;
        if (*c != '"') return JSON_ERR_UNEXPECTED;
        c++;
        if (strncmp(c, key, klen) == 0 && c[klen] == '"') {
            c += klen + 1;
            skip_ws(&c, e);
            if (*c != ':') return JSON_ERR_UNEXPECTED;
            c++;
            skip_ws(&c, e);
            s64 val = 0;
            int neg = 0;
            if (*c == '-') { neg = 1; c++; }
            while (c < e && *c >= '0' && *c <= '9') {
                val = val * 10 + (*c - '0');
                c++;
            }
            *out = neg ? -val : val;
            return JSON_OK;
        }
        /* 跳过当前 key */
        while (c < e && *c != '"') c++;
        c++;
        skip_ws(&c, e);
        if (*c != ':') return JSON_ERR_UNEXPECTED;
        c++;
        skip_ws(&c, e);
        /* 跳过 value */
        if (*c == '"') { c++; while (c < e && *c != '"') c++; c++; }
        else { while (c < e && *c != ',' && *c != '}') c++; }
        if (*c == ',') c++;
    }
    return JSON_ERR_UNEXPECTED;
}

json_err_t json_get_string(const char *json, size_t len,
                            const char *key, const char **out, size_t *out_len)
{
    const char *c = json, *e = json + len;
    size_t klen = strlen(key);
    skip_ws(&c, e);
    if (c >= e || *c != '{') return JSON_ERR_UNEXPECTED;
    c++;
    while (c < e) {
        skip_ws(&c, e);
        if (*c == '}') break;
        if (*c != '"') return JSON_ERR_UNEXPECTED;
        c++;
        if (strncmp(c, key, klen) == 0 && c[klen] == '"') {
            c += klen + 1;
            skip_ws(&c, e);
            if (*c != ':') return JSON_ERR_UNEXPECTED;
            c++;
            skip_ws(&c, e);
            if (*c != '"') return JSON_ERR_UNEXPECTED;
            c++;
            *out = c;
            const char *start = c;
            while (c < e && *c != '"') c++;
            *out_len = (size_t)(c - start);
            return JSON_OK;
        }
        while (c < e && *c != '"') c++;
        c++;
        skip_ws(&c, e);
        c++; /* : */
        skip_ws(&c, e);
        if (*c == '"') { c++; while (c < e && *c != '"') c++; c++; }
        else { while (c < e && *c != ',' && *c != '}') c++; }
        if (*c == ',') c++;
    }
    return JSON_ERR_UNEXPECTED;
}
