#ifndef SELECTOR_PROCEDURAL_GRAPH_SCHEDULER_H_
#define SELECTOR_PROCEDURAL_GRAPH_SCHEDULER_H_

namespace selector
{
class IScheduler
{
public:
    virtual ~IScheduler() = default;

	virtual void Initialize() = 0;
	virtual bool Step() = 0;
	virtual void Finalize() = 0;
};  // class IScheduler
}  // namespace selector
#endif
