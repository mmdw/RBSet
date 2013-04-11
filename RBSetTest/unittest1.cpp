#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\RBSet\src\RBSet.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RBSetTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestRemove)
		{
			RBSet<int> set;
			set.put(3);
			set.put(2);
			set.put(1);
			set.put(4);

			set.remove(3);

			Assert::AreEqual(set.size(), static_cast<size_t>(3));
		}
	};
}