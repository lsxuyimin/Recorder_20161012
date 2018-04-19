/////////////////////////////////
// 用户填加部分

//切换用睡眠时间
#define SLEEP_TIME			(200)


//触发方式
#define TRIMODE_EXTERNAL	0
#define TRIMODE_MANUAL		1
#define TRIMODE_RISINGEDGE	2
#define TRIMODE_FALLINGEDGE 3
#define TRIMODE_CHANNEL		4			//通道触发 高于上限或低于下限触发

//采样率
#define RATE_50M		0
#define RATE_10M		1
#define RATE_1M			2
#define RATE_100K		3
#define RATE_10K		4

//采样长度
#define LENGTH_1048576		0
#define LENGTH_262144		1
#define LENGTH_65536		2
#define LENGTH_16384		3
#define LENGTH_4096			4

//量程--------------------------------->原来用于采集参数的量程，后来不用了，只用CWatchDlg中用于显示的量程
#define RANGE_20V		0
#define RANGE_10V		1
#define RANGE_5V		2
#define RANGE_1V		3

//鼠标模式
#define MOUSEMODE_NONE		0
#define MOUSEMODE_ZOOMIN	1
#define MOUSEMODE_ZOOMOUT	2
#define MOUSEMODE_ZOOMDRAG	3
#define MOUSEMODE_RESTORE	4
#define MOUSEMODE_HAND		5
	
//存储相关
#define DATASTORAGE_SEGMENT		10240		//个 采样点
#define DATASTORAGE_MAX_PERCHN	128			//MB

//采集设备基本信息
#define	CH_NUM_IN_PER_CARD		8			//当前的每个卡中有8个通道


//采集状态
#define REC_STA_ACQEND		(0xFF)	//采集结束
#define REC_STA_INACQ		(1)		//正在采集
#define REC_STA_WAITTRG		(0)		//等待采集
#define REC_STA_IDLE		(2)		//空闲状态



//=======================CWatchDlg=========================================================

//显示模式
#define DISPLAY_MODE_TIME		 (0)
#define DISPLAY_MODE_FREQ		 (1)

//CWatchDlg尺寸定义
#define X_COORDINATE_HEIGHT 30
#define Y_COORDINATE_WIDTH	40
#define INTERVAL_Frame		15
#define Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT		35
#define Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT		35

#define BMP_MARK_SIZE		8

//CWatchDlg最大DrawWave的通道数
#define DRAW_CHNUM_MAX		4 

/*量程（包括时域和频域）*/
#define RANGE_CLOSED			(0)

#define RANGE_TIME_pn1V			(1)								//pn表示(Positive and negative)正负	e.g. n1V表示负1V
#define RANGE_TIME_pn5V			(5) 
#define RANGE_TIME_pn10V		(10) 
#define RANGE_TIME_pn20V		(20) 

#define RANGE_FREQ_V_0_500			(51) 
#define RANGE_FREQ_V_0_1000			(52) 

#define RANGE_FREQ_DBV_n100_0		(61)					//n代表negative负数 e.g. n100表示负100
#define RANGE_FREQ_DBV_n100_100		(62) 

#define RANGE_FREQ_DBu_0_100		(71) 
#define RANGE_FREQ_DBu_0_200		(72) 

#define RANGE_FREQ_DBm_n100_0		(81) 
#define RANGE_FREQ_DBm_n100_100		(82) 

//=======================CMeasureDlg=========================================================
//CMeasureDlg测量类型定义
#define MEATYPE_NORMAL_MAX		(0)
#define MEATYPE_NORMAL_MIN		(1)
#define MEATYPE_NORMAL_ACAVG	(2)

#define MEATYPE_FFT_AMPPEAK			(50)
#define MEATYPE_FFT_FREQPEAK		(51)


//CMeasureDlg测量范围定义
#define MEAPART_ALL				0
#define MEAPART_BET1AND2		1
#define MEAPART_LOOPBY1			2
#define MEAPART_LOOPBY2			3

