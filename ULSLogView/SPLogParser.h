

struct SPENTITY
{

};
struct PROCESS:SPENTITY
{
	TCHAR processName[100];
	DWORD processId;
};

struct SPLEVEL:SPENTITY
{
	TCHAR spLevel[20];

};

struct AREAINFO:SPENTITY
{
	
	TCHAR spArea[30];
};

struct CATEGORYINFO:SPENTITY
{
	
	TCHAR spCategory[50];
};

struct SPLOGHEADER
{
TCHAR timeStamp[50];
	TCHAR processInfo[50];
	TCHAR threadID[50];
	TCHAR areaInfo[50];
	TCHAR categoryInfo[50];
	TCHAR eventID[50];
	TCHAR spLevel[50];
	TCHAR Message[50];


};
struct SPLOG
{
	TCHAR timeStamp[50];
	PROCESS processInfo;
	DWORD threadID;
	AREAINFO areaInfo;
	CATEGORYINFO categoryInfo;
	DWORD eventID;
	SPLEVEL spLevel;
	TCHAR Message[1024];



};

class SPLogParser
{
private:
	
	DWORD _typeLog;
	SPLOG *spLog;
	SPLOGHEADER logHeader;
	int fileSize;
	HANDLE fileHandle;
	DWORD RecalculateSize();
	static const int SECTION_SIZE=8;
public:
	SPLogParser();
	void ParseFile(void *fileData, HANDLE);
	int ReadFile(void *fileData);
	int CloseFile();
	void* OpenFileReading(TCHAR* dirName, TCHAR fileName[]);
};