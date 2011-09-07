
#include "stdafx.h"
#include "ULSLogView.h"
#include "tchar.h"
#include "comutil.h"
#include "winerror.h"
#include "SPLogParser.h"


SPLogParser::SPLogParser()
{
	spLog=NULL;
}

int SPLogParser::ReadFile(void *fileData)
{

	return 0;

}

int SPLogParser::CloseFile()
{
	return 0;

}

void SPLogParser::ParseFile(void *fileData, HANDLE handle)
{
	
	//fileSize=fileSize;
	int sectionCount=0;
	int lastCount=0;
	void* fileDataPointer;

	if(spLog==NULL)
	{
		spLog=new SPLOG();
		
	}
	RecalculateSize();
	for(DWORD i=0;i<fileSize;i++)
	{

		char data=((char*)fileData)[i];
		
		if(data=='\t')
		{
			
			//DWORD newLoc=DWORD(fileData)+i;
			//void* newloc1=(void*)newLoc;
			//char c=((char*)newloc1)[0];
			//	c='\0';
			char* data=(char*)(fileData);
			 fileDataPointer=data+lastCount;
			switch(sectionCount)
			{
			case 0:
				memcpy(logHeader.timeStamp,fileDataPointer,i);
				lastCount=i;
				break;

				case 1:
					memcpy(logHeader.processInfo,fileDataPointer,i-lastCount);
					lastCount=i;
				break;

				case 2:
					memcpy(logHeader.threadID,fileDataPointer,i-lastCount);
					lastCount=i;
				break;

				case 3:
					memcpy(logHeader.areaInfo,fileDataPointer,i-lastCount);
					lastCount=i;
				break;

				case 4:
					memcpy(logHeader.categoryInfo,fileDataPointer,i-lastCount);
					lastCount=i;
				break;

				case 5:
					memcpy(logHeader.eventID,fileDataPointer,i-lastCount);
					lastCount=i;
				break;

				case 6:
					memcpy(logHeader.spLevel,fileDataPointer,i);
					lastCount=i;
				break;

				case 7:
					memcpy(logHeader.Message,fileDataPointer,i);
					lastCount=i;
				break;

			}
			
			//spLog->
			sectionCount++;
			
			
		}
		
		
	}


}

DWORD SPLogParser:: RecalculateSize()
{
	if(fileHandle!=NULL)
	{
		
		DWORD retFileSize=GetFileSize(fileHandle,NULL);
		fileSize=retFileSize;
		return retFileSize;
	}

}


void* SPLogParser::OpenFileReading(TCHAR* dirName, TCHAR fileName[])
{
	TCHAR fullFileName[MAX_PATH];
 	_tcscpy(fullFileName,dirName);
	_tcscat(fullFileName,_T("\\"));
	_tcscat(fullFileName,fileName);

	HANDLE hLogFile=CreateFile(fullFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
	
	if(hLogFile==INVALID_HANDLE_VALUE)
	{

	}

	fileHandle=hLogFile;

	DWORD fileSize;
	/*DWORD fileSize;
	DWORD retFileSize=GetFileSize(hLogFile,&fileSize);
	HGLOBAL hGlobal=GlobalAlloc(GHND,fileSize);
	void* memFile=GlobalLock(hGlobal);*/
	DWORD retFileSize=GetFileSize(hLogFile,&fileSize);
	HANDLE hFileMap=CreateFileMapping(hLogFile,NULL, PAGE_READONLY,0,0,NULL);
	DWORD a=GetLastError();

    void* memFile=MapViewOfFile(hFileMap,FILE_MAP_READ,NULL,NULL,NULL);

	
	return memFile;
	
}