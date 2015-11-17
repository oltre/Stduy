#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PARAMETER_ERROR				1
#define NOT_HTTP_PACKET				2
#define	HEADER_REMAIN_PACKET		3
#define BODY_REMAIN_PACKET			4
#define	BODYSIZE_UNKNOWN_PACKET		5
#define	COMPLETE_PACKET				6
#define	JOIN_PACKET					7
#define REQUEST_HEADER				8
#define RESPONSE_HEADER				9

#define HTTPVERSION09				10
#define HTTPVERSION10				11
#define	HTTPVERSION11				12

class CHTTPElement 
{
private:
	char 				*m_pHeaderName;
	char				*m_pHeaderValue;

public:
	CHTTPElement		*m_pBefore;
	CHTTPElement 		*m_pNext;

	CHTTPElement( );
	CHTTPElement(const char * pHeaderName, const char * pHeaderValue);
	~CHTTPElement( );

	void SetHeaderValue(const char * pHeaderValue);
	char* GetHeaderValue( ) { return this->m_pHeaderValue; }
	char* GetHeaderName( ) { return this->m_pHeaderName; }
};

class CHTTPParser 
{
private:
	char				*m_pHeader;
	unsigned int		m_nHeaderSize;

	char				*m_pBody;
	unsigned int		m_nBodySize;
	unsigned long		m_nContentLength;

	char				*m_pMethod;
	char				*m_pRequestUri;
	char				*m_pStatusCode;
	char				*m_pResonPhrase;
	char				*m_pVersion;
	int					m_nVersion;

	CHTTPElement 		*m_pHead;
	CHTTPElement		*m_pTail;
	
	bool SetBuffer(char *pDst, unsigned int nDestSize, char *pSrc, unsigned int nSrcSize);
	bool ParamCheck(const char *pParam);
	int CheckHTTPProtocol(const char * pData);
	char * Trim(char * str);
	char * Strtok(char * pSrc, char * pDelim, char ** pContext);

public:
	// 생성자 & 소멸자.
	CHTTPParser();
	~CHTTPParser();
	
	// URL 
	bool ExtractUrl(char *pUrl, char *pRequestUri, char *pHostName, int *pnPort);
	// URL Parameter parse.
	bool GetParamValue(const char *pString, const char *pKey, char *pValue);
	// URL Encoding.
	int EncodeURIComponent(char *pDst, char *pSrc);
	// URL Decoding.
	int DecodeURIComponent(char *pDst, char *pSrc);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Request Request-Line Set & Get Functions...
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Method Set & Get
	bool SetMethod(char *pMethod);
	char* GetMethod( ) { return this->m_pMethod; }

	// Request URI Set & Get
	bool SetRequestUri(char *pRequestUri);
	char* GetRequestUri( ) { return this->m_pRequestUri; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Response Status-Line Set & Get Functions...
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Status-Code Set & Get
	bool SetResponseStatusCode(char *pStatusCode);
	char* GetResponseStatusCode( ) { return this->m_pStatusCode; }

	// Reson-Phrase Set & Get
	bool SetResonPhrase(char *pResonPhrase);
	char* GetResonPhrase( ) { return this->m_pResonPhrase; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Common-Header Set & Get Functions...
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 프로토콜 Version 설정
	bool SetVersion(char *pVersion);
	char* GetVersion( ) { return this->m_pVersion; }
	
	// 헤더 설정.
	bool SetHeader(char *pHeader);
	bool SetHeader(char *pHeaderName, char *pValue);
	bool AddHeader(char *pHeaderName, char *pValue);
	bool RemoveHeader(char *pHeaderName);
	char* GetHeader( ) { return this->m_pHeader; }
	char* GetHeaderValue(char *pHeaderName);

	// 헤더 사이즈 
	int GetHeaderSize( ) { return this->m_nHeaderSize; }

	// 입력 데이터 파싱.
	int Parse(const char * pData, unsigned int nSize);
};
