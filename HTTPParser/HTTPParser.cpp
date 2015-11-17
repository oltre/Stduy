#include "HTTPParser.h"

// HTTP Element Class 정의
CHTTPElement::CHTTPElement( )
{
	this->m_pHeaderName = NULL;
	this->m_pHeaderValue = NULL;
	this->m_pBefore = NULL;
	this->m_pNext = NULL;
}

CHTTPElement::CHTTPElement(const char * pHeaderName, const char * pHeaderValue)
{
	this->m_pHeaderName = new char[strlen(pHeaderName) + sizeof(char)];
	memset( this->m_pHeaderName, '\0', strlen(pHeaderName) + sizeof(char));
	strcpy( this->m_pHeaderName, pHeaderName );

	this->m_pHeaderValue = new char[strlen(pHeaderValue) + sizeof(char)];	
	memset( this->m_pHeaderValue, '\0', strlen(pHeaderValue) + sizeof(char));
	strcpy( this->m_pHeaderValue, pHeaderValue );

	this->m_pBefore = NULL;
	this->m_pNext = NULL;
}

CHTTPElement::~CHTTPElement( )
{
	if ( this->m_pHeaderName )
		delete [] this->m_pHeaderName;
	if ( this->m_pHeaderValue )
		delete [] this->m_pHeaderValue;

	this->m_pBefore = NULL;
	this->m_pNext = NULL;
}

void CHTTPElement::SetHeaderValue(const char * pHeaderValue)
{
	if ( this->m_pHeaderValue )
		delete [] this->m_pHeaderValue;

	this->m_pHeaderValue = new char[strlen(pHeaderValue) + sizeof(char)];
	memset( this->m_pHeaderValue, '\0', strlen(pHeaderValue) + sizeof(char));
	strcpy( this->m_pHeaderValue, pHeaderValue );
}

