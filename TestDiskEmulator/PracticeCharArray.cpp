#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PracticeDiskEmulator
{
	TEST_CLASS(PracticeCharArray)
	{
	public:
		BEGIN_TEST_METHOD_ATTRIBUTE(FailCopyToConstCharArrayToPSTR)
			TEST_IGNORE()
			TEST_DESCRIPTION("아래 케이스는 실패함 원본 보다 메모리가 커야한다.")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(FailCopyToConstCharArrayToPSTR) {
			char arr[] = "TestMe";
			const size_t arrStrLen = strlen(arr);
			const size_t charSize = sizeof(char);

			
			PSTR destination;

			destination = (PSTR)calloc(arrStrLen, charSize);
			Assert::ExpectException<std::bad_alloc>(
				[destination, arrStrLen, arr]()->errno_t { return strcpy_s(destination, arrStrLen, arr); }
			);

			free(destination);

			destination = (PSTR)calloc(arrStrLen, charSize);
			Assert::ExpectException<std::bad_alloc>(
				[destination, arrStrLen, arr]()->errno_t { return strcpy_s(destination, arrStrLen + 1, arr); }
			);
			free(destination);
		}

		/////////////////////////////////////////////////////////////////////////
		TEST_METHOD(CopyToConstCharArrayToPSTR) {
			char arr[] = "TestMe";
			const size_t arrStrLen = strlen(arr);
			const size_t charSize = sizeof(char);

			PSTR destination = (PSTR)calloc(arrStrLen + 1, charSize);
			strcpy_s(destination, arrStrLen+1, arr);
			free(destination);
		}
	};
}