#ifndef _HQ_STRUCT_H_
#define _HQ_STRUCT_H_

#include <hqtransdef.h>
#include <datatype/datatype.h>
#include <lcomm/publicdef.h>
#include <lcomm/lmapstruct.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct
{
	int64 nSendNum;
	int64 nRecvNum;
}SEND_RECV_COUNT;

typedef struct
{
    char cProductClass;
	int  nMarket;
	char szCode[MAX_SRC_LEN];
	char szName[MAX_SRC_NAME_LEN];
}CTP_INSTR_INFO;
typedef CLMapStruct<std::string, CTP_INSTR_INFO > MAP_CODE_INFO;

#pragma pack(push, 1)

  typedef struct
{
    // ZY表示订单队列
    char            szPreFix[MAX_BASEBUF_LEN];         // 证券信息前置
    char            szSCR_CODE[MAX_SRC_LEN];           // 证券代码
    char            szSCR_NAME[MAX_SRC_NAME_LEN];      // 证券名称
	char            szCONTRACT_ID[MAX_BASEBUF_LEN];    // 交易代码
    int64           i64TM;                             // 行情时间
    int64           i64YSTD_CLS_QTN_PRICE;             // 昨收盘价
    int64           i64YSTD_SETL_PRICE;                // 昨结算
    int64           i64OPN_QTN_PRICE;                  // 今开盘价
    int64           i64TDY_CLS_QTN_PRICE;              // 今收盘
    int64           i64TDY_SETL_PRICE;                 // 今结算
    int64           i64HIGH_PRICE;                     // 最高价
	int64           i64LWS_PRICE;                      // 最低价
	int64           i64LTST_PRICE;                     // 最新价、逐笔价格(成交价格 委托价格 ZY价格)
    int64           i64APL_SELL_PRC[MQ_LEVEL_TWO];     // 申卖价
	int64           i64APL_SELL_AMT[MQ_LEVEL_TWO];     // 申卖量
	int64           i64APL_BID_PRC[MQ_LEVEL_TWO];      // 申买价
	int64           i64APL_BID_AMT[MQ_LEVEL_TWO];      // 申买量
	int64           i64BGN_CNT;                        // 成交笔数、逐笔(ZY数据笔数)
	int64           i64BGN_TOT_NUM;                    // 成交总量、逐笔数量(成交数量 委托数量 ZY订单数量)
	int64           i64BGN_TOT_AMT;                    // 成交总金额、逐笔成交金额
	int64           i64ENTRT_BUY_TOT_NUM;              // 委托买入总量
	int64           i64ENTRT_SELL_TOT_NUM;             // 委托卖出总量
	int64           i64WGHT_AVG_ENTRT_BUY_PRC;         // 加权平均委买价格
	int64           i64WGHT_AVG_ENTRT_SELL_PRC;        // 加权平均委卖价格
	int64           i64IOPV_NET_VAL_VALT;              // IOPV净值估值
	int64           i64MTUR_YLD;                       // 到期收益率(权证溢价率)
	int64           i64RISE_LMT_PRICE;                 // 涨停价
	int64           i64FALL_LMT_PRICE;                 // 跌停价
	int64           i64STD_MKT_MAKE_POS_TOT_NUM;       // 昨持仓
	int64           i64TDY_MKT_MAKE_POS_TOT_NUM;       // 持仓总量(合约持仓量)
	int64           i64YSTD_DELTA;                     // 昨虚实度
	int64           i64TDY_DELTA;                      // 今虚实度
	int64           i64DAY_AVERAGE_PRICE;              // 当日均价
	int64           i64PE_RATIO_1;                     // 市盈率1
	int64           i64PE_RATIO_2;                     // 市盈率2
	int64           i64PRICE_NC_1;                     // 价格升跌1
	int64           i64PRICE_NC_2;                     // 价格升跌2
	int64           i64ERROR_MARK;                     // 错误标识
	int64           i64PART_SNPST_ST_TP_CODE;          // 状态
    int64           i64AUCTION_PRICE;                  // 动态参考价格(期权)
	int64           i64AUCTION_QTY;                    // 虚拟匹配数量(期权)
    int64           i64TOT_LONG_POSITION;              // 当前合约未平仓数(期权)
    char            szTRADE_PHASE_CODE[MAX_BASEBUF_LEN]; // 产品实施阶段及标志
    /*********************L2行情*****************************/
    int64           i64ID;                              // 成交编号 委托编号
	char            szBSFlag[MAX_BASEBUF_LEN];          // 买卖方向
	char            szOrderKind[MAX_BASEBUF_LEN];       // 成交类别
	int64           i64ENTRT_NBR[MAX_ENTRT_NBR];        // ZY委托明细数量
    /*********************发布信息*****************************/
    int64           i64ID_NUM;                       // 本日编号
    bool            bTestHq;                         // 是否为测试行情
	int             nDate;                           // 行情日期
	char            cHqSrc;                          // 行情源：DBF 或 CTP 等
	int             nHqType;                         // 行情类别：股票、指数、期货、逐笔等
	int             nMQLevel;                        // 行情级别：五档，十档
	int             nStkType;                        // 具体证券类型
	int64           nPublishTM1;                     // 一级发布时间
	int64           nPublishTM2;                     // 一级发布时间
	int             nMKT_TP_CODE;                    // 市场
	/*********************保留******************************/
    char            szReserve[MAX_BASEBUF_LEN];
    int64           i64Reserve;                      // (基金T-1日净值)
}HQ_INNER, *PHQ_INNER;

