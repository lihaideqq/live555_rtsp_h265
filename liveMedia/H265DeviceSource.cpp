#include "H265DeviceSource.hh"

#ifdef __cplusplus
extern "C" {
#endif
#include "fifo.h"
#ifdef __cplusplus
}
#endif
DeviceSource*
H265DeviceSource::createNew(UsageEnvironment& env) {
	return new H265DeviceSource(env);
}

H265DeviceSource::H265DeviceSource(UsageEnvironment& env) :
		DeviceSource(env, DeviceParameters()) {
}

H265DeviceSource::~H265DeviceSource() {
	// TODO Auto-generated destructor stub
	delete[] fBuffer;	
}
unsigned H265DeviceSource::maxFrameSize() const {
  // By default, this source has no maximum frame size.
  return 140 *1024;
}

unsigned char buffer_fo[200 *1024] ={0};
int length = 0; 
void H265DeviceSource::doGetNextFrame() {
	if (!isCurrentlyAwaitingData())
		return; // we're not ready for the data yet

	if(ReadQueue(buffer_fo, &length, 0) < 0)
		usleep(100);

	// Deliver the data here:
	if (length < 0) {
		handleClosure();
		return;
	}
	if (length > fMaxSize) {
		fFrameSize = fMaxSize;
		fNumTruncatedBytes = length - fMaxSize;
	} else {
		fFrameSize = length;
	}
	

	gettimeofday(&fPresentationTime, NULL); // If you have a more accurate time - e.g., from an encoder - then use that instead.
	// If the device is *not* a 'live source' (e.g., it comes instead from a file or buffer), then set "fDurationInMicroseconds" here.
	memmove(fTo, buffer_fo, fFrameSize);

	FramedSource::afterGetting(this);
}


