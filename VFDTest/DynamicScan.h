
#pragma once

#include "Hui.h"

// DynamicScan ����Ŀ��

#define MAXSCANREADY	1		// ɨ��ͷ׼��ʱ�䣬��λ�롣ʵ��S/N201102ɨ��ͷԼ8�룬�����ڵ�sdk���һЩ��

class DynamicScan : public HCtrl
{
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(DynamicScan)

// Construction
public:
	DynamicScan(LPCRECT prc);
	virtual ~DynamicScan();
	virtual void initCtrl();	// ���ڽ�����Ϻ��ʼ������û����ʾ����

// Overflow
protected:
	virtual void draw( CDC* pDC, CRect & rcClip );	// ���ͻ���

// Methods
private:
	bool initCamera();	// ��ʼ��ɨ��ͷ
	void dynamicScan( RGBQUAD * pPalette );
	void temperatureCorrection(LPWORD m_pBuf);	// �¶�У׼

	//void stopScan();	// ֹͣɨ��
	//void go();			// ��ʼɨ��

public:
	void getData();		// ȡ����
	void layerout();
// Properties
private:
	
	HLabel *		m_pPrompt;		// ���Ͻ���ʾ��
	HLabel *		m_pInfo;		// ���½���ʾ��
	HLabel *		m_pNo;
	HLabel *		m_pText;
	HLabel *		m_pTime;		//���Ͻ���ʾ��
	//HLabel *		m_pMaxLabel;	// ���ֵ
	//HLabel *		m_pMinLabel;	// ��Сֵ
public:
	// ����״̬
	enum state{
		staErrInit,		// ��ʼ������
		staErrConnect,	// ���Ӵ���
		staError,		// �����Ǵ���״̬
		staStop,		// ֹͣ�ȴ�״̬
		//staFreeze,		// ����״̬
		staConnecting,	// ��������
		//staReady,		// ׼�����
		staScanning,		// ����ɨ��
		staCorrection	// У׼
	} m_state;

	state	getState(){ return m_state; }
	bool	isState( state sta ){ return sta == m_state; }
	int	m_nFilterOptions;	// �˲�����
	int m_nTemperature1;	// ����1, ����1/CAMERA����ͬ
	int m_nTemperature2;	// ʵ��1
	int m_nTemperature3;	// ����2
	int m_nTemperature4;	// ʵ��2
	int m_nTemperatureCalibration;	// 0 �������¶�У׼�����������¶�У׼

// Messages
public:
};


