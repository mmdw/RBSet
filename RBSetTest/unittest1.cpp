#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <sstream>

#include "..\RBSet\src\RBSet.h"
#include "Vector2D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RBSetTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		/**
		 * Удаление элемента
		 */
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

		/**
		 * Копирование
		 */
		TEST_METHOD(TestCopy)
		{
			RBSet<int> set;
			set.put(3);
			set.put(2);
			set.put(1);
			set.put(4);

			set.remove(3);

			RBSet<int> set1(set);

			Assert::AreEqual(static_cast<size_t>(3), set1.size());
		}

		/**
		 * Vector2D
		 */
		TEST_METHOD(TestVector2D)
		{
			RBSet<Vector2D> set;
			set.put(Vector2D(0, 5));
			set.put(Vector2D(0, 0));
			set.put(Vector2D(0, 1));
			set.put(Vector2D(0, 2));			

			Assert::AreEqual(static_cast<size_t>(4), set.size());

			Assert::IsTrue(set.contains(Vector2D(0, 0)));
			Assert::IsTrue(set.contains(Vector2D(0, 1)));
			Assert::IsTrue(set.contains(Vector2D(0, 2)));
			Assert::IsTrue(set.contains(Vector2D(0, 5)));

			Assert::IsFalse(set.contains(Vector2D(99, 99)));
		}

		/**
		 * Vector2D Serialize
		 */
		TEST_METHOD(TestVector2DSerialization)
		{
			RBSet<Vector2D> A;
			A.put(Vector2D(0, 5));
			A.put(Vector2D(0, 0));
			A.put(Vector2D(0, 1));
			A.put(Vector2D(0, 2));			

			std::ostringstream oss;
			A.serialize(oss);

			std::istringstream iss(oss.str());
			RBSet<Vector2D> B(iss);

			Assert::AreEqual(static_cast<size_t>(4), B.size());
			Assert::IsTrue(B.contains(Vector2D(0, 0)));
			Assert::IsTrue(B.contains(Vector2D(0, 1)));
			Assert::IsTrue(B.contains(Vector2D(0, 2)));
			Assert::IsTrue(B.contains(Vector2D(0, 5)));
		}
	};
}