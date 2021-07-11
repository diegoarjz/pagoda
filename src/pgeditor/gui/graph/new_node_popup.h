#pragma once

#include <QLineEdit>
#include <QListWidget>
#include <QStringList>
#include <QWidget>

#include <memory>
#include <unordered_map>
#include <vector>

namespace pagoda::objects
{
class OperationFactory;
using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace pagoda::objects

namespace pgeditor::gui::graph
{
class GraphNode;

class NewNodePopup : public QWidget
{
	Q_OBJECT
	public:
	NewNodePopup(pagoda::objects::OperationFactoryPtr operationFactory);
	~NewNodePopup() override;

	signals:
	void OperationSelected(const QString& operationName);
	void NodeCreated(GraphNode* n);

	private:
	bool eventFilter(QObject* watched, QEvent* event) override;

	void InitializeGUI();

	void updateOperationList();
	void setSelectedItem();
	void selectUp();
	void selectDown();

	pagoda::objects::OperationFactoryPtr m_operationFactory;
	QStringList m_availableOperations;
	std::unordered_map<QString, std::function<GraphNode*(void)>> m_factoryFunctions;

	QLineEdit* m_searchField;
	QListWidget* m_operationList;
};
}  // namespace pgeditor::gui::graph
