#include "Token.h"
#include "..\crypt\Crypt.h"
#include "..\crypt\KeyManager.h"
#include "WinSock2.h"
#include "..\crypt\md5.h"
#include "51ktv.h"

char TokenProtocol::reqbuf[MAX_CLIENT_MSG_LEN];
TokenProtocol::ErrorCode TokenProtocol::errorCode=TokenProtocol::EVERYTHING_OK;
std::wstring TokenProtocol::errorMsg;
std::wstring TokenProtocol::errorUrl;
std::wstring TokenProtocol::errorMsg2;

void TokenProtocol::GenOption(PkgHead& req_head, char arrbyKey[16],
						   std::string& strVerifyKey, std::wstring& strVerifyCode)
{
	char srcbuf[MAX_CLIENT_MSG_LEN];

	std::string verifycode = common::utility::stringhelper::UnicodeToUTF8(strVerifyCode);

	for(int i = 0; i < verifycode.size(); ++i)
	{
		verifycode[i] = toupper(verifycode[i]);
	}

	if((1+verifycode.size())+(2+strVerifyKey.size()) > MAX_CLIENT_MSG_LEN)
	{
		assert(0);
		return;
	}

	char* p = srcbuf;
	*p = verifycode.size();
	p++;
	strcpy_s(p, MAX_CLIENT_MSG_LEN-1, verifycode.c_str());//修改了原先彩虹代码中的一个错误:第二个参数不能为MAX_CLIENT_MSG_LEN
	p += strlen(verifycode.c_str());
	*(short*)p = htons(short(strVerifyKey.size()));
	p += sizeof(short);
	byte* pBuf = (byte*)strVerifyKey.c_str();
	memcpy(p, pBuf, strVerifyKey.size());
	p += strVerifyKey.size();

	char dstbuf[MAX_HEAD_OPTION_LEN];

	int bodybuflen = Auth51::CXTEA::Encrypt(srcbuf, p-srcbuf, dstbuf, MAX_HEAD_OPTION_LEN, arrbyKey);
	req_head.optionlen = bodybuflen;

	memcpy(&req_head.option, &dstbuf, bodybuflen <= MAX_HEAD_OPTION_LEN ? bodybuflen:MAX_HEAD_OPTION_LEN);
}

template<typename C,typename T>
static void SafeCopy(C & c,T *const des,std::size_t sizeofDes,bool appendZero=true)
{
	//assert((c.size()+(appendZero?1:0))<=sizeofDes);
	std::size_t minSize=min(c.size(),sizeofDes-(appendZero?1:0));
	int i;
	for(i=0;i<minSize;i++)
		des[i]=c[i];
	if(appendZero && (i<sizeofDes))
		des[i]=T(0);
}

#define SIZEOFARRY(a) (sizeof(a)/sizeof(a[0]))

