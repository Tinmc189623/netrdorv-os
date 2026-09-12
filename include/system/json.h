/* ============================================================================
 * NetrdorvOS - 极简 JSON 解析器
 * 不分配内存，逐 token 回调
 * ============================================================================ */

#ifndef __SYSTEM_JSON_H__
#define __SYSTEM_JSON_H__

#include <types.h>

typedef enum {
    JSON_OK = 0,
    JSON_ERR_UNEXPECTED,
    JSON_ERR_INCOMPLETE,
    JSON_ERR_NESTING,
} json_err_t;

typedef struct {
    const char *cur;
    const char *end;
    int         depth;
} json_parser_t;

/* 初始化解析器 */
void json_init(json_parser_t *p, const char *text, size_t len);

/* 查找字符串键对应的整数值，返回 JSON_OK 或错误码 */
json_err_t json_get_int(const char *json, size_t len,
                        const char *key, s64 *out);

/* 查找字符串键对应的字符串值（返回指向原 JSON 的指针） */
json_err_t json_get_string(const char *json, size_t len,
                           const char *key, const char **out, size_t *out_len);

#endif
