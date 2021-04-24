#pragma once

#include <QLineEdit>
#include <QListWidget>
#include <QStringList>
#include <QWidget>

#include <memory>
#include <vector>

namespace pagoda::objects
{
class OperationFactory;
using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace pagoda::objects

namespace pgeditor::gui::graph
{
class NewNodePopup : public QWidget
{
	Q_OBJECT
	public:
	NewNodePopup(pagoda::objects::OperationFactoryPtr operationFactory);
	~NewNodePopup() override;

	signals:
	void OperationSelected(const QString& operationName);

	private:
	bool eventFilter(QObject* watched, QEvent* event) override;

	void InitializeGUI();

	void updateOperationList();
	void setSelectedItem();
	void selectUp();
	void selectDown();

	pagoda::objects::OperationFactoryPtr m_operationFactory;
	QStringList m_availableOperations;

	QLineEdit* m_searchField;
	QListWidget* m_operationList;
};
}  // namespace pgeditor::gui::graph
