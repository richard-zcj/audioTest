#ifndef AUTHORIZATION_KEY_H
#define AUTHORIZATION_KEY_H

// 初始化加密锁
bool InitPwdKey();
// 释放加密锁
void DestroyPwdKey();
// 检查加密锁是否存在，true：存在，false：不存在
extern int CheckKeyExists(const char* pCheckCode = nullptr);
// 检查是否在有效期内，0：在有效期内，-1：加密锁不存在，-2：超过有效期，-3：系统时间被修改，其它值：检查失败
extern int CheckExpiryDate();
// 获取有效期，0：成功，1：长期有效，-1：加密锁不存在，-2：获取失败
extern int GetExpiryDate(char* pExpiryDate, size_t nSize);
// 获取用户号
extern int GetUserCode(const char* pCheckCode = nullptr);
// 更新授权号，nFlag->0：次数授权，1：绑定电脑授权，2：时间授权
extern int UpdateAuthCode(const char* pAuthCode, short nFlag = 2);

#endif	// AUTHORIZATION_KEY_H