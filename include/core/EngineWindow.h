#pragma once

#include <QMainWindow>

namespace Zero {

	// TODO: Finish Design and Implementation
	class EngineWindow : public QMainWindow {
		Q_OBJECT

	public:
		EngineWindow() = default;
		~EngineWindow() override = default;

	protected:
		void Update();
		void InitializeWidgets();
		void InitializeHotkeys();
		bool ConfirmClose();

	}; // class EngineWindow

} // namespace Zero