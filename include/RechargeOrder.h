#pragma once

#include "TianHongCardOrder.h"

struct LOG_QUERY_PARAM
{
	std::shared_ptr<int> spNUserId;
	std::shared_ptr<tstring> spStrDuration;
	std::shared_ptr<tstring> spStrMsg;
};

class CUser
{
public:
	CUser()
	{
		this->Reset();
	}

	~CUser()
	{
	}

	void Reset()
	{
		m_nId = 0;
		m_strUserName = _T("");
		m_strRoleName = _T("");
		m_strEmail = _T("");
		m_strMobile = _T("");
	}

public:
	int m_nId;
	tstring m_strUserName;
	tstring m_strRoleName;
	tstring m_strEmail;
	tstring m_strMobile;
};

class CUserLog
{
public:
	CUserLog()
	{
		this->Reset();
	}

	~CUserLog()
	{
	}

	void Reset()
	{
		m_nId = 0;
		m_nUserId = 0;
		m_nType = 0;
		m_strTime = _T("");
		m_strMsg = _T("");
	}

public:
	int m_nId;
	int m_nUserId;
	int m_nType;
	tstring m_strTime;
	tstring m_strMsg;
};

struct ORDER_QUERY_PARAM
{
	std::shared_ptr<tstring> spStrNumber;
	std::shared_ptr<int> spNCustomerId;
	std::shared_ptr<tstring> spStrUserName;
	std::shared_ptr<tstring> spStrState;
	std::shared_ptr<tstring> spStrGameName;
	std::shared_ptr<tstring> spStrChannel;
	std::shared_ptr<tstring> spStrDateStart;
	std::shared_ptr<tstring> spStrDateEnd;
};

class COrderSnapshot
{
public: 
	COrderSnapshot()
	{
		this->Reset();
	}

	~COrderSnapshot()
	{
	}

	void Reset()
	{
		m_nType = 0;
		m_strHashFileName = _T("");
		m_strOriginalFileName = _T("");
		m_strOriginalFilePath = _T("");
	}

public:
	int m_nType;
	tstring m_strHashFileName;
	tstring m_strOriginalFileName;
	tstring m_strOriginalFilePath;
};

class CRechargeOrder
{
public:
	CRechargeOrder()
	{
		this->Reset();
	}

	~CRechargeOrder()
	{
	}

	void Reset()
	{
		m_nId = 0;
		m_nType = 0;
		m_strNumber = _T("");
		m_nCustomerId = 0;
		m_strProductName = _T("");
		m_fPrice = 0;
		m_fPayPrice = 0;
		m_strQQNumber = _T("");
		m_strTelephoneNumber = _T("");
		m_strRoleName = _T("");
		m_strStandbyRoleName = _T("");
		m_strRoleAttri = _T("");

		m_nGameAccountId = 0;
		m_strUserName = _T("");
		m_strPassword = _T("");
		m_strGameName = _T("");
		m_strServiceArea = _T("");
		m_strChannel = _T("");

		m_strState = _T("");
		m_strBuyTime = _T("");
		m_strPayChannel = _T("");
		m_strPayChannelOrderNumber = _T("");
		m_strBankOrderNumber = _T("");
		m_strPayer = _T("");
		m_strDeliveryNote = _T("");
		m_strPayNote = _T("");
		m_strGifeCode = _T("");

		m_fRebateRatio = 0;
		m_vecSnapshot.clear();
		m_vecPlatformOrderNumber.clear();
	}

public:
	int m_nId;							// ID
	int m_nType;						// �������� 0:δ֪���� 1:�׳�Ŷ��� 2:�׳�����䶩�� 3:���䶩��
	tstring m_strNumber;				// ������
	int m_nCustomerId;					// ������ID
	tstring m_strProductName;			// ��Ʒ
	double m_fPrice;					// ����
	double m_fPayPrice;					// Ӧ�����
	tstring m_strQQNumber;				// QQ
	tstring m_strTelephoneNumber;		// �绰
	tstring m_strRoleName;				// ��ɫ��
	tstring m_strStandbyRoleName;		// ���ý�ɫ��
	tstring m_strRoleAttri;				// ��������

	int m_nGameAccountId;				// �ʺ�ID
	tstring m_strUserName;				// �׳��˺�
	tstring m_strPassword;				// �׳��˺�����
	tstring m_strGameName;				// ��Ϸ
	tstring m_strServiceArea;			// ����
	tstring m_strChannel;				// ����
		
	tstring m_strState;					// ����״̬
	tstring m_strBuyTime;				// ����ʱ��
	tstring m_strPayChannel;			// ֧������
	tstring m_strPayChannelOrderNumber;	// ֧������������
	tstring m_strBankOrderNumber;		// ���е���
	tstring m_strPayer;					// ֧����
	tstring m_strDeliveryNote;			// ������ע
	tstring m_strPayNote;				// ֧����ע
	tstring m_strGifeCode;				// ���������

	double m_fRebateRatio;				// ��������
	std::vector<COrderSnapshot> m_vecSnapshot;		// ��ͼ�б�
	std::vector<tstring> m_vecPlatformOrderNumber;	// ƽ̨��ֵ�������б�
};

class CRechargeOrderPayEx : public CRechargeOrder
{
public:
	std::vector<CPayOrder> m_vecPayOrder;
};
