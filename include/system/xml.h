/* ============================================================================
 * NetrdorvOS - 极简 XML 解析器
 * 提取指定标签内的文本内容
 * ============================================================================ */

#ifndef __SYSTEM_XML_H__
#define __SYSTEM_XML_H__

#include <types.h>

/* 在 XML 文本中查找 <tag>...</tag> 内容，返回内容长度，找不到返回 -1 */
int xml_find_text(const char *xml, size_t len,
                  const char *tag, const char **out, size_t *out_len);

#endif
