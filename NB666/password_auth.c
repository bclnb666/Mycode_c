/* password_auth.c - 系统阴影文件级的加密混淆校验系统 */
#define _XOPEN_SOURCE
#include "iot_gateway.h"
#include <shadow.h>
#include <crypt.h>
#include <string.h>
#include <errno.h>

/**
 * shadow_system_authenticate: 对口令发起基于不可逆盐值的比对
 * 返回值：0 表明身份审核批准, -1 表明凭据无效或被系统拒绝
 */
int shadow_system_authenticate(const char *req_user, const char *req_pass) {
    if (!req_user ||!req_pass) return -1;

    /* 获取受操作系统保护的账户密文对象（必须运行于 root 提权域下）*/
    struct spwd *shadow_record = getspnam(req_user);
    if (shadow_record == NULL) {
        /* 未在操作系统注册该实体名称，或者服务被降权无法跨越屏障 */
        return -1; 
    }

    /* 
     * shadow_record->sp_pwdp 内嵌格式: $加密算法ID$扰乱盐串$最终摘要体
     * crypt 内部解码会自适应识别算法ID及分离出前部用于重复掺沙过程
     */
    char *computed_hash_result = crypt(req_pass, shadow_record->sp_pwdp);
    
    if (computed_hash_result && strcmp(computed_hash_result, shadow_record->sp_pwdp) == 0) {
        return 0; /* 摘要散列完美匹配 */
    }
    
    return -1; /* 口令错漏 */
}