typedef struct
{
    int             nMKT_TP_CODE;                    // 交易市场
    char            szSCR_CODE[MAX_SRC_LEN];         // 合约编码
    char            szSCR_NAME[MAX_SRC_NAME_LEN];    // 证券名称
    int64           i64YSTD_CLS_QTN_PRICE;           // 昨收盘价
    int64           i64YSTD_SETL_PRICE;              // 昨结算
    int64           i64RISE_LMT_PRICE;               // 涨停价
    int64           i64FALL_LMT_PRICE;               // 跌停价
}STKOP_CODETABLE, *PSTKOP_CODETABLE;

typedef struct
{
	char	szZQDM[MAX_SRC_LEN];			// 股票代码
	char	szZQJC[MAX_SRC_NAME_LEN];		// 股票名称
	char    szYWJC[MAX_SRC_NAME_LEN];       // 英文简称
	float	fZRSP;				            // 昨收
	bool	bStopTrade;			            // 是否停盘
	char    cTPBZ;
	char	szLastHq[MAX_BUF_LEN];		    // 上次数据库中的信息
	char    szLastXX[MAX_BUF_LEN];          // 上次信息文件中的数据
	int64   i64ZTJG;  // 涨停价格
    int64   i64DTJG;  // 跌停价格
    bool    bSendXX;
    unsigned int unVersion;
    int     nHqType;
}DBFREALHQ, *LPDBFREALHQ;

typedef struct
{
	int  nMarket;                        // 市场
	int  nPort;                          // 服务器端口
	int  nScanDelay;                     // 扫描dbf延时
	int  nLevel;			             // 连接优先级.
	int  nProxyType;		             // 使用的代理类型.无代理时为零
	int  nProxyPort;	                 // 代理端口, 无代理时为0
	int  nConnTimeOut;                   // 连接超时
	char szServerId[MAX_SERVERID_LEN];   // 服务器地址
	char szUser[MAX_USER_LEN];           // 用户名
	char szPassword[MAX_PWD_LEN];        // 密码
	int  nType;                          // 服务器类型
	char szProxyHost[MAX_BASEBUF_LEN];	 // 代理地址, 无代理时为""
	char szProxyUser[MAX_USER_LEN];	     // 代理用户名, 无代理时为""
	char szProxyPass[MAX_PWD_LEN];	     // 代理密码),无代理时为""
	char szHQDbfPath[MAX_FILE_PATH];          // 行情dbf路径
	char szXXDbfPath[MAX_FILE_PATH];          // 信息dbf路径
	char szBrokerId[MAX_USER_LEN];       // CTP经济公司代码
	// 备用服务器参数
	int  nBType;                          // 备用服务器类型
	int  nBPort;                          // 备用服务器端口
	char szBServerId[MAX_SERVERID_LEN];   // 备用服务器地址
	char szBUser[MAX_USER_LEN];           // 备用用户名
	char szBPassword[MAX_PWD_LEN];        // 备用密码


}PLU_PARAMS, *PPLU_PARAMS;

