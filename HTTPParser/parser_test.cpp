#include "HTTPParser.h"

int main( )
{
	CHTTPParser parser;

//	char * pData = "GET /ajax/interest.nhn?m=getList&type=KIN HTTP/1.1\nHost: kin.naver.com\nConnection: keep-alive\nUser-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11\nContent-Type: application/x-www-form-urlencoded; charset=utf-8\ncharset: utf-8\nAccept: */*\nReferer: http://kin.naver.com/qna/detail.nhn?d1id=1&dirId=108&docId=117068896&qb=d2lyZXNoYXJrIGh0dHAgIO2XpOuNlCDrs7TquLA=&enc=utf8&section=kin&rank=1&search_sort=0&spq=0&pid=R7UiT35Y7t0ssakca20sssssstR-259909&sid=ULXZOXJvLDEAAAgiFnk\nAccept-Encoding: gzip,deflate,sdch\nAccept-Language: ko-KR,ko;q=0.8,en-US;q=0.6,en;q=0.4\nAccept-Charset: UTF-8,*;q=0.5\r\n\r\n";
	char * pData = "HTTP/1.1 200 OK\nDate: Wed, 28 Nov 2012 09:29:08 GMT\nServer: Apache\nCache-Control: no-cache\nExpires: Thu, 01 Jan 1970 00:00:00 GMT\nP3P: CP=\"ALL CURa ADMa DEVa TAIa OUR BUS IND PHY ONL UNI PUR FIN COM NAV INT DEM CNT STA POL HEA PRE LOC OTC\"\nVary: User-Agent,Accept-Encoding\nContent-Encoding: gzip\nContent-Length: 132\nConnection: close\nContent-Type: text/html;charset=utf-8\r\n\r\n............=..@.D.....ilS..Z...HH!...q..Q...[.yof...$.....#..bZ>..Y.*}G.9W.3>..(w...-..s...............F....H%.F.....b...p^.7.*....";
	parser.Parse(pData, 0);

	printf("%s\n", parser.GetHeaderValue("Content-Length"));
	printf("%d\n", parser.SetHeader("Content-Length", "11111"));

	printf("%s\n", parser.GetHeaderValue("Content-Length"));
	printf("%d\n", parser.AddHeader("Test", "test...."));
	printf("%s\n", parser.GetHeaderValue("Test"));

	return 0;
}