// 
// 생성자
//
CHTTPParser::CHTTPParser()
{
	this->m_pHeader = NULL;
	this->m_pBody = NULL;
	this->m_pMethod = NULL;
	this->m_pRequestUri = NULL;
	this->m_pStatusCode = NULL;
	this->m_pResonPhrase = NULL;
	this->m_pVersion = NULL;

	this->m_nHeaderSize = 0;
	this->m_nBodySize = 0;
	this->m_nContentLength = 0;
	this->m_nVersion = 0;

	this->m_pHead = new CHTTPElement( );
	this->m_pTail = new CHTTPElement( );
	this->m_pHead->m_pBefore = NULL;
	this->m_pHead->m_pNext = this->m_pTail;
	this->m_pTail->m_pBefore = this->m_pHead;
	this->m_pTail->m_pNext = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// 소멸자
//
CHTTPParser::~CHTTPParser()
{
	CHTTPElement * pTemp;

	if ( this->m_pHeader != NULL )
		delete [] this->m_pHeader;
	if ( this->m_pMethod != NULL )
		delete [] this->m_pMethod;
	if ( this->m_pRequestUri != NULL )
		delete [] this->m_pRequestUri;
	if ( this->m_pStatusCode != NULL )
		delete [] this->m_pStatusCode;
	if ( this->m_pResonPhrase != NULL )
		delete [] this->m_pResonPhrase;
	if ( this->m_pVersion != NULL ) 
		delete [] this->m_pVersion;
	if ( this->m_pBody != NULL )
		delete [] this->m_pBody;

	this->m_pHeader = NULL;
	this->m_pBody = NULL;
	this->m_pMethod = NULL;
	this->m_pRequestUri = NULL;
	this->m_pStatusCode = NULL;
	this->m_pResonPhrase = NULL;
	this->m_pVersion = NULL;

	this->m_nHeaderSize = 0;
	this->m_nBodySize = 0;
	this->m_nVersion = 0;
	this->m_nContentLength = 0;

	
	while ( this->m_pHead->m_pNext != this->m_pTail ) 
	{
		pTemp = this->m_pHead->m_pNext;
		this->m_pHead->m_pNext = pTemp->m_pNext;
		delete pTemp;
	}

	delete this->m_pHead;
	delete this->m_pTail;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// 클래스 내부에서 사용 될 메서드
//

// 버퍼 셋팅.
bool CHTTPParser::SetBuffer(char *pDst, unsigned int nDstSize, char *pSrc, unsigned int nSrcSize)
{
	int err;
	pDst = new char[(nDstSize * sizeof(char)) + sizeof(char)];
	memset(pDst, '\0', (nDstSize * sizeof(char)) + sizeof(char));
	err = memcpy_s(pDst, (nDstSize * sizeof(char)) + sizeof(char), pSrc, nSrcSize * sizeof(char));
	if ( err )
		return false;

	return true;
}

// 입력 값 체크
bool CHTTPParser::ParamCheck(const char *pParam)
{
	if ( pParam == NULL )
		return false;

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CHTTPParser::CheckHTTPProtocol(const char * pData)
{
	if ( ! strncmp( pData, "HTTP", 4 ) )
		return RESPONSE_HEADER;
	if ( ! strncmp( pData, "OPTIONS", 7 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "GET", 3 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "HEAD", 4 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "POST", 4 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "PUT", 3 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "DELETE", 6 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "TRACE", 5 ) )
		return REQUEST_HEADER;
	if ( ! strncmp( pData, "CONNECT", 7 ) )
		return REQUEST_HEADER;

	return NOT_HTTP_PACKET;
}

char * CHTTPParser::Trim(char * str)
{
	char * end;
	if ( ! str )
		return NULL;

	end = str + strlen(str) - 1;
	if ( end <= str )
		return NULL;

	// Left Trim.
	while ( (str != end) && isspace(*str) )
		str ++;

	// Right Trim
	while ( (str != end) && isspace(*end) )
		end --;

	*(end + 1) = '\0';

	return str;
}

char * CHTTPParser::Strtok(char * pSrc, char * pDelim, char ** pContext)
{
	char * pToken;
	char * pNext;
	char * pDelimPos;
	bool bFindDelim = false;

	if ( pSrc != NULL )
		pNext = pSrc;
	else
		pNext = *pContext;

	if ( *pNext == '\0' )
		return NULL;

	pToken = pNext;
	while ( *pNext != '\0' )
	{
		pDelimPos = pDelim;
		while ( *pDelimPos != '\0' )
		{
			if ( *pNext == *pDelimPos )
			{
				*pNext = '\0';
				bFindDelim = true;
				break;
			}
			pDelimPos ++;
		}

		pNext ++;
		if ( bFindDelim )
			break;
	}

	*pContext = pNext;
	return pToken;
}

//
// URL 분리.
//
bool CHTTPParser::ExtractUrl(char *pUrl, char *pRequestUri, char *pHostName, int *pnPort)
{
	bool bIsHttps = false;

	int nHostLen;
	int nPortLen;
	int nPathLen;

	char * pHost;
	char * pHostTemp;
	char * pPort;
	char * pPath;
	char * pSearchPart;

	char szPort[5];
	
	if ( ! this->ParamCheck( pUrl ) )
	{
		//printf("[ERROR] CHTTPParser::ExtractUrl( ) FirstMethod is NULL\n");
		return false;
	}

	if ( memcmp( pUrl, "http://", 7 ) == 0 )
		pHost = (char *) (pUrl + 7);
	else if ( memcmp( pUrl, "https://", 8 ) == 0 )
	{
		pHost = (char *) (pUrl + 8);
		bIsHttps = true;
	}
	else
		pHost = (char *) pUrl;

	pPath = strchr(pHost, '/');
	pSearchPart = strchr(pHost, '?');

	if ( (pPath == NULL) && (pSearchPart == NULL) )
		nHostLen = strlen(pHost); // URL이 <Host> or <Host>:<Port> 인 경우
	else if ( pPath && (pSearchPart == NULL) )
		nHostLen = pPath - pHost; // URL이 <Host>/<Path> 인 경우
	else if ( pSearchPart && (pPath == NULL) )
		nHostLen = pSearchPart - pHost; // URL이 <Host>?<SearchPart>인 경우
	else if ( pPath < pSearchPart )
		nHostLen = pPath - pHost;  // URL이 <Host>/<Path>?<SearchPart>인 경우
	else
		nHostLen = pSearchPart - pHost; // URL이 <Host>?<SearchPart>/<Path>인 경우

	// HostName을 찾아내서 가져가길 원할 경우
	if ( pHostName != NULL )
	{
		memcpy(pHostName, pHost, nHostLen);
		pHostName[nHostLen] = '\0';
	
		// HostName에 포트번호 있는지 확인 후 저장
		pPort = strchr(pHostName, ':');
		if ( pPort )
		{
			nPortLen = nHostLen;
			nHostLen = pPort - pHostName;
			nPortLen = nPortLen - nHostLen - 1;

			if ( nPortLen == 0 )
				return false;

			memset(pHostName, '\0', nHostLen + nPortLen + 1);
			memcpy(pHostName, pHost, nHostLen);
			pHostName[nHostLen] = '\0';

			memcpy(szPort, ++pPort, nPortLen);
			szPort[nPortLen] = '\0';
			if ( pnPort )
				*pnPort = atoi(szPort);
		}
		else
		{
			if ( pnPort )
			{
				if ( bIsHttps == true )
					*pnPort = 443;
				else
					*pnPort = 80;
			}
		}
	}
	// HostName을 가져가길 원하지 않는 경우.
	else 
	{
		pHostTemp = new char[nHostLen];
		memcpy(pHostTemp, pHost, nHostLen);
		pHostTemp[nHostLen] = '\0';
	
		// HostName에 포트번호 있는지 확인 후 저장
		pPort = strchr(pHostTemp, ':');
		if ( pPort )
		{
			nPortLen = nHostLen;
			nHostLen = pPort - pHostTemp;
			nPortLen = nPortLen - nHostLen - 1;

			if ( nPortLen == 0 )
				return false;

			memset(pHostTemp, '\0', nHostLen + nPortLen + 1);
			memcpy(pHostTemp, pHost, nHostLen);
			pHostTemp[nHostLen] = '\0';

			memcpy(szPort, ++pPort, nPortLen);
			szPort[nPortLen] = '\0';
			if ( pnPort )
				*pnPort = atoi(szPort);
		}
		else
		{
			if ( pnPort )
			{
				if ( bIsHttps == true )
					*pnPort = 443;
				else
					*pnPort = 80;
			}
		}

		delete [] pHostTemp;
	}

	// Request URI 찾기.
	if ( pRequestUri )
	{
		if ( pPath && (pSearchPart == NULL) )
			strcpy( pRequestUri, pPath ); // URL이 <Host>/<Path>인 경우
		else if ( pSearchPart && (pPath == NULL) )
			sprintf( pRequestUri, "/%s", pSearchPart ); // URL이 <Host>?<SearchPart>인 경우
		else if ( pPath < pSearchPart )
			strcpy( pRequestUri, pPath ); // URL이 <Host>/<Path>?<SearchPart>인 경우
		else
			sprintf( pRequestUri, "/%s", pSearchPart ); // URL이 <Host>?<SearchPart>/<Path>인 경우
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// URL에서 Param key와 Value 가져오기
//
bool CHTTPParser::GetParamValue(const char * pString, const char * pKey, char * pValue)
{
	pValue[0] = '\0';

	char * pKeyString = new char[strlen(pKey) + 2];
	strcpy(pKeyString, pKey);
	strcat(pKeyString, "=");

	const char * pSearch = strstr(pString, pKeyString);
	delete [] pKeyString;

	if (pSearch == NULL)
		return false;

	int nIndex = (int)(pSearch - pString);
	if (nIndex > 0 && 
		pString[nIndex - 1] != '?' &&
		pString[nIndex - 1] != '&' &&
#if 0
		pString[nIndex - 1] != ' ' && pString[nIndex - 1] != '\\' &&
		pString[nIndex - 1] != '/' && pString[nIndex - 1] != ':' && 
		pString[nIndex - 1] != '\"' && pString[nIndex - 1] != '\'' &&
#endif
		pString[nIndex - 1] != ';')
	{
		return false;
	}

	int nStart = strlen(pKey) + 1;
	int nCount = strlen(pSearch);
	int i;
	for (i = nStart; i < nCount; i++)
	{
		if (pSearch[i] == '?' ||
			pSearch[i] == '&' ||
#if 0
			pSearch[i] == ' ' || pSearch[i] == '\\' || 
			pSearch[i] == '/' || pSearch[i] == ':' || 
			pSearch[i] == '\"' ||	pSearch[i] == '\'' ||
#endif
			pSearch[i] == ';')
		{
			break;
		}
	}

	if (nStart == i)
	{
		pValue[0] = '\0';
		return true;
	}

	strncpy(pValue, pSearch + nStart, i - nStart);
	pValue[i - nStart] = '\0';

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CHTTPParser::EncodeURIComponent(char *pDst, char *pSrc)
{
	char szHex[4];
	int nLength = 0;
	char * pOffset = pDst;

	while ( *pSrc )
	{
		if ( (*pSrc > 47 && *pSrc < 57) ||
				(*pSrc > 64 && *pSrc < 92) ||
				(*pSrc > 96 && *pSrc < 123) ||
				*pSrc == '-' || *pSrc == '.' || *pSrc == '_' )
		{
			*pOffset = *pSrc;
		}
		else
		{
			sprintf( szHex, "%%%02X", *pSrc );
			strncat( pOffset, szHex, 3 );
			*pOffset ++;
			*pOffset ++;
			nLength += 2;
		}

		*pSrc ++;
		*pOffset ++;
		nLength ++;
	}

	pDst[nLength] = '\0';

	return nLength;
}

int CHTTPParser::DecodeURIComponent(char *pDst, char *pSrc)
{
	int i, nNum = 0, nLength = 0;
	int nTemp = 0;

	while ( *pSrc )
	{
		if ( *pSrc == '%' )
		{
			nNum = 0;
			nTemp = 0;

			for (i = 0; i < 2; i ++) 
			{
				*pSrc ++;
				if ( *(pSrc) < ':' )
					nNum = *(pSrc) - 48;
				else if ( *(pSrc) > '@' && *(pSrc) < '[' )
					nNum = (*(pSrc) - 'A') + 10;
				else
					nNum = (*(pSrc) - 'a') + 10;

				if ( (16*(1-i)) )
					nNum = (nNum * 16);
				nTemp += nNum;
			}

			pDst[nLength] = nTemp;
			nLength ++;
		}
		else
		{
			pDst[nLength] = *pSrc;
			nLength ++;
		}

		*pSrc++;
	}

	pDst[nLength] = '\0';

	return nLength;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Request Request-Line Set & Get Functions...
///////////////////////////////////////////////////////////////////////////////////////////////////

// Method Set 
bool CHTTPParser::SetMethod(char *pMethod)
{
	if (this->m_pMethod)
		delete [] this->m_pMethod;

	if ( pMethod == NULL )
		return false;

	this->m_pMethod = new char[strlen(pMethod) + sizeof(char)];
	strcpy(this->m_pMethod, pMethod);
	
	return true;
}

// Request URI Set & Get
bool CHTTPParser::SetRequestUri(char *pRequestUri)
{
	if (this->m_pRequestUri)
		delete [] this->m_pRequestUri;

	if ( pRequestUri == NULL )
		return false;

	this->m_pRequestUri = new char[strlen(pRequestUri) + sizeof(char)];
	strcpy(this->m_pRequestUri, pRequestUri);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Response Status-Line Set & Get Functions...
///////////////////////////////////////////////////////////////////////////////////////////////////

// Status-Code Set & Get
bool CHTTPParser::SetResponseStatusCode(char *pStatusCode)
{
	if (this->m_pStatusCode)
		delete [] this->m_pStatusCode;

	if ( pStatusCode == NULL )
		return false;

	this->m_pStatusCode = new char[strlen(pStatusCode) + sizeof(char)];
	strcpy(this->m_pStatusCode, pStatusCode);

	return true;
}

// Reson-Phrase Set & Get
bool CHTTPParser::SetResonPhrase(char *pResonPhrase)
{
	if ( this->m_pResonPhrase )
		delete [] this->m_pResonPhrase;

	if ( pResonPhrase == NULL )
		return false;

	this->m_pResonPhrase = new char[strlen(pResonPhrase) + sizeof(char)];
	strcpy( this->m_pResonPhrase, pResonPhrase );

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Common-Header Set & Get Functions...
///////////////////////////////////////////////////////////////////////////////////////////////////

// 프로토콜 Version 설정
bool CHTTPParser::SetVersion(char *pVersion)
{
	if (this->m_pVersion)
		delete [] this->m_pVersion;

	if (pVersion != NULL)
		return false;

	this->m_pVersion = new char[strlen(pVersion) + sizeof(char)];
	strcpy(this->m_pVersion, pVersion);

	return true;
}

// 헤더 설정.
bool CHTTPParser::SetHeader(char *pHeader)
{
	if ( this->m_pHeader )
		delete [] this->m_pHeader;

	if ( pHeader == NULL ) 
		return false;

	this->m_pHeader = new char[strlen(pHeader) + sizeof(char)];
	strcpy ( this->m_pHeader, pHeader );

	return true;
}

bool CHTTPParser::SetHeader(char *pHeaderName, char *pValue)
{
	CHTTPElement * pTemp;

	if ( (pHeaderName == NULL) || (pValue == NULL) )
		return false;

	// Linked List를 순회하면서 값을 찾는다.
	pTemp = m_pHead;
	while ( pTemp->m_pNext != this->m_pTail )
	{
		pTemp = pTemp->m_pNext;

		// 헤더 이름을 찾았을 경우 값을 변경
		if ( ! strnicmp( pTemp->GetHeaderName(), pHeaderName, strlen(pHeaderName) ) )
		{
			pTemp->SetHeaderValue(pValue);
			return true;
		}
	}

	// 헤더 이름이 없을 경우 추가 해준다.
	return this->AddHeader( pHeaderName, pValue );
}

bool CHTTPParser::AddHeader(char *pHeaderName, char *pValue)
{
	CHTTPElement * pElement;
	CHTTPElement * pTemp;

	if ( (pHeaderName == NULL) || (pValue == NULL) )
		return false;

	// Linked List를 순회하면서 값을 찾는다.
	pTemp = this->m_pHead;
	while ( pTemp->m_pNext != this->m_pTail )
	{
		pTemp = pTemp->m_pNext;

		// 헤더 이름이 Linke List에 존재하면 추가 할 필요가 없기에 실패 반환
		if ( ! strnicmp( pTemp->GetHeaderName(), pHeaderName, strlen(pHeaderName) ) )
			return false;
	}

	// 새로운 원소를 만든다.
	pElement = new CHTTPElement( pHeaderName, pValue );

	// 새로운 헤더 내용을 Linked List의 마지막에 넣는다.
	pTemp->m_pNext = pElement;
	this->m_pTail->m_pBefore = pElement;

	pElement->m_pBefore = pTemp;
	pElement->m_pNext = this->m_pTail;

	return true;
}

bool CHTTPParser::RemoveHeader(char *pHeaderName)
{
	CHTTPElement * pTemp;

	if ( pHeaderName == NULL )
		return false;

	// Linked List를 순회하면서 값을 찾는다.
	pTemp = m_pHead;
	while ( pTemp->m_pNext != this->m_pTail )
	{
		pTemp = pTemp->m_pNext;

		// 헤더 이름이 Linke List에 존재하면 추가 할 필요가 없기에 실패 반환
		if ( ! strcmp( pTemp->GetHeaderName(), pHeaderName ) )
		{
			pTemp->m_pBefore->m_pNext = pTemp->m_pNext;
			pTemp->m_pNext->m_pBefore = pTemp->m_pBefore;

			delete pTemp;

			return true;
		}
	}

	return false;
}

char* CHTTPParser::GetHeaderValue(char *pHeaderName)
{
	CHTTPElement * pTemp;

	if ( pHeaderName == NULL )
		return NULL;

	// Linked List를 순회하면서 값을 찾는다.
	pTemp = m_pHead;
	while ( pTemp->m_pNext != this->m_pTail )
	{
		pTemp = pTemp->m_pNext;

		// 헤더 이름으로 비교하여 해당 값 리턴.
		if ( ! strcmp( pTemp->GetHeaderName(), pHeaderName ) )
			return pTemp->GetHeaderValue( );
	}

	return NULL;
}

// 입력 데이터 파싱.
int CHTTPParser::Parse(const char * pData, unsigned int nSize)
{
	char * pHeader;
	char * pHeaderTmp;
	char * pHeaderCurLine;
	char * pHeaderNextLine;

	char szHeaderName[256];
	char szHeaderValue[512];

	char * pToken;
	char * pSavePtr;

	const char * pBodyPtr;

	int nChkProtocolType;

	unsigned int nRemainSize;
	unsigned int nHeaderSize;

	bool bIsLastHeader = false;

	// pData가 없을 경우.
	if ( pData == NULL )
		return PARAMETER_ERROR;

	// HTTP 헤더가 있는지 확인.
	pBodyPtr = strstr(pData, "\r\n\r\n");
	if ( pBodyPtr == NULL )
		return HEADER_REMAIN_PACKET;

	// HTTP 헤더를 받았을 경우 HTTP 헤더를 파싱하기 위해
	// HTTP 헤더를 임시로 저장하기 위하여 HTTP 헤더 사이즈를 구함.
	nHeaderSize = pBodyPtr - pData;

	// HTTP Body를 가르키기 위하여 포인터 위치 이동.
	pBodyPtr += strlen("\r\n\r\n");

	// HTTP 패킷인지 체크. (HTTP Packet이라면 Request인지 Response인지 확인)
	nChkProtocolType = this->CheckHTTPProtocol( pData );
	if ( nChkProtocolType == NOT_HTTP_PACKET )
		return NOT_HTTP_PACKET;

	// HTTP 헤더를 임시 버퍼에 복사. (헤더 파싱을 하기 위함)
	pHeader = new char[nHeaderSize + sizeof(char)];
 	memset( pHeader, '\0', nHeaderSize + sizeof(char) );
	strncpy( pHeader, pData, nHeaderSize );

	// 헤더의 첫 라인 읽기
	pHeaderNextLine = strchr( pHeader, '\n' );
	pHeaderNextLine++;
	if ( pHeaderNextLine == NULL )
		return NOT_HTTP_PACKET;

	pHeaderCurLine = new char[ (pHeaderNextLine - pHeader) + sizeof(char) ];
	memset( pHeaderCurLine, '\0', (pHeaderNextLine - pHeader) + sizeof(char) );
	strncpy( pHeaderCurLine, pHeader, pHeaderNextLine - pHeader );

	// HTTP 헤더를 파싱. 
	if ( nChkProtocolType == REQUEST_HEADER )
	{
		// 헤더가 Request 헤더일 경우
			
		// Request-Line 파싱.
		// Method 값 가져오기
		//pToken = strtok_r ( pHeaderCurLine, ' ', & pSavePtr );
		pToken = this->Strtok( pHeaderCurLine, " ", & pSavePtr );
		if ( pToken == NULL )
		{
			delete [] pHeader;
			delete [] pHeaderCurLine;
			return NOT_HTTP_PACKET;
		}

		this->m_pMethod = new char[strlen(pToken) + sizeof(char)];
		memset( this->m_pMethod, '\0', strlen(pToken) + sizeof(char) );
		strcpy( this->m_pMethod, pToken );

		// Request URI 가져오기
		pToken = this->Strtok ( NULL, " ", & pSavePtr );
		if ( pToken == NULL )
		{
			delete [] pHeader;
			delete [] pHeaderCurLine;
			return NOT_HTTP_PACKET;
		}

		this->m_pRequestUri = new char[strlen(pToken) + sizeof(char)];
		memset( this->m_pRequestUri, '\0', strlen(pToken) + sizeof(char) );
		strcpy( this->m_pRequestUri, pToken );

		// HTTP Version 가져오기
		pToken = this->Strtok ( NULL, "\r\n", & pSavePtr );
		if ( pToken == NULL )
		{
			delete [] pHeader;
			delete [] pHeaderCurLine;
			return NOT_HTTP_PACKET;
		}

		this->m_pVersion = new char[strlen(pToken) + sizeof(char)];
		memset( this->m_pVersion, '\0', strlen(pToken) + sizeof(char) );
		strcpy( this->m_pVersion, pToken );
	}
	else if ( nChkProtocolType == RESPONSE_HEADER )
	{
		// 헤더가 Response 헤더일 경우
		
		// Status-Line 파싱.
		// HTTP Version 가져오기
		pToken = this->Strtok ( pHeaderCurLine, " ", & pSavePtr );
		if ( pToken == NULL )
		{
			delete [] pHeader;
			delete [] pHeaderCurLine;
			return NOT_HTTP_PACKET;
		}

		this->m_pVersion = new char[strlen(pToken) + sizeof(char)];
		memset( this->m_pVersion, '\0', strlen(pToken) + sizeof(char) );
		strcpy( this->m_pVersion, pToken );
		
		// Status Code 값 가져오기
		pToken = this->Strtok ( NULL, " ", & pSavePtr );
		if ( pToken == NULL )
		{
			delete [] pHeader;
			delete [] pHeaderCurLine;
			return NOT_HTTP_PACKET;
		}

		this->m_pStatusCode = new char[strlen(pToken) + sizeof(char)];
		memset( this->m_pStatusCode, '\0', strlen(pToken) + sizeof(char) );
		strcpy( this->m_pStatusCode, pToken );

		// ResonPhrase 가져오기
		pToken = this->Strtok ( NULL, "\r\n", & pSavePtr );
		if ( pToken == NULL )
		{
			delete [] pHeader;
			delete [] pHeaderCurLine;
			return NOT_HTTP_PACKET;
		}

		this->m_pResonPhrase = new char[strlen(pToken) + sizeof(char)];
		memset( this->m_pResonPhrase, '\0', strlen(pToken) + sizeof(char) );
		strcpy( this->m_pResonPhrase, pToken );
	}

	if ( ! strcmp( this->m_pVersion, "HTTP/0.9" ) )
		this->m_nVersion = HTTPVERSION09;
	else if ( ! strcmp( this->m_pVersion, "HTTP/1.0" ) )
		this->m_nVersion = HTTPVERSION10;
	else if ( ! strcmp( this->m_pVersion, "HTTP/1.1" ) )
		this->m_nVersion = HTTPVERSION11;
	else
	{
		delete [] pHeader;
		delete [] pHeaderCurLine;
		return NOT_HTTP_PACKET;
	}

	// 남은 헤더 내용을 파싱
	while ( ! bIsLastHeader )
	{
		pHeaderTmp = pHeaderNextLine;
		pHeaderNextLine = strchr( pHeaderTmp, '\n' );
		if ( pHeaderNextLine == NULL )
		{
			bIsLastHeader = true;

			pHeaderNextLine = strchr( pHeaderTmp, '\0' );
		}
		delete [] pHeaderCurLine;

		pHeaderCurLine = new char[ (pHeaderNextLine - pHeaderTmp) + sizeof(char) ];
		memset( pHeaderCurLine, '\0', (pHeaderNextLine - pHeaderTmp) + sizeof(char) );
		strncpy( pHeaderCurLine, pHeaderTmp, pHeaderNextLine - pHeaderTmp );
		pHeaderNextLine ++;

		pToken = this->Strtok( pHeaderCurLine, ":", & pSavePtr );
		pToken = this->Trim(pToken);
		if ( pToken == NULL )
			continue;

		memset( szHeaderName, '\0', sizeof(char) * 256 );
		strcpy( szHeaderName, pToken );

		pToken = this->Strtok( NULL, "\n", & pSavePtr );
		pToken = this->Trim(pToken);
		if ( pToken == NULL )
			continue;

		memset( szHeaderValue, '\0', sizeof(char) * 512 );
		strcpy( szHeaderValue, pToken );

		this->AddHeader( szHeaderName, szHeaderValue );

		// Content - Length가 있는지 확인.
		if ( ! stricmp( szHeaderName, "Content-Length" ) )
			this->m_nContentLength = atol( szHeaderValue );
	}

	if ( pHeader )
		delete [] pHeader;
	if ( pHeaderCurLine )
		delete [] pHeaderCurLine;
	
	// 실제 헤더 사이즈 저장.
	this->m_nHeaderSize = pBodyPtr - pData;

	// 실제 헤더 저장.
	if ( this->m_nHeaderSize > 0 )
	{
		this->m_pHeader = new char[this->m_nHeaderSize + sizeof(char)];
		memcpy(m_pHeader, pData, this->m_nHeaderSize);
		this->m_pHeader[this->m_nHeaderSize] = '\0';
	}

	// 헤더 사이즈와 입력한 데이터의 길이가 갔다면 파싱 완료.
	if ( this->m_nHeaderSize == nSize )
		return COMPLETE_PACKET;

	// 바디 사이즈 확인
	this->m_nBodySize = strlen( pBodyPtr );

	// 바디 사이즈가 존재할 경우 저장.
	if ( this->m_nBodySize > 0 )
	{
		this->m_pBody = new char[this->m_nBodySize + sizeof(char)];
		memcpy(m_pBody, pBodyPtr, this->m_nBodySize);
		this->m_pBody[this->m_nBodySize] = '\0';
	}

	if ( this->m_nContentLength == 0 )
		return BODYSIZE_UNKNOWN_PACKET;
	else if ( this->m_nContentLength == this->m_nBodySize )
		return COMPLETE_PACKET;
	else if ( this->m_nContentLength > this->m_nBodySize )
		return BODY_REMAIN_PACKET;

	return JOIN_PACKET;
}