typedef struct
{
	int  nWorkType;                       // 工作类型
	char szHqType[MAX_HQ_TYPE_LEN];       // 行情类型
	int  nMarket;                         // 市场
	char szSecurityID[MAX_SRC_LEN];       // 证券标识
	char cSourceID;                       // 数据来源
	char szPublishID[MAX_PUBLISH_ID_LEN]; // 发布节点
}QTE_TOPIC, *LQTE_TOPIC;

typedef struct
{
    char            szSCR_CODE[MAX_SRC_LEN];         // 合约编码
    int             nMKT_TP_CODE;                    // 交易市场
    int             nMQLevel;                        // 行情级别：L1或L2
    char            cHqSrc;                          // 行情源
    int             nHqType;                         // 行情类别：股票、指数、期货、逐笔等
    void            *pData;                          // 行情数据
}BASE_HQ_INNER, *PBASE_HQ_INNER;


// 沪港通exra
typedef struct
{
	char	szDataType[MAX_SRC_NAME_LEN];
	char	szBuyPrice[MAX_SRC_NAME_LEN];
	char	szSellPrice[MAX_SRC_NAME_LEN];
	char	szNormalPrice[MAX_SRC_NAME_LEN];
	char	szMoneyType[MAX_SRC_NAME_LEN];
	char	szTxt[MAX_SRC_NAME_LEN]; // 备用字段
	int		nDate;
	char	cHqSrc;			// 数据源
	int		nMKT_TP_CODE;	// 市场
	int		nHqType;	//	数据类型
}HK_EXRA;

// 沪港通reff
typedef struct
{
	int		nHqType;	//	数据类型
	char	szRFSTREAMID[MAX_SRC_LEN];	// 数据类型(字符形式)
	char	szZQDM[MAX_SRC_LEN];			// 股票代码
	char	szISINDM[MAX_SRC_LEN];			// ISIN代码
	char	szZQJC[MAX_SRC_NAME_LEN];		// 股票中文名称todo
	char	szZQJCEN[MAX_SRC_LEN];		// 英文简称todo
	char	szZQQCEN[MAX_SRC_LEN];		// 英文全称
	char	szUnderSecID[MAX_SRC_NAME_LEN]; // 辅助证券代码
	char	szMarket[MAX_SRC_NAME_LEN]; // 市场种类
	char	szSecurityType[MAX_SRC_NAME_LEN]; // 证券类别
	char	szCurrency[MAX_SRC_NAME_LEN];	// 货币种类
	char	szPerValueCurrency[MAX_SRC_NAME_LEN];	// 面值货币种类
	int		iAmountTimes;	// 货币单位 当前收盘价有关
	char	szPerValue[MAX_SRC_LEN];	// 面值todo
	char	szInterest[MAX_SRC_LEN];   // 利息
	int64	i64IssueDate; //上市日期
	int		nRoundLot;  // 买卖单位
	char	szPreClosePx[MAX_SRC_NAME_LEN];	// 当前收盘价
	char	szTxt[MAX_SRC_NAME_LEN]; // 保留字段
	char	szSecurityStatusFlag[MAX_SRC_NAME_LEN]; // 产品状态信息
	char	cHqSrc;			// 数据源
	int		nMKT_TP_CODE;	// 市场

}HK_REFF;


typedef struct
{

	char	szMDStreamID[MAX_SRC_NAME_LEN];
	int64	i64ThresholdAmount;
	int64	i64PosAmt;
	int		iAmountStatus;
	char    szLastSt[MAX_SRC_NAME_LEN];
}HK_TRDS402;

