#ifndef OBSERVABLESAMPLEDINPUT_T_H_
#define OBSERVABLESAMPLEDINPUT_T_H_

#include "../ListenableSampledInput.h"
#include "DelegateBooleanParameter.h"

#include "Mocks/DummyPhysicalInput.h"
#include "Mocks/DummyListener.h"

class ListenableSampledInputTest: public CxxTest::TestSuite
{
public:
	void testSimpleGetStateAtConstruction()
	{
		DummyPhysicalInput physicalInput(false);
		ListenableSampledInput input(physicalInput);
		TS_ASSERT(input.GetState() == false);

		DummyPhysicalInput physicalInput2(true);
		ListenableSampledInput sampledInput2(physicalInput2);
		TS_ASSERT(sampledInput2.GetState() == true);
	}

	void testSimpleAcquirement()
	{
		DummyPhysicalInput physicalInput(false);
		ListenableSampledInput input(physicalInput);

		//Low to high
		physicalInput.changePhysicalState(true);
		TS_ASSERT(input.GetState() == false);
		input.Acquire();
		TS_ASSERT(input.GetState() == true);

		//high to low
		physicalInput.changePhysicalState(false);
		TS_ASSERT(input.GetState() == true);
		input.Acquire();
		TS_ASSERT(input.GetState() == false);
	}

	void testListenability()
	{
		DummyPhysicalInput physicalInput(false);
		ListenableSampledInput input(physicalInput);
		DummyListener listener;
		TypedDelegateBooleanParameter<DummyListener> * const handler =
				new TypedDelegateBooleanParameter<DummyListener>(listener, &DummyListener::handler);
		input.addListener(handler);

		//Low to high
		physicalInput.changePhysicalState(true);
		input.Acquire();
		TS_ASSERT(listener.handlerCalledSinceLastAsk());
		TS_ASSERT(listener.lastParameterTransmitted() == true);

		//High to low
		physicalInput.changePhysicalState(false);
		input.Acquire();
		TS_ASSERT(listener.handlerCalledSinceLastAsk());
		TS_ASSERT(listener.lastParameterTransmitted() == false);
	}
};

#endif /* OBSERVABLESAMPLEDINPUT_T_H_ */
