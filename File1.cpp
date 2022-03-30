#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <cmath>

void ViewHex(byte vNTFS_BootRecord[],int size)
{
	for (int i = 0; i < size; i++)
	{
	std::wcout<< std::hex <<vNTFS_BootRecord[i]<<" ";
	}
	std::wcout<<"\n";
}
void ViewDec(byte vNTFS_BootRecord[],int size)
{
	long sum=0;
	for (int i = 0; i < size; i++)
	{
		sum+=vNTFS_BootRecord[i]*std::pow(16,i*2);
	//std::wcout<<vNTFS_BootRecord[i]<<" ";
	}
	std::wcout<< std::dec<<sum<<"\n";
}

int main()
{
    setlocale (LC_ALL, ".866");

	wchar_t *fileName =L"\\\\.\\C:";

   HANDLE fileHander=CreateFileW(fileName,
   GENERIC_READ,
   FILE_SHARE_READ | FILE_SHARE_WRITE,
   NULL,
   OPEN_EXISTING,
   FILE_ATTRIBUTE_NORMAL,
   NULL);

	if(fileHander == INVALID_HANDLE_VALUE)
	{
		std::wcout<<L"Не удалось открыть диск C:.\nИспользуйте запуск с правами администратора.\n";
		system("pause");
		return 1;
	}



	byte *dataBuffer = new byte[512];


	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart=0;


	unsigned long currentPosition = SetFilePointer(fileHander,sectorOffset.LowPart,&sectorOffset.HighPart ,FILE_BEGIN);

	 if(currentPosition != sectorOffset.LowPart)  {
	  return 2;
	 }

	 DWORD bytesToRead=512;
	 DWORD bytesRead;
	 bool readResult = ReadFile(fileHander,dataBuffer,bytesToRead,&bytesRead,NULL);

	 if(readResult == false || bytesRead != bytesToRead){
	  return 3;
	 }


	NTFS_BootRecord* pNTFS_BootRecord = (NTFS_BootRecord*)dataBuffer;


	std::wcout<<L"Колличество байт на сектор: ";
	ViewDec(pNTFS_BootRecord->BPS,sizeof(pNTFS_BootRecord->BPS));
	std::wcout<<L"Количество секторов в кластере: ";
	ViewDec(&pNTFS_BootRecord->SPC,sizeof(pNTFS_BootRecord->SPC));
	std::wcout<<L"Тип носителя: ";
	ViewHex(&pNTFS_BootRecord->MediaDescription,sizeof(pNTFS_BootRecord->MediaDescription));
	std::wcout<<L"Число секторов в томе: ";
	ViewDec(pNTFS_BootRecord->TotalSector,sizeof(pNTFS_BootRecord->TotalSector));
	std::wcout<<L"Стартовый кластер MFT: ";
	ViewHex(pNTFS_BootRecord->LogClustNumbMFT,sizeof(pNTFS_BootRecord->LogClustNumbMFT));
	std::wcout<<L"Стартовый кластер копии MFT: ";
	ViewHex(pNTFS_BootRecord->LogClustNumbMFTMirr,sizeof(pNTFS_BootRecord->LogClustNumbMFTMirr));
	std::wcout<<L"Кластеров на запись MFT: ";
	ViewDec(pNTFS_BootRecord->ClusPFRS,sizeof(pNTFS_BootRecord->ClusPFRS));
	std::wcout<<L"Кластеров на индексную запись: ";
	ViewDec(&pNTFS_BootRecord->ClusPIB,sizeof(pNTFS_BootRecord->ClusPIB));
	std::wcout<<L"Серийный номер тома: ";
	ViewHex(pNTFS_BootRecord->VolumeSerNum,sizeof(pNTFS_BootRecord->VolumeSerNum));
	std::wcout<<L"Чексумма: ";
	ViewHex(pNTFS_BootRecord->Checksum,sizeof(pNTFS_BootRecord->Checksum));
	system("pause");

	CloseHandle(fileHander);
	delete[] dataBuffer;

	return 0;
}





