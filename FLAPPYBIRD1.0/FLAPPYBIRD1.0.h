
// FLAPPYBIRD1.0.h : FLAPPYBIRD1.0 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFLAPPYBIRD10App:
// �йش����ʵ�֣������ FLAPPYBIRD1.0.cpp
//

class CFLAPPYBIRD10App : public CWinAppEx
{
public:
	CFLAPPYBIRD10App();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFLAPPYBIRD10App theApp;
