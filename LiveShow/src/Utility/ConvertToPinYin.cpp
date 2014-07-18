// ConvertToPinYin.cpp: implementation of the CPinyin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConvertToPinYin.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPinyin::CPinyin()
{
    Init();
}

CPinyin::~CPinyin()
{
	for (int i = 0; i < m_PinYin.GetSize() ; i++)
	{
		delete m_PinYin.GetAt(i);
	}

	m_PinYin.RemoveAll();
}

void CPinyin::AddPinYin(CString strPinYin, int iMinValue)
{
	PinYin * pPinYin;
	pPinYin = new PinYin;
	pPinYin->strPinYin = strPinYin;
	pPinYin->nMinValue = iMinValue;
	m_PinYin.Add(pPinYin); 
}

void CPinyin::Init()
{
    AddPinYin(_T("a"), -20319);
    AddPinYin(_T("ai"), -20317);
    AddPinYin(_T("an"), -20304);
    AddPinYin(_T("ang"), -20295);
    AddPinYin(_T("ao"), -20292);
    AddPinYin(_T("ba"), -20283);
    AddPinYin(_T("bai"), -20265);
    AddPinYin(_T("ban"), -20257);
    AddPinYin(_T("bang"), -20242);
    AddPinYin(_T("bao"), -20230);
    AddPinYin(_T("bei"), -20051);
    AddPinYin(_T("ben"), -20036);
    AddPinYin(_T("beng"), -20032);
    AddPinYin(_T("bi"), -20026);
    AddPinYin(_T("bian"), -20002);
    AddPinYin(_T("biao"), -19990);
    AddPinYin(_T("bie"), -19986);
    AddPinYin(_T("bin"), -19982);
    AddPinYin(_T("bing"), -19976);
    AddPinYin(_T("bo"), -19805);
    AddPinYin(_T("bu"), -19784);
    AddPinYin(_T("ca"), -19775);
    AddPinYin(_T("cai"), -19774);
    AddPinYin(_T("can"), -19763);
    AddPinYin(_T("cang"), -19756);
    AddPinYin(_T("cao"), -19751);
    AddPinYin(_T("ce"), -19746);
    AddPinYin(_T("ceng"), -19741);
    AddPinYin(_T("cha"), -19739);
    AddPinYin(_T("chai"), -19728);
    AddPinYin(_T("chan"), -19725);
    AddPinYin(_T("chang"), -19715);
    AddPinYin(_T("chao"), -19540);
    AddPinYin(_T("che"), -19531);
    AddPinYin(_T("chen"), -19525);
    AddPinYin(_T("cheng"), -19515);
    AddPinYin(_T("chi"), -19500);
    AddPinYin(_T("chong"), -19484);
    AddPinYin(_T("chou"), -19479);
    AddPinYin(_T("chu"), -19467);
    AddPinYin(_T("chuai"), -19289);
    AddPinYin(_T("chuan"), -19288);
    AddPinYin(_T("chuang"), -19281);
    AddPinYin(_T("chui"), -19275);
    AddPinYin(_T("chun"), -19270);
    AddPinYin(_T("chuo"), -19263);
    AddPinYin(_T("ci"), -19261);
    AddPinYin(_T("cong"), -19249);
    AddPinYin(_T("cou"), -19243);
    AddPinYin(_T("cu"), -19242);
    AddPinYin(_T("cuan"), -19238);
    AddPinYin(_T("cui"), -19235);
    AddPinYin(_T("cun"), -19227);
    AddPinYin(_T("cuo"), -19224);
    AddPinYin(_T("da"), -19218);
    AddPinYin(_T("dai"), -19212);
    AddPinYin(_T("dan"), -19038);
    AddPinYin(_T("dang"), -19023);
    AddPinYin(_T("dao"), -19018);
    AddPinYin(_T("de"), -19006);
    AddPinYin(_T("deng"), -19003);
    AddPinYin(_T("di"), -18996);
    AddPinYin(_T("dian"), -18977);
    AddPinYin(_T("diao"), -18961);
    AddPinYin(_T("die"), -18952);
    AddPinYin(_T("ding"), -18783);
    AddPinYin(_T("diu"), -18774);
    AddPinYin(_T("dong"), -18773);
    AddPinYin(_T("dou"), -18763);
    AddPinYin(_T("du"), -18756);
    AddPinYin(_T("duan"), -18741);
    AddPinYin(_T("dui"), -18735);
    AddPinYin(_T("dun"), -18731);
    AddPinYin(_T("duo"), -18722);
    AddPinYin(_T("e"), -18710);
    AddPinYin(_T("en"), -18697);
    AddPinYin(_T("er"), -18696);
    AddPinYin(_T("fa"), -18526);
    AddPinYin(_T("fan"), -18518);
    AddPinYin(_T("fang"), -18501);
    AddPinYin(_T("fei"), -18490);
    AddPinYin(_T("fen"), -18478);
    AddPinYin(_T("feng"), -18463);
    AddPinYin(_T("fo"), -18448);
    AddPinYin(_T("fou"), -18447);
    AddPinYin(_T("fu"), -18446);
    AddPinYin(_T("ga"), -18239);
    AddPinYin(_T("gai"), -18237);
    AddPinYin(_T("gan"), -18231);
    AddPinYin(_T("gang"), -18220);
    AddPinYin(_T("gao"), -18211);
    AddPinYin(_T("ge"), -18201);
    AddPinYin(_T("gei"), -18184);
    AddPinYin(_T("gen"), -18183);
    AddPinYin(_T("geng"), -18181);
    AddPinYin(_T("gong"), -18012);
    AddPinYin(_T("gou"), -17997);
    AddPinYin(_T("gu"), -17988);
    AddPinYin(_T("gua"), -17970);
    AddPinYin(_T("guai"), -17964);
    AddPinYin(_T("guan"), -17961);
    AddPinYin(_T("guang"), -17950);
    AddPinYin(_T("gui"), -17947);
    AddPinYin(_T("gun"), -17931);
    AddPinYin(_T("guo"), -17928);
    AddPinYin(_T("ha"), -17922);
    AddPinYin(_T("hai"), -17759);
    AddPinYin(_T("han"), -17752);
    AddPinYin(_T("hang"), -17733);
    AddPinYin(_T("hao"), -17730);
    AddPinYin(_T("he"), -17721);
    AddPinYin(_T("hei"), -17703);
    AddPinYin(_T("hen"), -17701);
    AddPinYin(_T("heng"), -17697);
    AddPinYin(_T("hong"), -17692);
    AddPinYin(_T("hou"), -17683);
    AddPinYin(_T("hu"), -17676);
    AddPinYin(_T("hua"), -17496);
    AddPinYin(_T("huai"), -17487);
    AddPinYin(_T("huan"), -17482);
    AddPinYin(_T("huang"), -17468);
    AddPinYin(_T("hui"), -17454);
    AddPinYin(_T("hun"), -17433);
    AddPinYin(_T("huo"), -17427);
    AddPinYin(_T("ji"), -17417);
    AddPinYin(_T("jia"), -17202);
    AddPinYin(_T("jian"), -17185);
    AddPinYin(_T("jiang"), -16983);
    AddPinYin(_T("jiao"), -16970);
    AddPinYin(_T("jie"), -16942);
    AddPinYin(_T("jin"), -16915);
    AddPinYin(_T("jing"), -16733);
    AddPinYin(_T("jiong"), -16708);
    AddPinYin(_T("jiu"), -16706);
    AddPinYin(_T("ju"), -16689);
    AddPinYin(_T("juan"), -16664);
    AddPinYin(_T("jue"), -16657);
    AddPinYin(_T("jun"), -16647);
    AddPinYin(_T("ka"), -16474);
    AddPinYin(_T("kai"), -16470);
    AddPinYin(_T("kan"), -16465);
    AddPinYin(_T("kang"), -16459);
    AddPinYin(_T("kao"), -16452);
    AddPinYin(_T("ke"), -16448);
    AddPinYin(_T("ken"), -16433);
    AddPinYin(_T("keng"), -16429);
    AddPinYin(_T("kong"), -16427);
    AddPinYin(_T("kou"), -16423);
    AddPinYin(_T("ku"), -16419);
    AddPinYin(_T("kua"), -16412);
    AddPinYin(_T("kuai"), -16407);
    AddPinYin(_T("kuan"), -16403);
    AddPinYin(_T("kuang"), -16401);
    AddPinYin(_T("kui"), -16393);
    AddPinYin(_T("kun"), -16220);
    AddPinYin(_T("kuo"), -16216);
    AddPinYin(_T("la"), -16212);
    AddPinYin(_T("lai"), -16205);
    AddPinYin(_T("lan"), -16202);
    AddPinYin(_T("lang"), -16187);
    AddPinYin(_T("lao"), -16180);
    AddPinYin(_T("le"), -16171);
    AddPinYin(_T("lei"), -16169);
    AddPinYin(_T("leng"), -16158);
    AddPinYin(_T("li"), -16155);
    AddPinYin(_T("lia"), -15959);
    AddPinYin(_T("lian"), -15958);
    AddPinYin(_T("liang"), -15944);
    AddPinYin(_T("liao"), -15933);
    AddPinYin(_T("lie"), -15920);
    AddPinYin(_T("lin"), -15915);
    AddPinYin(_T("ling"), -15903);
    AddPinYin(_T("liu"), -15889);
    AddPinYin(_T("long"), -15878);
    AddPinYin(_T("lou"), -15707);
    AddPinYin(_T("lu"), -15701);
    AddPinYin(_T("lv"), -15681);
    AddPinYin(_T("luan"), -15667);
    AddPinYin(_T("lue"), -15661);
    AddPinYin(_T("lun"), -15659);
    AddPinYin(_T("luo"), -15652);
    AddPinYin(_T("ma"), -15640);
    AddPinYin(_T("mai"), -15631);
    AddPinYin(_T("man"), -15625);
    AddPinYin(_T("mang"), -15454);
    AddPinYin(_T("mao"), -15448);
    AddPinYin(_T("me"), -15436);
    AddPinYin(_T("mei"), -15435);
    AddPinYin(_T("men"), -15419);
    AddPinYin(_T("meng"), -15416);
    AddPinYin(_T("mi"), -15408);
    AddPinYin(_T("mian"), -15394);
    AddPinYin(_T("miao"), -15385);
    AddPinYin(_T("mie"), -15377);
    AddPinYin(_T("min"), -15375);
    AddPinYin(_T("ming"), -15369);
    AddPinYin(_T("miu"), -15363);
    AddPinYin(_T("mo"), -15362);
    AddPinYin(_T("mou"), -15183);
    AddPinYin(_T("mu"), -15180);
    AddPinYin(_T("na"), -15165);
    AddPinYin(_T("nai"), -15158);
    AddPinYin(_T("nan"), -15153);
    AddPinYin(_T("nang"), -15150);
    AddPinYin(_T("nao"), -15149);
    AddPinYin(_T("ne"), -15144);
    AddPinYin(_T("nei"), -15143);
    AddPinYin(_T("nen"), -15141);
    AddPinYin(_T("neng"), -15140);
    AddPinYin(_T("ni"), -15139);
    AddPinYin(_T("nian"), -15128);
    AddPinYin(_T("niang"), -15121);
    AddPinYin(_T("niao"), -15119);
    AddPinYin(_T("nie"), -15117);
    AddPinYin(_T("nin"), -15110);
    AddPinYin(_T("ning"), -15109);
    AddPinYin(_T("niu"), -14941);
    AddPinYin(_T("nong"), -14937);
    AddPinYin(_T("nu"), -14933);
    AddPinYin(_T("nv"), -14930);
    AddPinYin(_T("nuan"), -14929);
    AddPinYin(_T("nue"), -14928);
    AddPinYin(_T("nuo"), -14926);
    AddPinYin(_T("o"), -14922);
    AddPinYin(_T("ou"), -14921);
    AddPinYin(_T("pa"), -14914);
    AddPinYin(_T("pai"), -14908);
    AddPinYin(_T("pan"), -14902);
    AddPinYin(_T("pang"), -14894);
    AddPinYin(_T("pao"), -14889);
    AddPinYin(_T("pei"), -14882);
    AddPinYin(_T("pen"), -14873);
    AddPinYin(_T("peng"), -14871);
    AddPinYin(_T("pi"), -14857);
    AddPinYin(_T("pian"), -14678);
    AddPinYin(_T("piao"), -14674);
    AddPinYin(_T("pie"), -14670);
    AddPinYin(_T("pin"), -14668);
    AddPinYin(_T("ping"), -14663);
    AddPinYin(_T("po"), -14654);
    AddPinYin(_T("pu"), -14645);
    AddPinYin(_T("qi"), -14630);
    AddPinYin(_T("qia"), -14594);
    AddPinYin(_T("qian"), -14429);
    AddPinYin(_T("qiang"), -14407);
    AddPinYin(_T("qiao"), -14399);
    AddPinYin(_T("qie"), -14384);
    AddPinYin(_T("qin"), -14379);
    AddPinYin(_T("qing"), -14368);
    AddPinYin(_T("qiong"), -14355);
    AddPinYin(_T("qiu"), -14353);
    AddPinYin(_T("qu"), -14345);
    AddPinYin(_T("quan"), -14170);
    AddPinYin(_T("que"), -14159);
    AddPinYin(_T("qun"), -14151);
    AddPinYin(_T("ran"), -14149);
    AddPinYin(_T("rang"), -14145);
    AddPinYin(_T("rao"), -14140);
    AddPinYin(_T("re"), -14137);
    AddPinYin(_T("ren"), -14135);
    AddPinYin(_T("reng"), -14125);
    AddPinYin(_T("ri"), -14123);
    AddPinYin(_T("rong"), -14122);
    AddPinYin(_T("rou"), -14112);
    AddPinYin(_T("ru"), -14109);
    AddPinYin(_T("ruan"), -14099);
    AddPinYin(_T("rui"), -14097);
    AddPinYin(_T("run"), -14094);
    AddPinYin(_T("ruo"), -14092);
    AddPinYin(_T("sa"), -14090);
    AddPinYin(_T("sai"), -14087);
    AddPinYin(_T("san"), -14083);
    AddPinYin(_T("sang"), -13917);
    AddPinYin(_T("sao"), -13914);
    AddPinYin(_T("se"), -13910);
    AddPinYin(_T("sen"), -13907);
    AddPinYin(_T("seng"), -13906);
    AddPinYin(_T("sha"), -13905);
    AddPinYin(_T("shai"), -13896);
    AddPinYin(_T("shan"), -13894);
    AddPinYin(_T("shang"), -13878);
    AddPinYin(_T("shao"), -13870);
    AddPinYin(_T("she"), -13859);
    AddPinYin(_T("shen"), -13847);
    AddPinYin(_T("sheng"), -13831);
    AddPinYin(_T("shi"), -13658);
    AddPinYin(_T("shou"), -13611);
    AddPinYin(_T("shu"), -13601);
    AddPinYin(_T("shua"), -13406);
    AddPinYin(_T("shuai"), -13404);
    AddPinYin(_T("shuan"), -13400);
    AddPinYin(_T("shuang"), -13398);
    AddPinYin(_T("shui"), -13395);
    AddPinYin(_T("shun"), -13391);
    AddPinYin(_T("shuo"), -13387);
    AddPinYin(_T("si"), -13383);
    AddPinYin(_T("song"), -13367);
    AddPinYin(_T("sou"), -13359);
    AddPinYin(_T("su"), -13356);
    AddPinYin(_T("suan"), -13343);
    AddPinYin(_T("sui"), -13340);
    AddPinYin(_T("sun"), -13329);
    AddPinYin(_T("suo"), -13326);
    AddPinYin(_T("ta"), -13318);
    AddPinYin(_T("tai"), -13147);
    AddPinYin(_T("tan"), -13138);
    AddPinYin(_T("tang"), -13120);
    AddPinYin(_T("tao"), -13107);
    AddPinYin(_T("te"), -13096);
    AddPinYin(_T("teng"), -13095);
    AddPinYin(_T("ti"), -13091);
    AddPinYin(_T("tian"), -13076);
    AddPinYin(_T("tiao"), -13068);
    AddPinYin(_T("tie"), -13063);
    AddPinYin(_T("ting"), -13060);
    AddPinYin(_T("tong"), -12888);
    AddPinYin(_T("tou"), -12875);
    AddPinYin(_T("tu"), -12871);
    AddPinYin(_T("tuan"), -12860);
    AddPinYin(_T("tui"), -12858);
    AddPinYin(_T("tun"), -12852);
    AddPinYin(_T("tuo"), -12849);
    AddPinYin(_T("wa"), -12838);
    AddPinYin(_T("wai"), -12831);
    AddPinYin(_T("wan"), -12829);
    AddPinYin(_T("wang"), -12812);
    AddPinYin(_T("wei"), -12802);
    AddPinYin(_T("wen"), -12607);
    AddPinYin(_T("weng"), -12597);
    AddPinYin(_T("wo"), -12594);
    AddPinYin(_T("wu"), -12585);
    AddPinYin(_T("xi"), -12556);
    AddPinYin(_T("xia"), -12359);
    AddPinYin(_T("xian"), -12346);
    AddPinYin(_T("xiang"), -12320);
    AddPinYin(_T("xiao"), -12300);
    AddPinYin(_T("xie"), -12120);
    AddPinYin(_T("xin"), -12099);
    AddPinYin(_T("xing"), -12089);
    AddPinYin(_T("xiong"), -12074);
    AddPinYin(_T("xiu"), -12067);
    AddPinYin(_T("xu"), -12058);
    AddPinYin(_T("xuan"), -12039);
    AddPinYin(_T("xue"), -11867);
    AddPinYin(_T("xun"), -11861);
    AddPinYin(_T("ya"), -11847);
    AddPinYin(_T("yan"), -11831);
    AddPinYin(_T("yang"), -11798);
    AddPinYin(_T("yao"), -11781);
    AddPinYin(_T("ye"), -11604);
    AddPinYin(_T("yi"), -11589);
    AddPinYin(_T("yin"), -11536);
    AddPinYin(_T("ying"), -11358);
    AddPinYin(_T("yo"), -11340);
    AddPinYin(_T("yong"), -11339);
    AddPinYin(_T("you"), -11324);
    AddPinYin(_T("yu"), -11303);
    AddPinYin(_T("yuan"), -11097);
    AddPinYin(_T("yue"), -11077);
    AddPinYin(_T("yun"), -11067);
    AddPinYin(_T("za"), -11055);
    AddPinYin(_T("zai"), -11052);
    AddPinYin(_T("zan"), -11045);
    AddPinYin(_T("zang"), -11041);
    AddPinYin(_T("zao"), -11038);
    AddPinYin(_T("ze"), -11024);
    AddPinYin(_T("zei"), -11020);
    AddPinYin(_T("zen"), -11019);
    AddPinYin(_T("zeng"), -11018);
    AddPinYin(_T("zha"), -11014);
    AddPinYin(_T("zhai"), -10838);
    AddPinYin(_T("zhan"), -10832);
    AddPinYin(_T("zhang"), -10815);
    AddPinYin(_T("zhao"), -10800);
    AddPinYin(_T("zhe"), -10790);
    AddPinYin(_T("zhen"), -10780);
    AddPinYin(_T("zheng"), -10764);
    AddPinYin(_T("zhi"), -10587);
    AddPinYin(_T("zhong"), -10544);
    AddPinYin(_T("zhou"), -10533);
    AddPinYin(_T("zhu"), -10519);
    AddPinYin(_T("zhua"), -10331);
    AddPinYin(_T("zhuai"), -10329);
    AddPinYin(_T("zhuan"), -10328);
    AddPinYin(_T("zhuang"), -10322);
    AddPinYin(_T("zhui"), -10315);
    AddPinYin(_T("zhun"), -10309);
    AddPinYin(_T("zhuo"), -10307);
    AddPinYin(_T("zi"), -10296);
    AddPinYin(_T("zong"), -10281);
    AddPinYin(_T("zou"), -10274);
    AddPinYin(_T("zu"), -10270);
    AddPinYin(_T("zuan"), -10262);
    AddPinYin(_T("zui"), -10260);
    AddPinYin(_T("zun"), -10256);
    AddPinYin(_T("zuo"), -10254);
}

