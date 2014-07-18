#pragma once

#include "../Common.h"
#include <string>
#include <xstring>
#include <map>
#include <vector>
//#include <windows.h>

#define MINI_MEM_SIZE	8*1024*1024
#define MEDIUM_MEM_SIZE 12*1024*1024

namespace common
{
namespace utility
{
namespace systemhelper
{
   //请优先使用以下宏
	#define SwitchResourceToModule(A) common::utility::systemhelper::SwitchResource switchResource(A);

	class COMMON_INTERFACE SwitchResource
	{ 
	 public: 
		 /*
		* 构造函数，在构造函数中切换资源句柄 
		* @param [in] dll的名字，不需要.dll的后缀
		* @return 无
		*/
	  SwitchResource(std::string const & dllName);
	  SwitchResource(char const * const dllName);
	  SwitchResource(std::wstring const & dllName);
	  SwitchResource(wchar_t const * const dllName);
	  ~SwitchResource();
	 private:
	  void Init(std::string const & dllName);
	  void Init(std::wstring const & dllName);
	  HINSTANCE oldHINSTANCE;
	};



	/*
	* 获取系统版本参数
	* @param [in] 一个OSVERSIONINFO结构的引用
	* @return TRUE表示成功，FALSE表示失败
	*/
	COMMON_INTERFACE BOOL GetOsVersion(OSVERSIONINFO &);


	enum OS_VERSION//请不要调整顺序
	{
		OS_UNKNOWN		=	0,
		//OS_WIN95		=	1,
		//OS_WIN98		=	2,
		//OS_WIN98SE	=	3,
		//OS_WINME		=	4,
		//OS_WINNT		=   5,
		OS_WIN2000		=	6,
		OS_WINXP		=	7,
		OS_WIN2003		=	8,
		//OS_WINXP64	=	9,
		OS_VISTA		=   10,
		OS_WIN2008		=	11,
		OS_WIN2008R2	=	12,
		OS_WIN7			=	13,
		OS_WIN8			=	14,
	};

	/*
	* 判断当前系统是否等于某个版本
	* @param 无
	* @return OS_VERSION枚举中的一个值
	*/
	COMMON_INTERFACE OS_VERSION GetOsVersion();

	/*
	* 判断当前系统是否等于某个版本
	* @param [in] 一个OS_VERSION枚举值
	* @return TRUE表示相等，FALSE表示不等
	*/
	COMMON_INTERFACE BOOL IsWindowsVersionEqual(OS_VERSION const osv);

	/*
	* 判断当前系统是否等于某个版本或大于某个版本
	* @param [in] 一个OS_VERSION枚举值
	* @return TRUE表示相等或大于，FALSE表示不等或不大于
	*/
	COMMON_INTERFACE BOOL IsWindowsVersionEqualOrLater(OS_VERSION const osv);

	/*
	* 判断某个exe有几个运行实例
	* @param [in] exe的名字，如caihong.exe
	* @return 运行实例的数目
	*/
	COMMON_INTERFACE uint32 GetProcessCount(std::wstring const & name);

	/*
	* 终止某个exe的所有运行实例
	* @param [in] exe的名字，如caihong.exe
	* @return 是否成功终止，全部成功终止返回TRUE，有一个失败返回FALSE
	*/
	COMMON_INTERFACE BOOL KillProcess(std::wstring const & name);

	/*
	* 从资源载入字符串
	* @param [in] 资源id名字。本函数自动将资源句柄切换到GGres.dll
	* @return 对应id的字符串。如果没有对应，返回空串
	*/
	COMMON_INTERFACE std::wstring LoadResourceString(uint32 id);

	/*
	* 获取commonlibrary.dll所在目录的路径
	* @param [in] 无
	* @return 所在路径
	*/
	COMMON_INTERFACE std::wstring GetModulePath();
	
	/*
	* 获取数据目录
	* @param 无
	* @return 如果有相关值，返回，没有返回空串
	*/
	COMMON_INTERFACE std::wstring Get179AppPath();

	/*
	* 获取ie版本
	* @param [in] 无
	* @return ie版本
	*/
	COMMON_INTERFACE std::wstring GetIEVersion();

	/*
	* 打开一个url，使用默认浏览器，如果找不到使用ie。
	* @param [in] url地址
	* @return 如果shellexecute返回的instance不为null，返回TRUE，否则返回FALSE
	*/
	COMMON_INTERFACE BOOL OpenUrl(std::wstring const & wstrURL);

	/*
	* 获取数字月份对应的农历月份字符串
	* @param [in] 数字月份，可取值范围1~12
	* @return 返回对应数字的农历月份字符串，如果没有匹配值，返回空串
	*/
	COMMON_INTERFACE std::wstring GetNongliMonthName( uint32 nMonth);

