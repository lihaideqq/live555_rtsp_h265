#ifndef H265DEVICESOURCE_HH_
#define H265DEVICESOURCE_HH_
#include<DeviceSource.hh>

class H265DeviceSource: public DeviceSource {
public:
	static DeviceSource* createNew(UsageEnvironment& env);
protected:
	H265DeviceSource(UsageEnvironment& env);
	virtual ~H265DeviceSource();
private:
	virtual void doGetNextFrame();
	virtual unsigned maxFrameSize() const;
	u_int8_t * fBuffer;
	unsigned int   fBufferSize;
};

#endif /* H265DEVICESOURCE_HH_ */