ByteBuf TokenProtocol::GenTokenReq(std::string const& username,std::string const& pass,bool beCylcle,bool b179,
								 bool bVerify, std::string strVerifyKey, std::wstring strVerifyCode)
{
	//获得密码相关设定
	char localkey[16];
	char keybase[16];
	short keybaselen = 0;
	KeyManager keymgr;
	//目前没有版本区别
	keymgr.GetClientKey(localkey);
	keymgr.GetKeyBase(keybase, 0, keybaselen);

	//填写请求
	//报文头部
	PkgHead req_head;

	req_head.tag = 0x00;
	req_head.msgid = 0x10;
	req_head.version = 0;
	req_head.optionlen = 0;
	memset(req_head.flag, 0x04, 2);

	if(bVerify)
	{
		GenOption(req_head, localkey, strVerifyKey, strVerifyCode);
	}

	req_head.headlen = 11 + req_head.optionlen;

	//报文内容
	AuthReq req_body;

	//id长度包括空字符
	//  [10/29/2008 ken] bug 1619
	if((username.size()+1)<=MAX_UID_LEN)
		req_body.uidlen = username.size()+1;
	else
	{
		assert(0);
		req_body.uidlen=MAX_UID_LEN;
	}
	SafeCopy(username,req_body.uid,SIZEOFARRY(req_body.uid));
	
	req_body.isfirst = ((beCylcle==false)?0x31:0x30);
	req_body.iplen = 0;

	char *source = "VDC_CLIENT";
	strncpy_s(req_body.source, source, MAX_SOURCE_LEN);
	req_body.source[MAX_SOURCE_LEN-1] = '\0';
	req_body.sourcelen = strlen(req_body.source);

	req_body.appid = b179?0x7:0x6;

	//密码的md5
	char md5_32[33] = {0};
	SafeCopy(pass,md5_32,SIZEOFARRY(md5_32),false);

	//strncpy_s(username.c_str(), username.size(), req_body.uid, strlen(req_body.uid));

	char md5_16[16];
	md5_convert(md5_32, md5_16);

	//利用密码md5对keybase加密作为password
	req_body.passwordlen = Auth51::CXTEA::Encrypt(keybase, keybaselen, req_body.password, MAX_PSWD_LEN, md5_16);

	//原始body信息
	char body[MAX_CLIENT_MSG_LEN];
	char *cur = body;
	memcpy(cur, &req_body.uidlen, 1);
	cur += 1;

	memcpy(cur, req_body.uid, req_body.uidlen);
	cur += req_body.uidlen;

	memcpy(cur, &req_body.isfirst, 1);
	cur += 1;

	memcpy(cur, &req_body.iplen, 1);
	cur += 1;

	memcpy(cur, req_body.ip, req_body.iplen);
	cur += req_body.iplen;

	memcpy(cur, &req_body.sourcelen, 1);
	cur += 1;

	memcpy(cur, req_body.source, req_body.sourcelen);
	cur += req_body.sourcelen;

	req_body.appid = htons(req_body.appid);
	memcpy(cur, &req_body.appid, sizeof(req_body.appid));
	cur += 2;

	memcpy(cur, req_body.password, req_body.passwordlen);
	cur += req_body.passwordlen;

	int bodylen = cur - body;

	char bodybuf[MAX_CLIENT_MSG_LEN];
	int bodybuflen = Auth51::CXTEA::Encrypt(body, bodylen, bodybuf, MAX_CLIENT_MSG_LEN, localkey);

	req_head.totallen = req_head.headlen + bodybuflen;


	cur = reqbuf;
	*cur = req_head.tag;
	cur += 1;
	short itmp = htons(req_head.totallen);
	memcpy(cur, &itmp, sizeof(itmp));
	cur += sizeof(itmp);
	itmp = htons(req_head.headlen);
	memcpy(cur, &itmp, sizeof(itmp));
	cur += sizeof(itmp); 
	itmp = htons(req_head.msgid);
	memcpy(cur, &itmp, sizeof(itmp));
	cur += sizeof(itmp); 
	itmp = htons(req_head.version);
	memcpy(cur, &itmp, sizeof(itmp));
	cur += sizeof(itmp); 
	memcpy(cur, req_head.flag, 2);
	cur += 2;
	memcpy(cur, req_head.option, req_head.optionlen);
	cur += req_head.optionlen; 
	assert(cur - reqbuf == req_head.headlen);
	memcpy(cur, bodybuf, bodybuflen);
	cur += bodybuflen;
	assert(cur - reqbuf == req_head.totallen);

	return ByteBuf((std::size_t)req_head.totallen,(byte*)reqbuf);
}

