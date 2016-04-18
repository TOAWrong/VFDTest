
#pragma once

#include "Hui.h"

// DynamicScan 命令目标

#define MAXSCANREADY	1		// 扫描头准备时间，单位秒。实测S/N201102扫描头约8秒，换早期的sdk会快一些。

class DynamicScan : public HCtrl
{
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(DynamicScan)

// Construction
public:
	DynamicScan(LPCRECT prc);
	virtual ~DynamicScan();
	virtual void initCtrl();	// 窗口建立完毕后初始化，还没有显示窗口

// Overflow
protected:
	virtual void draw( CDC* pDC, CRect & rcClip );	// 画客户区

// Methods
private:
	bool initCamera();	// 初始化扫描头
	void dynamicScan( RGBQUAD * pPalette );
	void temperatureCorrection(LPWORD m_pBuf);	// 温度校准

	//void stopScan();	// 停止扫描
	//void go();			// 开始扫描

public:
	void getData();		// 取数据
	void layerout();
// Properties
private:
	
	HLabel *		m_pPrompt;		// 左上角提示框
	HLabel *		m_pInfo;		// 左下角提示框
	HLabel *		m_pNo;
	HLabel *		m_pText;
	HLabel *		m_pTime;		//右上角提示框
	//HLabel *		m_pMaxLabel;	// 最大值
	//HLabel *		m_pMinLabel;	// 最小值
public:
	// 工作状态
	enum state{
		staErrInit,		// 初始化错误
		staErrConnect,	// 连接错误
		staError,		// 以上是错误状态
		staStop,		// 停止等待状态
		//staFreeze,		// 冻结状态
		staConnecting,	// 正在连接
		//staReady,		// 准备完毕
		staScanning,		// 正在扫描
		staCorrection	// 校准
	} m_state;

	state	getState(){ return m_state; }
	bool	isState( state sta ){ return sta == m_state; }
	int	m_nFilterOptions;	// 滤波参数
	int m_nTemperature1;	// 设置1, 精度1/CAMERA，下同
	int m_nTemperature2;	// 实测1
	int m_nTemperature3;	// 设置2
	int m_nTemperature4;	// 实测2
	int m_nTemperatureCalibration;	// 0 不允许温度校准，否则允许温度校准

// Messages
public:
};