typedef struct
{
	char	szMDStreamID[MAX_SRC_NAME_LEN];
	char	szZQDM[MAX_SRC_NAME_LEN];
	char	szSecTradingStatus1[MAX_SRC_NAME_LEN];
	char	szSecTradingStatus2[MAX_SRC_NAME_LEN];
	char    szLastSt[MAX_SRC_NAME_LEN];

}HK_TRDS403;

// 是402与403的集合体
typedef struct
{
	int		ntype;
	char	szMDStreamID[MAX_SRC_NAME_LEN];
	int64	i64ThresholdAmount;
	int64	i64PosAmt;
	int		iAmountStatus;

	char	szZQDM[MAX_SRC_NAME_LEN];
	char	szSecTradingStatus1[MAX_SRC_NAME_LEN];
	char	szSecTradingStatus2[MAX_SRC_NAME_LEN];
	char    szLastSt[MAX_SRC_NAME_LEN];

	int64   i64Date;
	int		nMarketId;

}HK_TRDS;

typedef struct
{
    int  nMARKET;      // 市场
    char szCURRENCY[2];    // 货币
    char szSECU_NAME[MAX_SRC_NAME_LEN];  // 证券代码
    char szSECU_CODE[MAX_SRC_LEN];       // 证券名称
    int  nSPREAD;
    char szSECU_STATUS[2];
    int  nTRD_FLOOR;
    int  nTRD_CEILING;
    int64 i64FACE_VAL;
    int64 i64PRICE_CEILING;
    int64 i64PRICE_FLOOR;
    char  szSUSP_FLAG[2];
    char  szMKT_VAL_FLAG[2];
    char  szSECU_CLS[2];
    char  szTRADES[MAX_BASEBUF_LEN];
    char  szDELIST_DATE[MAX_BASEBUF_LEN];
    char  szWARNING_INFO[MAX_BASEBUF_LEN];
    char  szYWJC[MAX_SRC_NAME_LEN];  // 英文简称
    uint32 unCODETABLE_VER;

    int64 i64YSTD_CLS_QTN_PRICE;
    int64 i64OPN_QTN_PRICE;
}SECU_INFO;

/*
typedef struct
{
    int   nMarket;                               // 交易市场
    char  cMoneytype;                            // 货币
    char  szStkname[MAX_SRC_NAME_LEN];           // 证券名称
    char  szStkcode[MAX_SRC_LEN];                // 证券代码
    char  cStktype;                              // 证券类别
    int   nPriceunit;                            // 价位
    int64 i64Ticketprice;                        // 面值
    char  cBsflag;                               // 买卖标志,      'A' 买卖 'B' 只买 'S' 只卖 'N' 不可买卖
    char  cLimitpriceflag;                       // 限价标志 0 无限制，1 涨跌幅度，2 档位幅度
	char  cDefaultpricetype;                     // 缺省价格类型 0 昨收盘，1 今开盘，2 最新价 3 面值
	int   nMaxriserate;                          // 涨幅比例, (0.01的倍数)
	int   nMaxdownrate;                          // 跌幅比例, (0.01的倍数)
    int64 i64Maxrisevalue;                       // 涨停价格
    int64 i64Maxdownvalue;                       // 跌停价格
    char  cStopflag;                             // 停牌标志
    char  cMtkcalflag;                           // 市值计算标识
    int64 i64Bondintr;                           // 债券应计利息
    int   nMaxqty;                               // 最高数量
    int   nMinqty;                               // 最低数量
    int   nBuyunit;                              // 买入最小单位
    int   nSaleunit;                             // 卖出最小单位
    char  cStkstatus;                            // 证券状态
    char  cStklevel;                             // 证券级别
    char  cTrdid;                                // 交易类型
    int   nQuitdate;                             // 退市证券交易截止日期
    char  szMemotext[MAX_BASEBUF_LEN];           // 退市证券提示信息
    unsigned int unCODETABLE_VER;                // 版本号
}STK_INFO;
*/
struct STK_INFO{

