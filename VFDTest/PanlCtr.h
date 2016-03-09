
#pragma once

#include "DynamicScan.h"
// PanlCtr ÃüÁî´°¿Ú

class PanlCtr : public HCtrl
{
	DECLARE_DYNAMIC(PanlCtr)

// Construction
public:
	PanlCtr(LPCRECT prc);
	virtual ~PanlCtr();

// Properties
public:
	DynamicScan * m_pDynamicScan;
	HEdit *	m_pLog;
	HEdit * m_pScan;
	HLabel * m_pTitle;
	HCtrl * m_pc;
// Overwrite
public:
	virtual void	layerout();					//ÖØÅÅ¿Ø¼þ
};


