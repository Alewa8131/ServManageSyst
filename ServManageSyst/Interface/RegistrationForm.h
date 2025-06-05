#pragma once

#include <string>
#include "../GameEntitiesLib/Player.h"

namespace ServManageSyst {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::IO;
    using namespace msclr::interop;

    public ref class RegistrationForm : public Form {
    public:
        RegistrationForm() {
            InitializeComponent();
        }

    private:
        TextBox^ textBoxLogin;
        TextBox^ textBoxPassword;
        Button^ buttonSubmit;

        void InitializeComponent() {
            this->Text = "Register";
            this->ClientSize = System::Drawing::Size(400, 220);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->StartPosition = FormStartPosition::Manual;
            this->Location = Drawing::Point(550, 300);

            Label^ labelLogin = gcnew Label();
            labelLogin->Text = "Login:";
            labelLogin->Location = Point(50, 50);
            labelLogin->Size = Drawing::Size(60, 20);

            textBoxLogin = gcnew TextBox();
            textBoxLogin->Location = Drawing::Point(120, 50);
            textBoxLogin->Size = Drawing::Size(200, 22);

            Label^ labelPassword = gcnew Label();
            labelPassword->Text = "Password:";
            labelPassword->Location = Point(50, 100);
            labelPassword->Size = Drawing::Size(60, 20);

            textBoxPassword = gcnew TextBox();
            textBoxPassword->Location = Drawing::Point(120, 100);
            textBoxPassword->Size = Drawing::Size(200, 22);
            textBoxPassword->UseSystemPasswordChar = true;

            buttonSubmit = gcnew Button();
            buttonSubmit->Location = Drawing::Point(150, 150);
            buttonSubmit->Size = Drawing::Size(100, 30);
            buttonSubmit->Text = "Register";
            buttonSubmit->Click += gcnew EventHandler(this, &RegistrationForm::buttonSubmit_Click);

            Controls->Add(labelLogin);
            Controls->Add(labelPassword);
            Controls->Add(textBoxLogin);
            Controls->Add(textBoxPassword);
            Controls->Add(buttonSubmit);
        }


        void buttonSubmit_Click(Object^ sender, EventArgs^ e) {
            String^ login = textBoxLogin->Text->Trim();
            String^ password = textBoxPassword->Text->Trim();

            if (login->Length == 0 || password->Length == 0) {
                MessageBox::Show("Please enter both login and password.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            std::string loginStd = marshal_as<std::string>(login);
            std::string passwordStd = marshal_as<std::string>(password);

            try {
                // users.csv
                User new_user(loginStd, passwordStd, UserRole::Player);
                User::add_user(new_user, USER_DB_PATH);

                // players.csv
                Player new_player;
                new_player.set_id(new_user.get_id());
                new_player.set_username(loginStd);
                new_player.set_password(passwordStd);

                std::ofstream playerfile(PLAYER_DB_PATH, std::ios::app);
                if (playerfile.is_open()) {
                    playerfile << new_player.to_csv_line() << "\n";
                    playerfile.close();
                }
                MessageBox::Show("Account successfully created!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
                this->Close();
            }
            catch (const std::exception& ex) {
                MessageBox::Show(gcnew String(ex.what()), "Registration Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    };
}
