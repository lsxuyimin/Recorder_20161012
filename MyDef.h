/////////////////////////////////
// �û���Ӳ���

//�л���˯��ʱ��
#define SLEEP_TIME			(200)


//������ʽ
#define TRIMODE_EXTERNAL	0
#define TRIMODE_MANUAL		1
#define TRIMODE_RISINGEDGE	2
#define TRIMODE_FALLINGEDGE 3
#define TRIMODE_CHANNEL		4			//ͨ������ �������޻�������޴���

//������
#define RATE_50M		0
#define RATE_10M		1
#define RATE_1M			2
#define RATE_100K		3
#define RATE_10K		4

//��������
#define LENGTH_1048576		0
#define LENGTH_262144		1
#define LENGTH_65536		2
#define LENGTH_16384		3
#define LENGTH_4096			4

//����--------------------------------->ԭ�����ڲɼ����������̣����������ˣ�ֻ��CWatchDlg��������ʾ������
#define RANGE_20V		0
#define RANGE_10V		1
#define RANGE_5V		2
#define RANGE_1V		3

//���ģʽ
#define MOUSEMODE_NONE		0
#define MOUSEMODE_ZOOMIN	1
#define MOUSEMODE_ZOOMOUT	2
#define MOUSEMODE_ZOOMDRAG	3
#define MOUSEMODE_RESTORE	4
#define MOUSEMODE_HAND		5
	
//�洢���
#define DATASTORAGE_SEGMENT		10240		//�� ������
#define DATASTORAGE_MAX_PERCHN	128			//MB

//�ɼ��豸������Ϣ
#define	CH_NUM_IN_PER_CARD		8			//��ǰ��ÿ��������8��ͨ��


//�ɼ�״̬
#define REC_STA_ACQEND		(0xFF)	//�ɼ�����
#define REC_STA_INACQ		(1)		//���ڲɼ�
#define REC_STA_WAITTRG		(0)		//�ȴ��ɼ�
#define REC_STA_IDLE		(2)		//����״̬



//=======================CWatchDlg=========================================================

//��ʾģʽ
#define DISPLAY_MODE_TIME		 (0)
#define DISPLAY_MODE_FREQ		 (1)

//CWatchDlg�ߴ綨��
#define X_COORDINATE_HEIGHT 30
#define Y_COORDINATE_WIDTH	40
#define INTERVAL_Frame		15
#define Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT		35
#define Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT		35

#define BMP_MARK_SIZE		8

//CWatchDlg���DrawWave��ͨ����
#define DRAW_CHNUM_MAX		4 

/*���̣�����ʱ���Ƶ��*/
#define RANGE_CLOSED			(0)

#define RANGE_TIME_pn1V			(1)								//pn��ʾ(Positive and negative)����	e.g. n1V��ʾ��1V
#define RANGE_TIME_pn5V			(5) 
#define RANGE_TIME_pn10V		(10) 
#define RANGE_TIME_pn20V		(20) 

#define RANGE_FREQ_V_0_500			(51) 
#define RANGE_FREQ_V_0_1000			(52) 

#define RANGE_FREQ_DBV_n100_0		(61)					//n����negative���� e.g. n100��ʾ��100
#define RANGE_FREQ_DBV_n100_100		(62) 

#define RANGE_FREQ_DBu_0_100		(71) 
#define RANGE_FREQ_DBu_0_200		(72) 

#define RANGE_FREQ_DBm_n100_0		(81) 
#define RANGE_FREQ_DBm_n100_100		(82) 

//=======================CMeasureDlg=========================================================
//CMeasureDlg�������Ͷ���
#define MEATYPE_NORMAL_MAX		(0)
#define MEATYPE_NORMAL_MIN		(1)
#define MEATYPE_NORMAL_ACAVG	(2)

#define MEATYPE_FFT_AMPPEAK			(50)
#define MEATYPE_FFT_FREQPEAK		(51)


//CMeasureDlg������Χ����
#define MEAPART_ALL				0
#define MEAPART_BET1AND2		1
#define MEAPART_LOOPBY1			2
#define MEAPART_LOOPBY2			3

//CMeasureDlg��ͼ�ߴ綨��
#define MEA_TITLE_HEIGHT		20						//�������߶�
#define MEA_ITEM_HEIGHT			20						//ÿһ��߶�
#define MEA_HSCROLL_HEIGHT		15
#define MEA_COL_NUM				8


//=======================CMarkDlg=========================================================
//CMarkDlg��ͼ�ߴ綨��
#define MARK_TITLE_HEIGHT		20						//�������߶�
#define MARK_ITEM_HEIGHT		15						//ÿһ��߶�

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
//CMarkDlg��ͼ�ߴ綨��
#define Zoom_TITLE_HEIGHT		20						//�������߶�
#define Zoom_ITEM_HEIGHT		15						//ÿһ��߶�

#define Zoom_DEFAULT_HEIGHT		100						//ZoomDlg�ĳ�ʼ��Ĭ�ϸ�  ������һ����С
#define Zoom_DEFAULT_WIDTH		150						//ZoomDlg�ĳ�ʼ��Ĭ�Ͽ�  ������һ����С
#define Zoom_MIN_WIDTH			125						//ZoomDlg����С��ʱ��  ������һ����С








/////////////////////////////////////////////////////////////////////////////

/*������Ϣ*/
typedef struct
{
	int range;										//����
	long samplingLength;								//��������
	int samplingRate;								//��������
	double samplingTime;							//samplingTime = samplingLength/samplingRate
	int trigMode;									//������ʽ

	int trigChan;									//����ͨ��
	int trigLevel;									//������ƽ

	int samplingDelay;								//������ʱ

	int trigUpperLimit;								//ͨ����������������
	int trigLowerLimit;								//ͨ����������������

}AcqInfo;

/*���ſ���*/
typedef struct
{
	int nInterval;									//ȡ���� nInterval * fXRatio = (10240*nSegmentNum)/nWidth
	double fXRatio;									//X��Ŵ���
	double fYRatio;									//Y��Ŵ���
	long nTotalPoint;								//�ܹ��ĵ��� nTotalPoint = samplingLength
	long nStartPointIndex;							//��ǰ���ŵ������

	int nSpeed;
	
}PlayCtrlInfo;

/*���������Ⲩ����Ϣ*/
typedef struct
{
	int nIndex;										//���
	bool bOpened;									//�Ƿ�ʹ��
	int nWaveForm;									//����		0�����Ҳ�	1������
	int nFrequency;									//Ƶ��		��λ��kHz
	int nAmpMax;									//������
	int nAmpMin;									//��С���
	
}VirtualCardChnInfo;



typedef struct  
{
	CRect rcDisplay;
	CRect rcReal;
	CRect rcPhy;								//rcPhyָ������ʵ���ݾ��� 
	
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


/*FFT�任����*/
typedef struct
{
	int nFftBins;				//������źŵĳ��ȣ�������2��n�η�
	int nWindowMode;			//�Ӵ�����
	int nOutputMode;			//���ģʽ����ӦYAXIS_UNIT_DBV��YAXIS_UNIT_DBu��YAXIS_UNIT_DBm
	int nImpedance;			//�迹����YAXIS_UNIT_DBm������ʹ��	��λ��ŷķ

}FftConf;
