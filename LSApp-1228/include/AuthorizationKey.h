#ifndef AUTHORIZATION_KEY_H
#define AUTHORIZATION_KEY_H

// ��ʼ��������
bool InitPwdKey();
// �ͷż�����
void DestroyPwdKey();
// ���������Ƿ���ڣ�true�����ڣ�false��������
extern int CheckKeyExists(const char* pCheckCode = nullptr);
// ����Ƿ�����Ч���ڣ�0������Ч���ڣ�-1�������������ڣ�-2��������Ч�ڣ�-3��ϵͳʱ�䱻�޸ģ�����ֵ�����ʧ��
extern int CheckExpiryDate();
// ��ȡ��Ч�ڣ�0���ɹ���1��������Ч��-1�������������ڣ�-2����ȡʧ��
extern int GetExpiryDate(char* pExpiryDate, size_t nSize);
// ��ȡ�û���
extern int GetUserCode(const char* pCheckCode = nullptr);
// ������Ȩ�ţ�nFlag->0��������Ȩ��1���󶨵�����Ȩ��2��ʱ����Ȩ
extern int UpdateAuthCode(const char* pAuthCode, short nFlag = 2);

#endif	// AUTHORIZATION_KEY_H