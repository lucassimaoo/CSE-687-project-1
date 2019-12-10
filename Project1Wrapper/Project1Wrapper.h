#pragma once

using namespace System;

namespace Project1Wrapper {

	template<class T>
	public ref class ManagedTestHarness
	{
	protected:
		T* harnessInstance;
	public:
		ManagedTestHarness(T* instance) : harnessInstance(instance) {}
		virtual ~ManagedTestHarness() {
			if (harnessInstance != nullptr) {
				delete harnessInstance;
			}
		}
		!ManagedTestHarness() {
			if (harnessInstance != nullptr) {
				delete harnessInstance;
			}
		}
		T* GetInstance() {
			return harnessInstance;
		}
	};
}