TokenProtocol::TokenRspError TokenProtocol::Decode51TokenRsp(uint32& unOutUIN, std::string const &pass,
													 ByteBuf & buf,ByteBuf &token,ByteBuf &sessionKey,
													 uint32 &un51UIN, ByteBuf &account, ByteBuf& inneraccount)
{
	if(buf.first<3)//包长度数据没到达，直接返回
		return TOKEN_RSP_NO_HEAD;

	char md5_32[33] = {0};
	SafeCopy(pass,md5_32,SIZEOFARRY(md5_32),false);

	char md5_16[16];
	md5_convert(md5_32, md5_16);

	//获得密码相关设定

	char localkey[16];
	char keybase[16];
	short keybaselen = 0;
	KeyManager keymgr;
	//目前没有版本区别
	keymgr.GetClientKey(localkey);
	keymgr.GetKeyBase(keybase, 0, keybaselen);

	uint32 rcvlen=buf.first;
	char * revdata= (char *)buf.second;

	PkgHead res_head;
	char *rescur = revdata;

	//下面对包头内容解析
	memcpy(&res_head.tag, rescur, 1);
	rescur += 1;
	res_head.totallen = /*htons(*(short*)rescur);*/*(short*)rescur;

	//接收的数据未达到包长
	if(res_head.totallen > rcvlen)
		return TOKEN_RSP_NO_ENOUGH_LENTH;

	rescur += 2;
	res_head.headlen = ntohs(*(short*)rescur);
	rescur += 2;
	res_head.msgid = ntohs(*(short*)rescur);
	rescur += 2;

	//just like this
	assert(res_head.totallen == rcvlen);

	//下面对包体内容解密和解析
	char *resbodybuf = revdata + res_head.headlen;
	uint32 resbodylen = res_head.totallen - res_head.headlen;

	//解密会引起数据变化，这里先备份数据
	char tmp[MAX_SERVER_MSG_LEN];
	memcpy(tmp, resbodybuf, resbodylen);
	int tmplen = resbodylen;

	//存放解密后内容
	char realbody[MAX_SERVER_MSG_LEN];
	int realbodylen = 0 ;

	//int suc = 0;
	TokenRspError error=TOKEN_RSP_UNKNOWN_ERROR;

	realbodylen = Auth51::CXTEA::Decrypt(tmp, tmplen, realbody, MAX_SERVER_MSG_LEN, md5_16);
	if(realbodylen > 0)
	{
		error = TOKEN_RSP_SUCCESS;
		////printf("Dec suc, login suc, realbodylen(%d)\n", realbodylen);
	}
	else
	{
		realbodylen = Auth51::CXTEA::Decrypt(resbodybuf, resbodylen, realbody, MAX_SERVER_MSG_LEN, localkey);
		if(realbodylen > 0)
		{
			error = TOKEN_RSP_ERROR;
			////printf("Dec suc, login fail, realbodylen(%d)\n", realbodylen);
		}
		else
		{
			error = TOKEN_RSP_ERROR;
		}
	}

	//密文用md5key解开，登录成功
	if(error == TOKEN_RSP_SUCCESS)
	{
		rescur = realbody;
		short result = htons(*(short*)rescur);
		rescur += 2;
		assert(result == 0);
		un51UIN = htonl(*(int*)rescur);
		rescur += 4;
		char uidlen = *rescur;
		rescur += 1;

		strncpy_s((char*)inneraccount.second, inneraccount.first, rescur, MAX_UID_LEN);
		inneraccount.second[MAX_UID_LEN-1] = '\0';
		rescur += uidlen;

		/////////////////////new ///////////////////////////////////////////////////////

		char photeLen= *rescur;
		rescur += 1;

		strncpy_s((char*)account.second, account.first, rescur, MAX_UID_LEN);
		account.second[MAX_UID_LEN-1] = '\0';

		rescur+=photeLen;
		unOutUIN=htonl(*(int*)rescur);
		rescur += 4;			

		///////////////////////////////////////////////////////////////////////////
		memcpy(sessionKey.second, rescur, 16);
		rescur += 16;
		token.first = *rescur;
		rescur += 1;
		
		memcpy_s(token.second,TOKEN_MAX_LEN, rescur, token.first);
	}
	//密文用localkey解开，登录不成功
	else if(error == TOKEN_RSP_ERROR)
	{

		rescur = realbody;
		uint16 result = htons(*(short*)rescur);

		//出错了，直接返回，只解析错误原因
		errorCode=(ErrorCode)result;

		if(errorCode == JUREN_ACCOUNT_NOEXIST)
		{
			errorCode = ACCOUNT_NOEXIST;
		}
		else if(errorCode == JUREN_PASSWORD_WRONG)
		{
			errorCode = PASSWORD_WRONG;
		}

		if(errorCode !=	COMMON_ERROR && errorCode != ACCOUNT_NOEXIST
			&& errorCode !=	PASSWORD_WRONG && errorCode != ACCOUNT_FORBIDEN
			&& errorCode !=	NEED_VERIFY && errorCode !=	VERIFY_FORMAT_ERROR				
			&& errorCode !=	VERIFY_ERROR && errorCode != VERIFY_TIMEOUT
			&& errorCode !=	LOGIN_REFUSED
			&& errorCode !=	LOGIN_INVALID_EMAIL_PASS_SUCC && errorCode != LOGIN_INVALID_EMAIL_PASS_FAIL)
		{
			errorCode = COMMON_ERROR;
		}

		rescur += 2;

		char regfromlen = *rescur;
		rescur += 1;
		char regfrom[MAX_SOURCE_LEN];
		regfrom[0]=0;

		if(regfromlen>MAX_SOURCE_LEN)
			regfromlen=MAX_SOURCE_LEN-1;

		memcpy(regfrom, rescur, regfromlen);
		regfrom[regfromlen] = '\0';
		rescur += regfromlen;


		uint16 errlen = htons(*(short*)rescur);
		rescur += 2;

		char err[MAX_ERRMSG_LEN];
		err[0]=0;

		if(errlen>MAX_ERRMSG_LEN)
			errlen=MAX_ERRMSG_LEN-1;

		memcpy(err, rescur, errlen);
		rescur += errlen;
		err[errlen] = '\0';

		char url[TokenProtocol::MAX_URL_LEN];
		url[0]=0;

		uint16 urllen = htons(*(short*)rescur);

		if(urllen>TokenProtocol::MAX_URL_LEN)
			urllen=TokenProtocol::MAX_URL_LEN-1;

		rescur += 2;
		memcpy(url, rescur, urllen);
		url[urllen] = '\0';

		rescur += urllen;

		char msg[1024];
		msg[0]=0;

		if(ACCOUNT_FORBIDEN == errorCode || LOGIN_REFUSED == errorCode)
		{
			uint16 msglen = htons(*(short*)rescur);

			if (msglen != 0)
			{
				//printf("totallen: %d, msglen: %d, currentlen: %d, curcur: %s\n", realbodylen, msglen, realbody + realbodylen - rescur, rescur);

				int len = sizeof(msglen);
				rescur += len;

				if(msglen>1024)
					msglen=1024-1;

				memcpy(msg, rescur, msglen);
				msg[msglen] = '\0';

				//printf("Err: %d, %s, %s, Url: %s, errmsg: %s\n", result, regfrom, err, url, msg);
			}
		}

		std::string tempMsg=err;
		std::string tempUrl=url;
		std::string tempMsg2;

		if(ACCOUNT_FORBIDEN == errorCode || LOGIN_REFUSED == errorCode)
			tempMsg2 = msg;

		errorMsg = common::utility::stringhelper::UTF8ToUnicode(tempMsg);
		errorUrl = common::utility::stringhelper::UTF8ToUnicode(tempUrl);
		errorMsg2 = common::utility::stringhelper::UTF8ToUnicode(tempMsg2);

		return TOKEN_RSP_ERROR;
	}

	return error;
}

