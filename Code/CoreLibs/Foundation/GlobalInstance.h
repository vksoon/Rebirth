#pragma once

#include "Prerequisites.h"
#include "Exceptions.h"

RB_NAMESPACE_BEGIN

template< class _Type >
class GlobalInstance
{
public:
	static _Type& Ref()
	{
		CheckValidityForAccess();
		return *PrivateInstance();
	}

	static _Type* Ptr()
	{
		CheckValidityForAccess();
		return PrivateInstance();
	}

	template< class ...Args >
	static void StartUp(Args&& ...args)
	{
		if (IsStartedUp())
		{
			RB_EXCEPT(InternalErrorException, "Trying to start an already started module.");
		}

		PrivateInstance() = RB_New<_Type>(std::forward<Args>(args)...);
		IsStartedUp() = true;

		((GlobalInstance*)PrivateInstance())->OnStartUp();
	}

	template< class _SubType, class ...Args >
	static void StartUp(Args&& ...args)
	{
		static_assert(std::is_base_of<_Type, _SubType>::value, "Provided type is not derived from type the Module is initialized with.");

		if (IsStartedUp())
		{
			RB_EXCEPT(InternalErrorException, "Trying to start an already started module.");
		}

		PrivateInstance() = RB_New<_SubType>(std::forward<Args>(args)...);
		IsStartedUp() = true;

		((GlobalInstance*)PrivateInstance())->OnStartUp();
	}

	static void ShutDown()
	{
		if (IsDestroyed())
		{
			RB_EXCEPT(InternalErrorException, "Trying to shut down an already shut down module.");
		}

		if (!IsStartedUp())
		{
			RB_EXCEPT(InternalErrorException, "Trying to shut down a module which was never started.");
		}

		((GlobalInstance*)PrivateInstance())->OnShutDown();

		RB_Delete(PrivateInstance());
		IsDestroyed() = true;
	}

	static bool& IsDestroyed()
	{
		static bool sIsDestroyed = false;
		return sIsDestroyed;
	}

	static bool& IsStartedUp()
	{
		static bool sIsStartedUp = false;
		return sIsStartedUp;
	}

protected:
	GlobalInstance() = default;
	virtual ~GlobalInstance() = default;

	GlobalInstance(GlobalInstance&&) = delete;
	GlobalInstance(const GlobalInstance&) = delete;
	GlobalInstance& operator=(GlobalInstance&&) = delete;
	GlobalInstance& operator=(const GlobalInstance&) = delete;

	virtual void OnStartUp() {}
	virtual void OnShutDown() {}

private:
	static _Type*& PrivateInstance()
	{
		static _Type* Instance = nullptr;
		return Instance;
	}

	static void CheckValidityForAccess()
	{
		if (!IsStartedUp())
		{
			RB_EXCEPT(InternalErrorException, "Trying to access a module but it hasn't been started up yet.");
		}

		if (IsDestroyed())
		{
			RB_EXCEPT(InternalErrorException, "Trying to access a destroyed module.");
		}
	}
};

RB_NAMESPACE_END