//CMeasureDlg绘图尺寸定义
#define MEA_TITLE_HEIGHT		20						//标题栏高度
#define MEA_ITEM_HEIGHT			20						//每一项高度
#define MEA_HSCROLL_HEIGHT		15
#define MEA_COL_NUM				8


//=======================CMarkDlg=========================================================
//CMarkDlg绘图尺寸定义
#define MARK_TITLE_HEIGHT		20						//标题栏高度
#define MARK_ITEM_HEIGHT		15						//每一项高度

#define MARK_COL_NUM			4

#define MARK_MODE_LEFTONE_1		1
#define MARK_MODE_LEFTONE_2		2
#define MARK_MODE_RIGHTONE_1	3
#define MARK_MODE_RIGHTONE_2	4
#define MARK_MODE_LEFTTWO_1		5
#define MARK_MODE_LEFTTWO_2		6
#define MARK_MODE_RIGHTTWO_1	7
#define MARK_MODE_RIGHTTWO_2	8
#define MARK_MODE_MARKX_1		9
#define MARK_MODE_MARKX_2		10
#define MARK_MODE_SCALE_1		11
#define MARK_MODE_SCALE_2		12


//=======================CZoomDlg=========================================================
//CMarkDlg绘图尺寸定义
#define Zoom_TITLE_HEIGHT		20						//标题栏高度
#define Zoom_ITEM_HEIGHT		15						//每一项高度

#define Zoom_DEFAULT_HEIGHT		100						//ZoomDlg的初始化默认高  先做成一定大小
#define Zoom_DEFAULT_WIDTH		150						//ZoomDlg的初始化默认宽  先做成一定大小
#define Zoom_MIN_WIDTH			125						//ZoomDlg的最小化时宽  先做成一定大小








/////////////////////////////////////////////////////////////////////////////

/*采样信息*/
typedef struct
{
	int range;										//量程
	long samplingLength;								//采样长度
	int samplingRate;								//采样速率
	double samplingTime;							//samplingTime = samplingLength/samplingRate
	int trigMode;									//触发方式

	int trigChan;									//触发通道
	int trigLevel;									//触发电平

	int samplingDelay;								//采样延时

	int trigUpperLimit;								//通道触发：触发上限
	int trigLowerLimit;								//通道触发：触发下限

}AcqInfo;

/*播放控制*/
typedef struct
{
	int nInterval;									//取点间隔 nInterval * fXRatio = (10240*nSegmentNum)/nWidth
	double fXRatio;									//X轴放大倍数
	double fYRatio;									//Y轴放大倍数
	long nTotalPoint;								//总共的点数 nTotalPoint = samplingLength
	long nStartPointIndex;							//当前播放到的起点

	int nSpeed;
	
}PlayCtrlInfo;

/*产生的虚拟波形信息*/
typedef struct
{
	int nIndex;										//序号
	bool bOpened;									//是否使用
	int nWaveForm;									//波形		0：正弦波	1：方波
	int nFrequency;									//频率		单位：kHz
	int nAmpMax;									//最大振幅
	int nAmpMin;									//最小振幅
	
}VirtualCardChnInfo;



typedef struct  
{
	CRect rcDisplay;
	CRect rcReal;
	CRect rcPhy;								//rcPhy指的是真实数据矩形 
	
	CRect rcDrag;
	CPoint ptDisplayInReal;
	float fCurRatio;
	float fNewRatio;
	float fCurRatioX;
	float fNewRatioX;
	float fCurRatioY;
	float fNewRatioY;
	
	CPoint ptClickCorrd;
}WaveDisplayWnd;

typedef struct  
{

	float fRatioX;
	float fRatioY;

}WavePlayCtrl;


/*FFT变换设置*/
typedef struct
{
	int nFftBins;				//输出的信号的长度，长度是2的n次方
	int nWindowMode;			//加窗函数
	int nOutputMode;			//输出模式，对应YAXIS_UNIT_DBV，YAXIS_UNIT_DBu，YAXIS_UNIT_DBm
	int nImpedance;			//阻抗，和YAXIS_UNIT_DBm输出配合使用	单位：欧姆

}FftConf;
