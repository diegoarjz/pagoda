#pragma once

namespace pgeditor::common
{
class Manager
{
	public:
	Manager();
	virtual ~Manager();

	virtual bool Init() = 0;
	virtual void Update(double dT) = 0;
	virtual void Destroy() = 0;

	private:
};
}  // namespace pgeditor::common
