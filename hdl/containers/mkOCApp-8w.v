// The container RTL for an app with 3 workers and no adc or memory connections
module mkOCApp4B(RST_N_rst_0,
	       RST_N_rst_1,
	       RST_N_rst_2,
	       RST_N_rst_3,
	       RST_N_rst_4,
	       RST_N_rst_5,
	       RST_N_rst_6,
	       RST_N_rst_7,
	       CLK,
	       RST_N,
	       wci_s_0_MCmd,
	       wci_s_0_MAddrSpace,
	       wci_s_0_MByteEn,
	       wci_s_0_MAddr,
	       wci_s_0_MData,
	       wci_s_0_SResp,
	       wci_s_0_SData,
	       wci_s_0_SThreadBusy,
	       wci_s_0_SFlag,
	       wci_s_0_MFlag,
	       wci_s_1_MCmd,
	       wci_s_1_MAddrSpace,
	       wci_s_1_MByteEn,
	       wci_s_1_MAddr,
	       wci_s_1_MData,
	       wci_s_1_SResp,
	       wci_s_1_SData,
	       wci_s_1_SThreadBusy,
	       wci_s_1_SFlag,
	       wci_s_1_MFlag,
	       wci_s_2_MCmd,
	       wci_s_2_MAddrSpace,
	       wci_s_2_MByteEn,
	       wci_s_2_MAddr,
	       wci_s_2_MData,
	       wci_s_2_SResp,
	       wci_s_2_SData,
	       wci_s_2_SThreadBusy,
	       wci_s_2_SFlag,
	       wci_s_2_MFlag,
	       wci_s_3_MCmd,
	       wci_s_3_MAddrSpace,
	       wci_s_3_MByteEn,
	       wci_s_3_MAddr,
	       wci_s_3_MData,
	       wci_s_3_SResp,
	       wci_s_3_SData,
	       wci_s_3_SThreadBusy,
	       wci_s_3_SFlag,
	       wci_s_3_MFlag,
	       wci_s_4_MCmd,
	       wci_s_4_MAddrSpace,
	       wci_s_4_MByteEn,
	       wci_s_4_MAddr,
	       wci_s_4_MData,
	       wci_s_4_SResp,
	       wci_s_4_SData,
	       wci_s_4_SThreadBusy,
	       wci_s_4_SFlag,
	       wci_s_4_MFlag,
	       wci_s_5_MCmd,
	       wci_s_5_MAddrSpace,
	       wci_s_5_MByteEn,
	       wci_s_5_MAddr,
	       wci_s_5_MData,
	       wci_s_5_SResp,
	       wci_s_5_SData,
	       wci_s_5_SThreadBusy,
	       wci_s_5_SFlag,
	       wci_s_5_MFlag,
	       wci_s_6_MCmd,
	       wci_s_6_MAddrSpace,
	       wci_s_6_MByteEn,
	       wci_s_6_MAddr,
	       wci_s_6_MData,
	       wci_s_6_SResp,
	       wci_s_6_SData,
	       wci_s_6_SThreadBusy,
	       wci_s_6_SFlag,
	       wci_s_6_MFlag,
	       wci_s_7_MCmd,
	       wci_s_7_MAddrSpace,
	       wci_s_7_MByteEn,
	       wci_s_7_MAddr,
	       wci_s_7_MData,
	       wci_s_7_SResp,
	       wci_s_7_SData,
	       wci_s_7_SThreadBusy,
	       wci_s_7_SFlag,
	       wci_s_7_MFlag,
	       wti_s_0_MCmd,
	       wti_s_0_MData,
	       wti_s_0_SThreadBusy,
	       wti_s_0_SReset_n,
	       wti_s_1_MCmd,
	       wti_s_1_MData,
	       wti_s_1_SThreadBusy,
	       wti_s_1_SReset_n,
	       wti_s_2_MCmd,
	       wti_s_2_MData,
	       wti_s_2_SThreadBusy,
	       wti_s_2_SReset_n,
	       wmiM0_MCmd,
	       wmiM0_MReqLast,
	       wmiM0_MReqInfo,
	       wmiM0_MAddrSpace,
	       wmiM0_MAddr,
	       wmiM0_MBurstLength,
	       wmiM0_MDataValid,
	       wmiM0_MDataLast,
	       wmiM0_MData,
	       wmiM0_MDataByteEn,
	       wmiM0_SResp,
	       wmiM0_SData,
	       wmiM0_SThreadBusy,
	       wmiM0_SDataThreadBusy,
	       wmiM0_SRespLast,
	       wmiM0_SFlag,
	       wmiM0_MFlag,
	       wmiM0_MReset_n,
	       wmiM0_SReset_n,
	       wmiM1_MCmd,
	       wmiM1_MReqLast,
	       wmiM1_MReqInfo,
	       wmiM1_MAddrSpace,
	       wmiM1_MAddr,
	       wmiM1_MBurstLength,
	       wmiM1_MDataValid,
	       wmiM1_MDataLast,
	       wmiM1_MData,
	       wmiM1_MDataByteEn,
	       wmiM1_SResp,
	       wmiM1_SData,
	       wmiM1_SThreadBusy,
	       wmiM1_SDataThreadBusy,
	       wmiM1_SRespLast,
	       wmiM1_SFlag,
	       wmiM1_MFlag,
	       wmiM1_MReset_n,
	       wmiM1_SReset_n,
	       wmemiM0_MCmd,
	       wmemiM0_MReqLast,
	       wmemiM0_MAddr,
	       wmemiM0_MBurstLength,
	       wmemiM0_MDataValid,
	       wmemiM0_MDataLast,
	       wmemiM0_MData,
	       wmemiM0_MDataByteEn,
	       wmemiM0_SResp,
	       wmemiM0_SRespLast,
	       wmemiM0_SData,
	       wmemiM0_SCmdAccept,
	       wmemiM0_SDataAccept,
	       wmemiM0_MReset_n,
	       wsi_s_adc_MCmd,
	       wsi_s_adc_MReqLast,
	       wsi_s_adc_MBurstPrecise,
	       wsi_s_adc_MBurstLength,
	       wsi_s_adc_MData,
	       wsi_s_adc_MByteEn,
	       wsi_s_adc_MReqInfo,
	       wsi_s_adc_SThreadBusy,
	       wsi_s_adc_SReset_n,
	       wsi_s_adc_MReset_n,
	       wsi_m_dac_MCmd,
	       wsi_m_dac_MReqLast,
	       wsi_m_dac_MBurstPrecise,
	       wsi_m_dac_MBurstLength,
	       wsi_m_dac_MData,
	       wsi_m_dac_MByteEn,
	       wsi_m_dac_MReqInfo,
	       wsi_m_dac_SThreadBusy,
	       wsi_m_dac_MReset_n,
	       wsi_m_dac_SReset_n,
	       uuid,
	       rom_en,
	       rom_addr,
	       rom_data);
  parameter [0 : 0] hasDebugLogic = 1'b0;
  input  RST_N_rst_0;
  input  RST_N_rst_1;
  input  RST_N_rst_2;
  input  RST_N_rst_3;
  input  RST_N_rst_4;
  input  RST_N_rst_5;
  input  RST_N_rst_6;
  input  RST_N_rst_7;
  input  CLK;
  input  RST_N;

  // action method wci_s_0_mCmd
  input  [2 : 0] wci_s_0_MCmd;

  // action method wci_s_0_mAddrSpace
  input  wci_s_0_MAddrSpace;

  // action method wci_s_0_mByteEn
  input  [3 : 0] wci_s_0_MByteEn;

  // action method wci_s_0_mAddr
  input  [31 : 0] wci_s_0_MAddr;

  // action method wci_s_0_mData
  input  [31 : 0] wci_s_0_MData;

  // value method wci_s_0_sResp
  output [1 : 0] wci_s_0_SResp;

  // value method wci_s_0_sData
  output [31 : 0] wci_s_0_SData;

  // value method wci_s_0_sThreadBusy
  output wci_s_0_SThreadBusy;

  // value method wci_s_0_sFlag
  output [1 : 0] wci_s_0_SFlag;

  // action method wci_s_0_mFlag
  input  [1 : 0] wci_s_0_MFlag;

  // action method wci_s_1_mCmd
  input  [2 : 0] wci_s_1_MCmd;

  // action method wci_s_1_mAddrSpace
  input  wci_s_1_MAddrSpace;

  // action method wci_s_1_mByteEn
  input  [3 : 0] wci_s_1_MByteEn;

  // action method wci_s_1_mAddr
  input  [31 : 0] wci_s_1_MAddr;

  // action method wci_s_1_mData
  input  [31 : 0] wci_s_1_MData;

  // value method wci_s_1_sResp
  output [1 : 0] wci_s_1_SResp;

  // value method wci_s_1_sData
  output [31 : 0] wci_s_1_SData;

  // value method wci_s_1_sThreadBusy
  output wci_s_1_SThreadBusy;

  // value method wci_s_1_sFlag
  output [1 : 0] wci_s_1_SFlag;

  // action method wci_s_1_mFlag
  input  [1 : 0] wci_s_1_MFlag;

  // action method wci_s_2_mCmd
  input  [2 : 0] wci_s_2_MCmd;

  // action method wci_s_2_mAddrSpace
  input  wci_s_2_MAddrSpace;

  // action method wci_s_2_mByteEn
  input  [3 : 0] wci_s_2_MByteEn;

  // action method wci_s_2_mAddr
  input  [31 : 0] wci_s_2_MAddr;

  // action method wci_s_2_mData
  input  [31 : 0] wci_s_2_MData;

  // value method wci_s_2_sResp
  output [1 : 0] wci_s_2_SResp;

  // value method wci_s_2_sData
  output [31 : 0] wci_s_2_SData;

  // value method wci_s_2_sThreadBusy
  output wci_s_2_SThreadBusy;

  // value method wci_s_2_sFlag
  output [1 : 0] wci_s_2_SFlag;

  // action method wci_s_2_mFlag
  input  [1 : 0] wci_s_2_MFlag;

  // action method wci_s_3_mCmd
  input  [2 : 0] wci_s_3_MCmd;

  // action method wci_s_3_mAddrSpace
  input  wci_s_3_MAddrSpace;

  // action method wci_s_3_mByteEn
  input  [3 : 0] wci_s_3_MByteEn;

  // action method wci_s_3_mAddr
  input  [31 : 0] wci_s_3_MAddr;

  // action method wci_s_3_mData
  input  [31 : 0] wci_s_3_MData;

  // value method wci_s_3_sResp
  output [1 : 0] wci_s_3_SResp;

  // value method wci_s_3_sData
  output [31 : 0] wci_s_3_SData;

  // value method wci_s_3_sThreadBusy
  output wci_s_3_SThreadBusy;

  // value method wci_s_3_sFlag
  output [1 : 0] wci_s_3_SFlag;

  // action method wci_s_3_mFlag
  input  [1 : 0] wci_s_3_MFlag;

  // action method wci_s_4_mCmd
  input  [2 : 0] wci_s_4_MCmd;

  // action method wci_s_4_mAddrSpace
  input  wci_s_4_MAddrSpace;

  // action method wci_s_4_mByteEn
  input  [3 : 0] wci_s_4_MByteEn;

  // action method wci_s_4_mAddr
  input  [31 : 0] wci_s_4_MAddr;

  // action method wci_s_4_mData
  input  [31 : 0] wci_s_4_MData;

  // value method wci_s_4_sResp
  output [1 : 0] wci_s_4_SResp;

  // value method wci_s_4_sData
  output [31 : 0] wci_s_4_SData;

  // value method wci_s_4_sThreadBusy
  output wci_s_4_SThreadBusy;

  // value method wci_s_4_sFlag
  output [1 : 0] wci_s_4_SFlag;

  // action method wci_s_4_mFlag
  input  [1 : 0] wci_s_4_MFlag;

  // action method wci_s_5_mCmd
  input  [2 : 0] wci_s_5_MCmd;

  // action method wci_s_5_mAddrSpace
  input  wci_s_5_MAddrSpace;

  // action method wci_s_5_mByteEn
  input  [3 : 0] wci_s_5_MByteEn;

  // action method wci_s_5_mAddr
  input  [31 : 0] wci_s_5_MAddr;

  // action method wci_s_5_mData
  input  [31 : 0] wci_s_5_MData;

  // value method wci_s_5_sResp
  output [1 : 0] wci_s_5_SResp;

  // value method wci_s_5_sData
  output [31 : 0] wci_s_5_SData;

  // value method wci_s_5_sThreadBusy
  output wci_s_5_SThreadBusy;

  // value method wci_s_5_sFlag
  output [1 : 0] wci_s_5_SFlag;

  // action method wci_s_5_mFlag
  input  [1 : 0] wci_s_5_MFlag;

  // action method wci_s_6_mCmd
  input  [2 : 0] wci_s_6_MCmd;

  // action method wci_s_6_mAddrSpace
  input  wci_s_6_MAddrSpace;

  // action method wci_s_6_mByteEn
  input  [3 : 0] wci_s_6_MByteEn;

  // action method wci_s_6_mAddr
  input  [31 : 0] wci_s_6_MAddr;

  // action method wci_s_6_mData
  input  [31 : 0] wci_s_6_MData;

  // value method wci_s_6_sResp
  output [1 : 0] wci_s_6_SResp;

  // value method wci_s_6_sData
  output [31 : 0] wci_s_6_SData;

  // value method wci_s_6_sThreadBusy
  output wci_s_6_SThreadBusy;

  // value method wci_s_6_sFlag
  output [1 : 0] wci_s_6_SFlag;

  // action method wci_s_6_mFlag
  input  [1 : 0] wci_s_6_MFlag;

  // action method wci_s_7_mCmd
  input  [2 : 0] wci_s_7_MCmd;

  // action method wci_s_7_mAddrSpace
  input  wci_s_7_MAddrSpace;

  // action method wci_s_7_mByteEn
  input  [3 : 0] wci_s_7_MByteEn;

  // action method wci_s_7_mAddr
  input  [31 : 0] wci_s_7_MAddr;

  // action method wci_s_7_mData
  input  [31 : 0] wci_s_7_MData;

  // value method wci_s_7_sResp
  output [1 : 0] wci_s_7_SResp;

  // value method wci_s_7_sData
  output [31 : 0] wci_s_7_SData;

  // value method wci_s_7_sThreadBusy
  output wci_s_7_SThreadBusy;

  // value method wci_s_7_sFlag
  output [1 : 0] wci_s_7_SFlag;

  // action method wci_s_7_mFlag
  input  [1 : 0] wci_s_7_MFlag;

  // action method wti_s_0_mCmd
  input  [2 : 0] wti_s_0_MCmd;

  // action method wti_s_0_mData
  input  [63 : 0] wti_s_0_MData;

  // value method wti_s_0_sThreadBusy
  output wti_s_0_SThreadBusy;

  // value method wti_s_0_sReset_n
  output wti_s_0_SReset_n;

  // action method wti_s_1_mCmd
  input  [2 : 0] wti_s_1_MCmd;

  // action method wti_s_1_mData
  input  [63 : 0] wti_s_1_MData;

  // value method wti_s_1_sThreadBusy
  output wti_s_1_SThreadBusy;

  // value method wti_s_1_sReset_n
  output wti_s_1_SReset_n;

  // action method wti_s_2_mCmd
  input  [2 : 0] wti_s_2_MCmd;

  // action method wti_s_2_mData
  input  [63 : 0] wti_s_2_MData;

  // value method wti_s_2_sThreadBusy
  output wti_s_2_SThreadBusy;

  // value method wti_s_2_sReset_n
  output wti_s_2_SReset_n;

  // value method wmiM0_mCmd
  output [2 : 0] wmiM0_MCmd;

  // value method wmiM0_mReqLast
  output wmiM0_MReqLast;

  // value method wmiM0_mReqInfo
  output wmiM0_MReqInfo;

  // value method wmiM0_mAddrSpace
  output wmiM0_MAddrSpace;

  // value method wmiM0_mAddr
  output [13 : 0] wmiM0_MAddr;

  // value method wmiM0_mBurstLength
  output [11 : 0] wmiM0_MBurstLength;

  // value method wmiM0_mDataValid
  output wmiM0_MDataValid;

  // value method wmiM0_mDataLast
  output wmiM0_MDataLast;

  // value method wmiM0_mData
  output [31 : 0] wmiM0_MData;

  // value method wmiM0_mDataInfo

  // value method wmiM0_mDataByteEn
  output [3 : 0] wmiM0_MDataByteEn;

  // action method wmiM0_sResp
  input  [1 : 0] wmiM0_SResp;

  // action method wmiM0_sData
  input  [31 : 0] wmiM0_SData;

  // action method wmiM0_sThreadBusy
  input  wmiM0_SThreadBusy;

  // action method wmiM0_sDataThreadBusy
  input  wmiM0_SDataThreadBusy;

  // action method wmiM0_sRespLast
  input  wmiM0_SRespLast;

  // action method wmiM0_sFlag
  input  [31 : 0] wmiM0_SFlag;

  // value method wmiM0_mFlag
  output [31 : 0] wmiM0_MFlag;

  // value method wmiM0_mReset_n
  output wmiM0_MReset_n;

  // action method wmiM0_sReset_n
  input  wmiM0_SReset_n;

  // value method wmiM1_mCmd
  output [2 : 0] wmiM1_MCmd;

  // value method wmiM1_mReqLast
  output wmiM1_MReqLast;

  // value method wmiM1_mReqInfo
  output wmiM1_MReqInfo;

  // value method wmiM1_mAddrSpace
  output wmiM1_MAddrSpace;

  // value method wmiM1_mAddr
  output [13 : 0] wmiM1_MAddr;

  // value method wmiM1_mBurstLength
  output [11 : 0] wmiM1_MBurstLength;

  // value method wmiM1_mDataValid
  output wmiM1_MDataValid;

  // value method wmiM1_mDataLast
  output wmiM1_MDataLast;

  // value method wmiM1_mData
  output [31 : 0] wmiM1_MData;

  // value method wmiM1_mDataInfo

  // value method wmiM1_mDataByteEn
  output [3 : 0] wmiM1_MDataByteEn;

  // action method wmiM1_sResp
  input  [1 : 0] wmiM1_SResp;

  // action method wmiM1_sData
  input  [31 : 0] wmiM1_SData;

  // action method wmiM1_sThreadBusy
  input  wmiM1_SThreadBusy;

  // action method wmiM1_sDataThreadBusy
  input  wmiM1_SDataThreadBusy;

  // action method wmiM1_sRespLast
  input  wmiM1_SRespLast;

  // action method wmiM1_sFlag
  input  [31 : 0] wmiM1_SFlag;

  // value method wmiM1_mFlag
  output [31 : 0] wmiM1_MFlag;

  // value method wmiM1_mReset_n
  output wmiM1_MReset_n;

  // action method wmiM1_sReset_n
  input  wmiM1_SReset_n;

  // value method wmemiM0_mCmd
  output [2 : 0] wmemiM0_MCmd;

  // value method wmemiM0_mReqLast
  output wmemiM0_MReqLast;

  // value method wmemiM0_mAddr
  output [35 : 0] wmemiM0_MAddr;

  // value method wmemiM0_mBurstLength
  output [11 : 0] wmemiM0_MBurstLength;

  // value method wmemiM0_mDataValid
  output wmemiM0_MDataValid;

  // value method wmemiM0_mDataLast
  output wmemiM0_MDataLast;

  // value method wmemiM0_mData
  output [127 : 0] wmemiM0_MData;

  // value method wmemiM0_mDataByteEn
  output [15 : 0] wmemiM0_MDataByteEn;

  // action method wmemiM0_sResp
  input  [1 : 0] wmemiM0_SResp;

  // action method wmemiM0_sRespLast
  input  wmemiM0_SRespLast;

  // action method wmemiM0_sData
  input  [127 : 0] wmemiM0_SData;

  // action method wmemiM0_sCmdAccept
  input  wmemiM0_SCmdAccept;

  // action method wmemiM0_sDataAccept
  input  wmemiM0_SDataAccept;

  // value method wmemiM0_mReset_n
  output wmemiM0_MReset_n;

  // action method wsi_s_adc_mCmd
  input  [2 : 0] wsi_s_adc_MCmd;

  // action method wsi_s_adc_mReqLast
  input  wsi_s_adc_MReqLast;

  // action method wsi_s_adc_mBurstPrecise
  input  wsi_s_adc_MBurstPrecise;

  // action method wsi_s_adc_mBurstLength
  input  [11 : 0] wsi_s_adc_MBurstLength;

  // action method wsi_s_adc_mData
  input  [31 : 0] wsi_s_adc_MData;

  // action method wsi_s_adc_mByteEn
  input  [3 : 0] wsi_s_adc_MByteEn;

  // action method wsi_s_adc_mReqInfo
  input  [7 : 0] wsi_s_adc_MReqInfo;

  // action method wsi_s_adc_mDataInfo

  // value method wsi_s_adc_sThreadBusy
  output wsi_s_adc_SThreadBusy;

  // value method wsi_s_adc_sReset_n
  output wsi_s_adc_SReset_n;

  // action method wsi_s_adc_mReset_n
  input  wsi_s_adc_MReset_n;

  // value method wsi_m_dac_mCmd
  output [2 : 0] wsi_m_dac_MCmd;

  // value method wsi_m_dac_mReqLast
  output wsi_m_dac_MReqLast;

  // value method wsi_m_dac_mBurstPrecise
  output wsi_m_dac_MBurstPrecise;

  // value method wsi_m_dac_mBurstLength
  output [11 : 0] wsi_m_dac_MBurstLength;

  // value method wsi_m_dac_mData
  output [31 : 0] wsi_m_dac_MData;

  // value method wsi_m_dac_mByteEn
  output [3 : 0] wsi_m_dac_MByteEn;

  // value method wsi_m_dac_mReqInfo
  output [7 : 0] wsi_m_dac_MReqInfo;

  // value method wsi_m_dac_mDataInfo

  // action method wsi_m_dac_sThreadBusy
  input  wsi_m_dac_SThreadBusy;

  // value method wsi_m_dac_mReset_n
  output wsi_m_dac_MReset_n;

  // action method wsi_m_dac_sReset_n
  input  wsi_m_dac_SReset_n;

  output [511 : 0] uuid;

  input 	   rom_en;
  input  [9:0] 	   rom_addr;
  output [31:0]    rom_data;

  wire [511 : 0]   myUUID;
  wire [511 : 0]   uuid = myUUID;
  
  // signals for module outputs
  wire [127 : 0] wmemiM0_MData;
  wire [35 : 0] wmemiM0_MAddr;
  wire [31 : 0] wci_s_0_SData,
		wci_s_1_SData,
		wci_s_2_SData,
		wci_s_3_SData,
		wci_s_4_SData,
		wci_s_5_SData,
		wci_s_6_SData,
		wci_s_7_SData,
		wmiM0_MData,
		wmiM0_MFlag,
		wmiM1_MData,
		wmiM1_MFlag,
		wsi_m_dac_MData;
  wire [15 : 0] wmemiM0_MDataByteEn;
  wire [13 : 0] wmiM0_MAddr, wmiM1_MAddr;
  wire [11 : 0] wmemiM0_MBurstLength,
		wmiM0_MBurstLength,
		wmiM1_MBurstLength,
		wsi_m_dac_MBurstLength;
  wire [7 : 0] wsi_m_dac_MReqInfo;
  wire [3 : 0] wmiM0_MDataByteEn, wmiM1_MDataByteEn, wsi_m_dac_MByteEn;
  wire [2 : 0] wmemiM0_MCmd, wmiM0_MCmd, wmiM1_MCmd, wsi_m_dac_MCmd;
  wire [1 : 0] wci_s_0_SFlag,
	       wci_s_0_SResp,
	       wci_s_1_SFlag,
	       wci_s_1_SResp,
	       wci_s_2_SFlag,
	       wci_s_2_SResp,
	       wci_s_3_SFlag,
	       wci_s_3_SResp,
	       wci_s_4_SFlag,
	       wci_s_4_SResp,
	       wci_s_5_SFlag,
	       wci_s_5_SResp,
	       wci_s_6_SFlag,
	       wci_s_6_SResp,
	       wci_s_7_SFlag,
	       wci_s_7_SResp;
  wire wci_s_0_SThreadBusy,
       wci_s_1_SThreadBusy,
       wci_s_2_SThreadBusy,
       wci_s_3_SThreadBusy,
       wci_s_4_SThreadBusy,
       wci_s_5_SThreadBusy,
       wci_s_6_SThreadBusy,
       wci_s_7_SThreadBusy,
       wmemiM0_MDataLast,
       wmemiM0_MDataValid,
       wmemiM0_MReqLast,
       wmemiM0_MReset_n,
       wmiM0_MAddrSpace,
       wmiM0_MDataLast,
       wmiM0_MDataValid,
       wmiM0_MReqInfo,
       wmiM0_MReqLast,
       wmiM0_MReset_n,
       wmiM1_MAddrSpace,
       wmiM1_MDataLast,
       wmiM1_MDataValid,
       wmiM1_MReqInfo,
       wmiM1_MReqLast,
       wmiM1_MReset_n,
       wsi_m_dac_MBurstPrecise,
       wsi_m_dac_MReqLast,
       wsi_m_dac_MReset_n,
       wsi_s_adc_SReset_n,
       wsi_s_adc_SThreadBusy;

  mkUUID id(.uuid(myUUID));

  // Instantiate the wip-compliant app
  ocpi_app app(
  .wci_Clk(CLK),
  .wci0_MReset_n(RST_N_rst_0),
  .wci0_MAddr(wci_s_0_MAddr),
  .wci0_MAddrSpace(wci_s_0_MAddrSpace),
  .wci0_MByteEn(wci_s_0_MByteEn),
  .wci0_MCmd(wci_s_0_MCmd),
  .wci0_MData(wci_s_0_MData),
  .wci0_MFlag(wci_s_0_MFlag),
  .wci0_SResp(wci_s_0_SResp),
  .wci0_SData(wci_s_0_SData),
  .wci0_SThreadBusy(wci_s_0_SThreadBusy),
  .wci0_SFlag(wci_s_0_SFlag),

  .wci1_MReset_n(RST_N_rst_1),
  .wci1_MAddr(wci_s_1_MAddr),
  .wci1_MAddrSpace(wci_s_1_MAddrSpace),
  .wci1_MByteEn(wci_s_1_MByteEn),
  .wci1_MCmd(wci_s_1_MCmd),
  .wci1_MData(wci_s_1_MData),
  .wci1_MFlag(wci_s_1_MFlag),
  .wci1_SResp(wci_s_1_SResp),
  .wci1_SData(wci_s_1_SData),
  .wci1_SThreadBusy(wci_s_1_SThreadBusy),
  .wci1_SFlag(wci_s_1_SFlag),

  .wci2_MReset_n(RST_N_rst_2),
  .wci2_MAddr(wci_s_2_MAddr),
  .wci2_MAddrSpace(wci_s_2_MAddrSpace),
  .wci2_MByteEn(wci_s_2_MByteEn),
  .wci2_MCmd(wci_s_2_MCmd),
  .wci2_MData(wci_s_2_MData),
  .wci2_MFlag(wci_s_2_MFlag),
  .wci2_SResp(wci_s_2_SResp),
  .wci2_SData(wci_s_2_SData),
  .wci2_SThreadBusy(wci_s_2_SThreadBusy),
  .wci2_SFlag(wci_s_2_SFlag),

  .wci3_MReset_n(RST_N_rst_3),
  .wci3_MAddr(wci_s_3_MAddr),
  .wci3_MAddrSpace(wci_s_3_MAddrSpace),
  .wci3_MByteEn(wci_s_3_MByteEn),
  .wci3_MCmd(wci_s_3_MCmd),
  .wci3_MData(wci_s_3_MData),
  .wci3_MFlag(wci_s_3_MFlag),
  .wci3_SResp(wci_s_3_SResp),
  .wci3_SData(wci_s_3_SData),
  .wci3_SThreadBusy(wci_s_3_SThreadBusy),
  .wci3_SFlag(wci_s_3_SFlag),

  .wci4_MReset_n(RST_N_rst_4),
  .wci4_MAddr(wci_s_4_MAddr),
  .wci4_MAddrSpace(wci_s_4_MAddrSpace),
  .wci4_MByteEn(wci_s_4_MByteEn),
  .wci4_MCmd(wci_s_4_MCmd),
  .wci4_MData(wci_s_4_MData),
  .wci4_MFlag(wci_s_4_MFlag),
  .wci4_SResp(wci_s_4_SResp),
  .wci4_SData(wci_s_4_SData),
  .wci4_SThreadBusy(wci_s_4_SThreadBusy),
  .wci4_SFlag(wci_s_4_SFlag),

  .wci5_MReset_n(RST_N_rst_5),
  .wci5_MAddr(wci_s_5_MAddr),
  .wci5_MAddrSpace(wci_s_5_MAddrSpace),
  .wci5_MByteEn(wci_s_5_MByteEn),
  .wci5_MCmd(wci_s_5_MCmd),
  .wci5_MData(wci_s_5_MData),
  .wci5_MFlag(wci_s_5_MFlag),
  .wci5_SResp(wci_s_5_SResp),
  .wci5_SData(wci_s_5_SData),
  .wci5_SThreadBusy(wci_s_5_SThreadBusy),
  .wci5_SFlag(wci_s_5_SFlag),

  .wci6_MReset_n(RST_N_rst_6),
  .wci6_MAddr(wci_s_6_MAddr),
  .wci6_MAddrSpace(wci_s_6_MAddrSpace),
  .wci6_MByteEn(wci_s_6_MByteEn),
  .wci6_MCmd(wci_s_6_MCmd),
  .wci6_MData(wci_s_6_MData),
  .wci6_MFlag(wci_s_6_MFlag),
  .wci6_SResp(wci_s_6_SResp),
  .wci6_SData(wci_s_6_SData),
  .wci6_SThreadBusy(wci_s_6_SThreadBusy),
  .wci6_SFlag(wci_s_6_SFlag),

  .wci7_MReset_n(RST_N_rst_7),
  .wci7_MAddr(wci_s_7_MAddr),
  .wci7_MAddrSpace(wci_s_7_MAddrSpace),
  .wci7_MByteEn(wci_s_7_MByteEn),
  .wci7_MCmd(wci_s_7_MCmd),
  .wci7_MData(wci_s_7_MData),
  .wci7_MFlag(wci_s_7_MFlag),
  .wci7_SResp(wci_s_7_SResp),
  .wci7_SData(wci_s_7_SData),
  .wci7_SThreadBusy(wci_s_7_SThreadBusy),
  .wci7_SFlag(wci_s_7_SFlag),

  .wti0_MCmd(wti_s_0_MCmd),
  .wti0_MData(wti_s_0_MData),
  .wti0_SReset_n(wti_s_0_SReset_n),
  .wti0_SThreadBusy(wti_s_0_SThreadBusy)
);
  


  BRAM1Load #(.FILENAME("metadatarom.data"),
	      .PIPELINED(1'd0),
	      .ADDR_WIDTH(32'd10),
	      .DATA_WIDTH(32'd32),
	      .MEMSIZE(11'd1024),
	      .BINARY(1'd0)) rom_memory(.CLK(CLK),
					.ADDR(rom_addr),
					.DI(32'd0),
					.WE(1'd0),
					.EN(rom_en),
					.DO(rom_data));
// Tieoffs for unused signals
// First, the unused WSI slave signals
//assign wci_s_0_SResp = 2'b0;
//assign wci_s_0_SThreadBusy = 1'b0;
//assign wci_s_0_SFlag = 2'b0;
//assign wci_s_0_SData = 32'b0;
//assign wci_s_1_SResp = 2'b0;
//assign wci_s_1_SThreadBusy = 1'b0;
//assign wci_s_1_SFlag = 2'b0;
//assign wci_s_1_SData = 32'b0;
//assign wci_s_6_SResp = 2'b0;
//assign wci_s_6_SThreadBusy = 1'b0;
//assign wci_s_6_SFlag = 2'b0;
//assign wci_s_6_SData = 32'b0;
//assign wci_s_7_SResp = 2'b0;
//assign wci_s_7_SThreadBusy = 1'b0;
//assign wci_s_7_SFlag = 2'b0;
//assign wci_s_7_SData = 32'b0;
// Unused memory master signals
assign wmemiM0_MCmd = 3'b0;
assign wmemiM0_MReqLast = 1'b0;
assign wmemiM0_MAddr = 36'b0;
assign wmemiM0_MBurstLength = 12'b0;
assign wmemiM0_MDataValid = 1'b0;
assign wmemiM0_MDataLast = 1'b0;
assign wmemiM0_MData = 128'b0;
assign wmemiM0_MDataByteEn = 16'b0;
assign wmemiM0_MReset_n = 1'b1;
// Unused adc slave signals
assign wsi_s_adc_SThreadBusy = 1'b1;
assign wsi_s_adc_SReset_n = 1'b0;
// Unusd dac master signals
assign wsi_m_dac_MCmd = 3'b0;
assign wsi_m_dac_MReqLast = 1'b0;
assign wsi_m_dac_MBurstPrecise = 1'b0;
assign wsi_m_dac_MBurstLength = 12'b0;
assign wsi_m_dac_MData = 32'b0;
assign wsi_m_dac_MByteEn = 4'b0;
assign wsi_m_dac_MReqInfo = 8'b0;
assign wsi_m_dac_MReset_n = 1'b0;

assign wmiM0_MAddr = 14'b0;
assign wmiM0_MAddrSpace = 1'b0;
assign wmiM0_MBurstLength = 12'b0;
assign wmiM0_MCmd = 3'b0;
assign wmiM0_MData = 32'b0;
assign wmiM0_MDataByteEn = 4'b0;
assign wmiM0_MDataLast = 1'b0;
assign wmiM0_MDataValid = 1'b0;
assign wmiM0_MFlag = 32'b0;
assign wmiM0_MReqInfo = 1'b0;
assign wmiM0_MReqLast = 1'b0;
assign wmiM0_MReset_n = 1'b0;

assign wmiM1_MAddr = 14'b0;
assign wmiM1_MAddrSpace = 1'b0;
assign wmiM1_MBurstLength = 12'b0;
assign wmiM1_MCmd = 3'b0;
assign wmiM1_MData = 32'b0;
assign wmiM1_MDataByteEn = 4'b0;
assign wmiM1_MDataLast = 1'b0;
assign wmiM1_MDataValid = 1'b0;
assign wmiM1_MFlag = 32'b0;
assign wmiM1_MReqInfo = 1'b0;
assign wmiM1_MReqLast = 1'b0;
assign wmiM1_MReset_n = 1'b0;

endmodule  // mkOCApp