    int   nMarket;                               // 交易市场
    char  szStkname[MAX_SRC_NAME_LEN];           // 证券名称
    char  szStkcode[MAX_SRC_LEN];                // 证券代码
    char  szStopflag[2];      // 停牌标志, F-正常、T-停牌
    char  szStklevel[2];       // 证券级别
    char  szStkstatus[2];      // 证券状态
    int   nQuitdate;          // 退市证券交易截止日期，如证券近期将退市，则可设置具体日期，否则记为0
    int   nDays;               // 出现退市后经过的天数
    char  szMemotext[MAX_BASEBUF_LEN];         // 退市证券交易风险提示内容
    int64 i64Maxrisevalue;      // 最大涨幅, 比例数或跌幅最大绝对值
    int64 i64Maxdownvalue;      // 最大跌幅, 比例数或涨幅最大绝对值
    // add
    int         nBegstkcode;             //开始证券代码
    int         nEndstkcode;             //结束证券代码
	char        szStktype[8];                //证券类别
	char        szTrdid[8];                  //交易类型
	char        szMoneytype[8];              //货币代码
	char        szLimitpriceflag[8];         //限价标志 0 无限制，1 涨跌幅度，2 档位幅度
	char        szDefaultpricetype[8];       //缺省价格类型 0 昨收盘，1 今开盘，2 最新价 3 面值
	int         nMaxriserate;            //涨幅比例, (0.01的倍数)
	int         nMaxdownrate;            //跌幅比例, (0.01的倍数)
	char        szPt_ordertype[8];        //允许订单类型 0 限价委托 1 市价委托 ...(数据字典定义)
	char        szKeepmode[8];               //托管方式, (0：券商 1：交易所 2：双向)
	int64       i64Ticketprice;           //面值
	char        szMtkcalflag[8];             //计算市值标志, 0-不计算、1-计算
	char        szBsflag[8];                 //买卖标志,      'A' 买卖 'B' 只买 'S' 只卖 'N' 不可买卖
	char        szCancelflag[8];             //撤单标志,      '0' 不允许撤单 '1' 允许撤单 '2' 允许重复撤单
	char        szPriceflag[8];              //委托价格标志,  '0' 客户委托价 '1' 系统定价,忽略用户委托价 '2' 系统定价,检查用户委托价
	char        szStkoverdraw[8];            //新指定可卖空标志
	char        szFundrealback[8];           //实时回报资金标志, 0 卖出资金不可用，    1 卖出资金当天可用
	char        szStockrealback[8];          //实时回报证券标志, 0 买入证券当天不可用，1 买入证当天可用
	int         nSettlefundays;          //资金交收天数, (T+N)
	int         nSettlestkdays;          //证券交收天数, (T+N)
	char        szFunduseflag[8];            //未交收资金使用标志,  '0' 使用 '1' 不使用
	char        szStkusedflag[8];            //未交收证券使用标志,  '0' 使用 '1' 不使用
	int         nPriceunit;              //价位, (0.001)
	int         nBuyunit;                //买入单位, (最小单位1股1张,系统余额存储按最小单位存储)
	int         nSaleunit;               //卖出单位
	int         nExchunit;               //报盘单位
	int         nMaxqty;                 //交易最高数量
	int         nMinqty;                 //交易最低数量
	char        szChkmtvalueflag[8];          //交易市值额度校验标志(默认校验), '0' 不校验 '1' 校验 SPB-2326,ganz,20131207,新股发行
};




// 证券状态
#define ZQZT_N "N"   // 通常状态
#define ZQZT_Y "Y"   // 首日上市
#define ZQZT_Z "Z"   // 增发股份上市
#define ZQZT_F "F"   // 证券上网定价发行
#define ZQZT_I "I"   // 证券上网竞价发行
#define ZQZT_P "P"   // 债券挂牌分销
#define ZQZT_D 'D'   // 股票挂牌转让


#define DEFAULT_WARNING_INFO  "股市有风险，投资需谨慎"

#pragma  pack(pop)

#endif


