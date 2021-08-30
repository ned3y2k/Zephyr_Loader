#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>

#include "../DiskEmulator/CdFs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestDiskEmulator
{
	TEST_CLASS(TestCdFs)
	{
	private:
		CCdFs* p;

	public:
		TestCdFs() {
			p = new CCdFs();			
		}

		TEST_METHOD(TestMethod1) {
//			Assert::AreEqual(false, p->GetFileName(0, 0, nullptr));
			//Logger::WriteMessage(fileName);
		}
	};
}