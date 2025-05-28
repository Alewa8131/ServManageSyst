#pragma once

namespace ServManageSyst {

	using namespace System;
	using namespace System::Windows::Forms;

	public ref class ModeratorMenu : public Form {
	public:
		ModeratorMenu() {
			InitializeComponent();
		}

	protected:
		~ModeratorMenu() {
			if (components) delete components;
		}

	private:
		Label^ label;
		System::ComponentModel::Container^ components;

		void InitializeComponent(void) {
			this->label = gcnew Label();
			this->SuspendLayout();
			this->label->Text = L"Moderator reports window";
			this->label->Location = System::Drawing::Point(80, 80);
			this->label->AutoSize = true;
			this->Controls->Add(this->label);
			this->Text = L"Moderator Menu";
			this->ClientSize = System::Drawing::Size(300, 250);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
	};
}