	/*
	* 获取数字日期对应的农历日期字符串
	* @param [in] 数字月份，可取值范围1~31
	* @return 返回对应数字的农历日期字符串，如果没有匹配值，返回空串
	*/
	COMMON_INTERFACE std::wstring GetNongliDayName( uint32 nDay);

	class COMMON_INTERFACE CHighPerformanceTimeCounter
	{
	public:
		enum ETimeCounterEvent 
		{
			eTCEvent_Null,
			eTCEvent_PickMsgFromTray,
			eTCEvent_PickMsgFromFriendList,
			eTCEvent_PickMsgFromHotKey,
			eTCEvent_Accumulate_processmessage_mf,

			eTCEvent_Max = 5000,
		};

	protected:
		wchar_t* szComment;

		LARGE_INTEGER m_liCounterStart;
		bool m_bAccumulateMode;
		ETimeCounterEvent m_eTCEventAccumulate;

		struct STimePiece
		{
			LARGE_INTEGER liStart;
			LARGE_INTEGER liEnd;

			double dEllapse;		// microsecond, for accumulate counting
			double dEllapseTotal;
			int cnt;

			STimePiece()
			{
				memset(&liStart, 0, sizeof liStart);
				memset(&liEnd, 0, sizeof liEnd);
				dEllapse = 0;
				dEllapseTotal = 0;
				cnt = 0;
			}
		};

		static std::map<ETimeCounterEvent, STimePiece*> m_mapTimeCounter;

		static bool IsInCounting(ETimeCounterEvent eTCEvent);
		static STimePiece* GetTimePiece(ETimeCounterEvent eTCEvent);
		static std::wstring Enum2String(ETimeCounterEvent eTCEvent);

	public:
		CHighPerformanceTimeCounter(wchar_t* szComment = NULL);
		CHighPerformanceTimeCounter(ETimeCounterEvent eTCEvent, wchar_t* szComment = NULL);
		~CHighPerformanceTimeCounter();

		void Start();
		double Stop();

		static void StartCountEvent(ETimeCounterEvent eTCEvent);
		static double StopCountEvent(ETimeCounterEvent eTCEvent, bool bTrace = true);
		
		static void StartAccumulateCountEvent(ETimeCounterEvent eTCEvent, wchar_t* szComment = NULL);
		static double StopAccumulateCountEvent(ETimeCounterEvent eTCEvent, bool bTrace = true);
	};

	/*
	* 调整某个句柄的SecurityLevel
	* @param [in] 要被调整的句柄
	* @return 成功返回TRUE，失败返回FALSE
	*/
	COMMON_INTERFACE BOOL SetSecurityLevel(HANDLE const & hObject);

	/*
	* 接受一个组合键，返回组合键的字符串形式
	* @param [in] 一个组合键
	* @return 对应的字符串
	*/
	COMMON_INTERFACE std::wstring HotkeyToString(DWORD dwHotKey);

	/*
	*  判断一个键是否被按下
	* @param [in] 一个键
	* @return 是返回TRUE，否返回FALSE
	*/
	COMMON_INTERFACE BOOL IsKeyDown(int nVirtKey);

	/*
	*  判断一个字符串是否全是数字
	* @param [in] 一个字串
	* @return 是返回TRUE，否返回FALSE
	*/
	COMMON_INTERFACE BOOL IsDigit( std::wstring wstr);

	/*
	*  判断一个processid 是否是彩虹的id，是否活着
	* @param [in] 一个processid
	* @return 是返回true，否返回false
	*/
	COMMON_INTERFACE bool RainbowProcessAlive(DWORD);

	/*
	*  
	* @param [in] 
	* @
	*/
	COMMON_INTERFACE char GetFirstPinYin(const char * cs);

	//获取所有中文字体
	COMMON_INTERFACE void GetChineseFontNameList(std::vector<std::wstring>&);

	//获取软件默认字体(选择顺序 微软雅黑>宋体)
	COMMON_INTERFACE void GetDefaultFontName(std::wstring&);

	//将steText的内容拷贝到剪切板
	COMMON_INTERFACE void CopyDataToClipbord( std::wstring strText );
	
	//过滤昵称关键词
	COMMON_INTERFACE void FilterNickName(std::wstring& strText);

	//过滤关键词
	COMMON_INTERFACE void FilterWords(std::wstring& strText);

	//过滤公告关键词
	COMMON_INTERFACE void FilterNoticeWords(std::wstring& strText);

	//获取命令行(去掉路径部分)
	COMMON_INTERFACE std::wstring GetCommandLineCommand();

	namespace imply
	{
		struct PinYin
		{
			std::wstring strPinYin;
			int nMinValue;
		};
	}
	class COMMON_INTERFACE CPinyin  
{
public:
	CPinyin();
	~CPinyin();
	std::wstring Change(std::wstring const &strSource);


protected:
	
	//std::vector<imply::PinYin *, imply::PinYin *> m_PinYin;
	void AddPinYin(std::wstring const & strPinYin, int iMinValue);
	void Init();
};




}
}
}