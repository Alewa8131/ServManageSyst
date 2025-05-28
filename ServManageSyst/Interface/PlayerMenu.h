// Copyright 2025 Alewa8131
#pragma once

namespace ServManageSyst {

    using namespace System;
    using namespace System::Windows::Forms;

    public ref class PlayerMenu : public Form {
    public:
        PlayerMenu() {
            InitializeComponent();
        }

    protected:
        ~PlayerMenu() {
            if (components) delete components;
        }

    private:
        Label^ label;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void) {
            this->label = gcnew Label();
            this->SuspendLayout();
            this->label->Text = L"Player statistics window";
            this->label->Location = System::Drawing::Point(100, 100);
            this->label->AutoSize = true;
            this->Controls->Add(this->label);
            this->Text = L"Player Menu";
            this->ClientSize = System::Drawing::Size(300, 250);
            this->ResumeLayout(false);
            this->PerformLayout();
        }
    };
}  // namespace ServManageSyst
