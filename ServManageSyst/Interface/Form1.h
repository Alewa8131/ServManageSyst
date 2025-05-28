// Copyright 2025 Alewa8131
#pragma once

#include "ModeratorMenu.h"
#include "PlayerMenu.h"

namespace ServManageSyst {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::IO;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void) {
            InitializeComponent();
        }

    protected:
        ~Form1() {
            if (components) {
                delete components;
            }
        }

    private: System::Windows::Forms::TextBox^ textBoxLogin;
    private: System::Windows::Forms::TextBox^ textBoxPassword;
    private: System::Windows::Forms::Button^ buttonLogin;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Label^ label2;
    private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
           void InitializeComponent(void) {
               this->textBoxLogin = (gcnew System::Windows::Forms::TextBox());
               this->textBoxPassword = (gcnew System::Windows::Forms::TextBox());
               this->buttonLogin = (gcnew System::Windows::Forms::Button());
               this->label1 = (gcnew System::Windows::Forms::Label());
               this->label2 = (gcnew System::Windows::Forms::Label());
               this->SuspendLayout();

               this->textBoxLogin->Location = System::Drawing::Point(120, 50);
               this->textBoxLogin->Size = System::Drawing::Size(200, 22);

               this->textBoxPassword->Location = System::Drawing::Point(120, 100);
               this->textBoxPassword->PasswordChar = '*';
               this->textBoxPassword->Size = System::Drawing::Size(200, 22);

               this->buttonLogin->Location = System::Drawing::Point(150, 150);
               this->buttonLogin->Size = System::Drawing::Size(100, 30);
               this->buttonLogin->Text = L"Login";
               this->buttonLogin->Click += gcnew System::EventHandler(this,
                   &Form1::buttonLogin_Click);

               this->label1->Location = System::Drawing::Point(50, 50);
               this->label1->Size = System::Drawing::Size(60, 20);
               this->label1->Text = L"Login:";

               this->label2->Location = System::Drawing::Point(50, 100);
               this->label2->Size = System::Drawing::Size(60, 20);
               this->label2->Text = L"Password:";

               this->ClientSize = System::Drawing::Size(400, 220);
               this->Controls->Add(this->textBoxLogin);
               this->Controls->Add(this->textBoxPassword);
               this->Controls->Add(this->buttonLogin);
               this->Controls->Add(this->label1);
               this->Controls->Add(this->label2);
               this->Name = L"Form1";
               this->Text = L"Login";
               this->ResumeLayout(false);
               this->PerformLayout();
           }
#pragma endregion

    private: System::Void buttonLogin_Click(System::Object^ sender,
        System::EventArgs^ e) {

        String^ login = textBoxLogin->Text;
        String^ password = textBoxPassword->Text;
        array<String^>^ lines = File::ReadAllLines("../CoreLib/db/users.csv");

        for each (String ^ line in lines) {
            array<String^>^ parts = line->Split(',');
            if (parts->Length >= 4 && parts[1] == login && parts[2] == password) {
                String^ role = parts[3];
                this->Hide();
                if (role == "Player") {
                    PlayerMenu^ p = gcnew PlayerMenu();
                    p->ShowDialog();
                } else if (role == "Moderator") {
                    ModeratorMenu^ m = gcnew ModeratorMenu();
                    m->ShowDialog();
                }
                this->Close();
                return;
            }
        }
        MessageBox::Show("Invalid login or password.");
    }
    };
}  // namespace ServManageSyst
