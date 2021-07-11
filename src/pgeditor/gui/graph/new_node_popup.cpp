#include "new_node_popup.h"

#include <pagoda/common/debug/assertions.h>

#include <pagoda/objects/operation_factory.h>

#include <QEvent>
#include <QKeyEvent>
#include <QVBoxLayout>

#include <iostream>

using namespace pagoda::objects;

namespace pgeditor::gui::graph
{
NewNodePopup::NewNodePopup(OperationFactoryPtr operationFactory) : QWidget(), m_operationFactory{operationFactory}
{
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	auto operations = m_operationFactory->RegisteredTypes();
	for (const auto& o : operations) {
		m_availableOperations.push_back(QString(o.c_str()));
		m_factoryFunctions.emplace(QString(o.c_str()), [this, &o]() {
			auto operation = m_operationFactory->Create(o);
			return nullptr;
		});
	}

	InitializeGUI();
}

NewNodePopup::~NewNodePopup() {}

void NewNodePopup::InitializeGUI()
{
	setGeometry(300, 100, 300, 100);
	setMaximumSize(300, 400);

	auto layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	m_searchField = new QLineEdit();
	m_searchField->setContentsMargins(-1, -1, -1, 0);
	m_searchField->installEventFilter(this);
	connect(m_searchField, &QLineEdit::textChanged, [this]() { updateOperationList(); });

	m_operationList = new QListWidget();
	m_operationList->setContentsMargins(-1, -1, -1, 0);
	m_operationList->addItems(m_availableOperations);
	m_operationList->installEventFilter(this);

	layout->addWidget(m_searchField);
	layout->addWidget(m_operationList);

	setLayout(layout);

	installEventFilter(this);
}

bool NewNodePopup::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::KeyPress) {
		auto key = static_cast<QKeyEvent*>(event)->key();
		switch (key) {
			case Qt::Key_Escape:
				close();
				return true;
			case Qt::Key_Return:
				setSelectedItem();
				close();
				return true;
			case Qt::Key_Up:
				selectUp();
				return true;
			case Qt::Key_Down:
				selectDown();
				return true;
		};
	}
	return QWidget::eventFilter(watched, event);
}

void NewNodePopup::updateOperationList()
{
	while (m_operationList->count() > 0) {
		m_operationList->takeItem(0);
	}

	if (m_searchField->text() == "") {
		m_operationList->addItems(m_availableOperations);
	} else {
		m_operationList->addItems(m_availableOperations.filter(m_searchField->text(), Qt::CaseInsensitive));
	}
}

void NewNodePopup::setSelectedItem()
{
	emit OperationSelected(m_operationList->currentItem()->text());

	auto name = m_operationList->currentItem()->text();
	DBG_ASSERT(m_factoryFunctions.find(name) != m_factoryFunctions.end());
	emit NodeCreated(m_factoryFunctions[name]());
}

void NewNodePopup::selectUp()
{
	auto selected = m_operationList->currentRow();
	m_operationList->setCurrentRow(--selected);
}

void NewNodePopup::selectDown()
{
	auto selected = m_operationList->currentRow();
	m_operationList->setCurrentRow(++selected);
}

}  // namespace pgeditor::gui::graph
