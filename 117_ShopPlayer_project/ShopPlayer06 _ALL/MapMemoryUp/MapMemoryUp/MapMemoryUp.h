// MapMemoryUp.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMapMemoryUpApp:
// �аѾ\��@�����O�� MapMemoryUp.cpp
//

class CMapMemoryUpApp : public CWinApp
{
public:
	CMapMemoryUpApp();

// �мg
	public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CMapMemoryUpApp theApp;