CString CPinyin::Change(CString strSource1)
{
	USES_CONVERSION;

	string str = T2A((LPTSTR)(LPCTSTR)strSource1);
	string strResult;
	int nLength = str.length();

	for (int i = 0; i < nLength; i++)
	{
		BYTE byteFirstChar = str[i];
		
		if (IsDBCSLeadByte(byteFirstChar) && i < nLength - 1)//ºº×Ö
		{ 
			BYTE byteSecondChar = str[i + 1];
			int nHightByte =(int)byteFirstChar;
			int nLowByte =(int)byteSecondChar;
			
			int nAscii = (256*nHightByte + nLowByte) - 256*256;
			
			int nSize = m_PinYin.GetSize();
			for (int nIndex = 0; nIndex < nSize - 1; nIndex++)
			{
				PinYin * pPinYin = m_PinYin.GetAt(nIndex);
				if (nAscii < pPinYin->nMinValue)
				{
					if (nIndex > 0)
					{
						pPinYin = m_PinYin.GetAt(nIndex - 1);
						strResult += T2A((LPTSTR)(LPCTSTR)pPinYin->strPinYin);
						i++;
						break;
					}
					else
					{
						strResult += byteFirstChar;
						strResult += byteSecondChar;
						i++;
						break;
					}
				}
			}           
		}
		else
		{
			strResult += byteFirstChar;
		}		  
	}
	
	CString strTemp;
	strTemp = A2T((char*)strResult.c_str());
	return strTemp;
}