TokenProtocol::TokenRspError TokenProtocol::Decode179TokenRsp(uint32& unOutUIN, std::string const &pass,
															 ByteBuf & buf,ByteBuf &token,ByteBuf &sessionKey,
															 uint32 &un179UIN, ByteBuf &email)
{
	if(buf.first<3)//包长度数据没到达，直接返回
		return TOKEN_RSP_NO_HEAD;

	char md5_32[33] = {0};
	SafeCopy(pass,md5_32,SIZEOFARRY(md5_32),false);

	char md5_16[16];
	md5_convert(md5_32, md5_16);

	//获得密码相关设定

	char localkey[16];
	char keybase[16];
	short keybaselen = 0;
	KeyManager keymgr;
	//目前没有版本区别
	keymgr.GetClientKey(localkey);
	keymgr.GetKeyBase(keybase, 0, keybaselen);

	uint32 rcvlen=buf.first;
	char * revdata= (char *)buf.second;

	PkgHead res_head;
	char *rescur = revdata;

	//下面对包头内容解析
	memcpy(&res_head.tag, rescur, 1);
	rescur += 1;
	res_head.totallen = /*htons(*(short*)rescur);*/*(short*)rescur;

	//接收的数据未达到包长
	if(res_head.totallen > rcvlen)
		return TOKEN_RSP_NO_ENOUGH_LENTH;

	rescur += 2;
	res_head.headlen = ntohs(*(short*)rescur);
	rescur += 2;
	res_head.msgid = ntohs(*(short*)rescur);
	rescur += 2;

	//just like this
	assert(res_head.totallen == rcvlen);

	//下面对包体内容解密和解析
	char *resbodybuf = revdata + res_head.headlen;
	uint32 resbodylen = res_head.totallen - res_head.headlen;

	//解密会引起数据变化，这里先备份数据
	char tmp[MAX_SERVER_MSG_LEN];
	memcpy(tmp, resbodybuf, resbodylen);
	int tmplen = resbodylen;

	//存放解密后内容
	char realbody[MAX_SERVER_MSG_LEN];
	int realbodylen = 0 ;

	//int suc = 0;
	TokenRspError error=TOKEN_RSP_UNKNOWN_ERROR;

	realbodylen = Auth51::CXTEA::Decrypt(tmp, tmplen, realbody, MAX_SERVER_MSG_LEN, md5_16);
	if(realbodylen > 0)
	{
		error = TOKEN_RSP_SUCCESS;
		////printf("Dec suc, login suc, realbodylen(%d)\n", realbodylen);
	}
	else
	{
		realbodylen = Auth51::CXTEA::Decrypt(resbodybuf, resbodylen, realbody, MAX_SERVER_MSG_LEN, localkey);
		if(realbodylen > 0)
		{
			error = TOKEN_RSP_ERROR;
			////printf("Dec suc, login fail, realbodylen(%d)\n", realbodylen);
		}
		else
		{
			error = TOKEN_RSP_ERROR;
		}
	}

	//密文用md5key解开，登录成功
	if(error == TOKEN_RSP_SUCCESS)
	{
		rescur = realbody;
		short result = htons(*(short*)rescur);
		rescur += 2;
		assert(result == 0);
		un179UIN = htonl(*(int*)rescur);
		rescur += 4;

		unOutUIN = htonl(*(int*)rescur);
		rescur += 4;

		char uidlen = *rescur;
		rescur += 1;

		strncpy_s((char*)email.second, email.first, rescur, MAX_UID_LEN);
		email.second[MAX_UID_LEN-1] = '\0';
		rescur += uidlen;	

		///////////////////////////////////////////////////////////////////////////
		memcpy(sessionKey.second, rescur, 16);
		rescur += 16;
		token.first = *rescur;
		rescur += 1;

		memcpy_s(token.second,TOKEN_MAX_LEN, rescur, token.first);
	}
	//密文用localkey解开，登录不成功
	else if(error == TOKEN_RSP_ERROR)
	{
		rescur = realbody;
		uint16 result = htons(*(short*)rescur);

		//出错了，直接返回，只解析错误原因
		errorCode=(ErrorCode)result;

		if(errorCode == JUREN_ACCOUNT_NOEXIST)
		{
			errorCode = ACCOUNT_NOEXIST;
		}
		else if(errorCode == JUREN_PASSWORD_WRONG)
		{
			errorCode = PASSWORD_WRONG;
		}

		if(errorCode !=	COMMON_ERROR && errorCode != ACCOUNT_NOEXIST
			&& errorCode !=	PASSWORD_WRONG && errorCode != ACCOUNT_FORBIDEN
			&& errorCode !=	NEED_VERIFY && errorCode !=	VERIFY_FORMAT_ERROR				
			&& errorCode !=	VERIFY_ERROR && errorCode != VERIFY_TIMEOUT
			&& errorCode !=	LOGIN_REFUSED
			&& errorCode !=	LOGIN_INVALID_EMAIL_PASS_SUCC && errorCode != LOGIN_INVALID_EMAIL_PASS_FAIL)
		{
			errorCode = COMMON_ERROR;
		}

		rescur += 2;

		char regfromlen = *rescur;
		rescur += 1;
		char regfrom[MAX_SOURCE_LEN];
		regfrom[0]=0;

		if(regfromlen>MAX_SOURCE_LEN)
			regfromlen=MAX_SOURCE_LEN-1;

		memcpy(regfrom, rescur, regfromlen);
		regfrom[regfromlen] = '\0';
		rescur += regfromlen;


		uint16 errlen = htons(*(short*)rescur);
		rescur += 2;

		char err[MAX_ERRMSG_LEN];
		err[0]=0;

		if(errlen>MAX_ERRMSG_LEN)
			errlen=MAX_ERRMSG_LEN-1;

		memcpy(err, rescur, errlen);
		rescur += errlen;
		err[errlen] = '\0';

		char url[TokenProtocol::MAX_URL_LEN];
		url[0]=0;

		uint16 urllen = htons(*(short*)rescur);

		if(urllen>TokenProtocol::MAX_URL_LEN)
			urllen=TokenProtocol::MAX_URL_LEN-1;

		rescur += 2;
		memcpy(url, rescur, urllen);
		url[urllen] = '\0';

		rescur += urllen;

		char msg[1024];
		msg[0]=0;

		if(ACCOUNT_FORBIDEN == errorCode || LOGIN_REFUSED == errorCode)
		{
			uint16 msglen = htons(*(short*)rescur);

			if (msglen != 0)
			{
				//printf("totallen: %d, msglen: %d, currentlen: %d, curcur: %s\n", realbodylen, msglen, realbody + realbodylen - rescur, rescur);

				int len = sizeof(msglen);
				rescur += len;

				if(msglen>1024)
					msglen=1024-1;

				memcpy(msg, rescur, msglen);
				msg[msglen] = '\0';

				//printf("Err: %d, %s, %s, Url: %s, errmsg: %s\n", result, regfrom, err, url, msg);
			}
		}

		std::string tempMsg=err;
		std::string tempUrl=url;
		std::string tempMsg2;

		if(ACCOUNT_FORBIDEN == errorCode || LOGIN_REFUSED == errorCode)
			tempMsg2 = msg;

		errorMsg = common::utility::stringhelper::UTF8ToUnicode(tempMsg);
		errorUrl = common::utility::stringhelper::UTF8ToUnicode(tempUrl);
		errorMsg2 = common::utility::stringhelper::UTF8ToUnicode(tempMsg2);

		return TOKEN_RSP_ERROR;
	}

	return error;
}