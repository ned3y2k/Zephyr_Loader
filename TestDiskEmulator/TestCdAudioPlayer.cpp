#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include "../DiskEmulator/CdAudioPlayer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestDiskEmulator
{
	TEST_CLASS(TestCdAudioPlayer)
	{
	private:
		CCdAudioPlayer* p;

	public:
		TestCdAudioPlayer() {
			p = new CCdAudioPlayer();
			p->SetRootPath("ROOT_PATH");
		}
		TEST_METHOD(TestMethod1) {
			Assert::AreEqual(false, p->GetFileName(0, 0, nullptr));
			
			char fileName[MAX_PATH];
			ZeroMemory(fileName, MAX_PATH);

			p->GetFileName(1, 2, fileName);
			Logger::WriteMessage(fileName);
			Logger::WriteMessage("\n");

			p->GetFilePath(1, 2, fileName);
			Logger::WriteMessage(fileName);
		}
	};
}
