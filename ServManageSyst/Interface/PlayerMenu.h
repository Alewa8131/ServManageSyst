// Copyright 2025 Alewa8131
// Player can buy privilege, find other players, change password
#pragma once

#include "ServerMenu.h"

#include "../GameEntitiesLib/Player.h"

namespace ServManageSyst {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace msclr::interop;

    public ref class PlayerMenu : public Form {
    private:
        Player* _player;
        String^ _login;
        Panel^ mainPanel;
        TableLayoutPanel^ infoLayout;
        Panel^ privilegePanel;
        FlowLayoutPanel^ privilegeList;
        Panel^ buttonsPanel;
        Button^ viewServersButton;
        Button^ changePasswordButton;

    public:
        PlayerMenu(String^ login) {
            _login = login;
            FindAndStorePlayer();
            InitializeComponent();
            LoadPlayerInfo();
        }

    protected:
        ~PlayerMenu() {
            if (components) delete components;
            if (_player != nullptr) {
                delete _player;
                _player = nullptr;
            }
        }


    private:
        Label^ labelInfo;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->SuspendLayout();

            // Main container panel
            this->mainPanel = gcnew Panel();
            this->mainPanel->Dock = DockStyle::Fill;
            this->mainPanel->Padding = System::Windows::Forms::Padding(20);
            this->mainPanel->BackColor = Color::FromArgb(240, 245, 250);

            // Player info table layout (similar to serverInfoLayout)
            this->infoLayout = gcnew TableLayoutPanel();
            this->infoLayout->Dock = DockStyle::Top;
            this->infoLayout->AutoSize = true;
            this->infoLayout->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->infoLayout->ColumnCount = 2;
            this->infoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::AutoSize));
            this->infoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));
            this->infoLayout->Margin = System::Windows::Forms::Padding(0, 0, 0, 20);
            this->infoLayout->Padding = System::Windows::Forms::Padding(15);
            this->infoLayout->BackColor = Color::White;
            this->infoLayout->BorderStyle = BorderStyle::FixedSingle;

            // Privileges scroll panel (similar to eventPanel)
            this->privilegePanel = gcnew Panel();
            this->privilegePanel->Dock = DockStyle::Fill;
            this->privilegePanel->AutoScroll = true;
            this->privilegePanel->BorderStyle = BorderStyle::FixedSingle;
            this->privilegePanel->BackColor = Color::FromArgb(245, 250, 255);

            // Privileges list container (similar to eventList)
            this->privilegeList = gcnew FlowLayoutPanel();
            this->privilegeList->FlowDirection = FlowDirection::TopDown;
            this->privilegeList->WrapContents = false;
            this->privilegeList->AutoSize = true;
            this->privilegeList->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->privilegeList->Padding = System::Windows::Forms::Padding(15);
            this->privilegeList->BackColor = Color::FromArgb(250, 253, 255);
            this->privilegeList->Dock = DockStyle::Top;

            this->privilegePanel->Controls->Add(this->privilegeList);

            // Buttons panel (similar to ServerMenu)
            this->buttonsPanel = gcnew Panel();
            this->buttonsPanel->Dock = DockStyle::Bottom;
            this->buttonsPanel->Height = 80;
            this->buttonsPanel->Padding = System::Windows::Forms::Padding(20, 10, 20, 10);
            this->buttonsPanel->BackColor = Color::Transparent;

            // Button container (similar to ServerMenu structure)
            Panel^ buttonContainer = gcnew Panel();
            buttonContainer->Dock = DockStyle::Fill;
            buttonContainer->BackColor = Color::Transparent;

            // View Servers Button
            this->viewServersButton = gcnew Button();
            this->viewServersButton->Text = "View Servers";
            this->viewServersButton->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            this->viewServersButton->BackColor = Color::FromArgb(70, 130, 180);
            this->viewServersButton->ForeColor = Color::White;
            this->viewServersButton->FlatStyle = FlatStyle::Flat;
            this->viewServersButton->FlatAppearance->BorderSize = 0;
            this->viewServersButton->Size = Drawing::Size(160, 40);
            this->viewServersButton->Cursor = Cursors::Hand;
            this->viewServersButton->Click += gcnew EventHandler(this, &PlayerMenu::OnViewServersClick);
            this->viewServersButton->Location = Point(80, 10);
            buttonContainer->Controls->Add(this->viewServersButton);

            // Change Password Button
            this->changePasswordButton = gcnew Button();
            this->changePasswordButton->Text = "Change Password";
            this->changePasswordButton->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            this->changePasswordButton->BackColor = Color::FromArgb(100, 149, 237);
            this->changePasswordButton->ForeColor = Color::White;
            this->changePasswordButton->FlatStyle = FlatStyle::Flat;
            this->changePasswordButton->FlatAppearance->BorderSize = 0;
            this->changePasswordButton->Size = Drawing::Size(160, 40);
            this->changePasswordButton->Cursor = Cursors::Hand;
            this->changePasswordButton->Click += gcnew EventHandler(this, &PlayerMenu::OnChangePasswordClick);
            this->changePasswordButton->Location = Point(260, 10);
            buttonContainer->Controls->Add(this->changePasswordButton);

            this->buttonsPanel->Controls->Add(buttonContainer);

            Panel^ contentPanel = gcnew Panel();
            contentPanel->Dock = DockStyle::Fill;
            contentPanel->Controls->Add(this->privilegePanel);
            contentPanel->Controls->Add(this->infoLayout);

            this->mainPanel->Controls->Add(contentPanel);
            this->mainPanel->Controls->Add(this->buttonsPanel);

            this->Controls->Add(this->mainPanel);
            this->MinimumSize = Drawing::Size(650, 550);
            this->Text = L"Player Menu";
            this->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
            this->BackColor = Color::FromArgb(240, 245, 250);

            this->ResumeLayout(false);
            this->PerformLayout();
        }
        void OnChangePasswordClick(Object^ sender, EventArgs^ e) {
            String^ newPassword = Prompt("Enter new password:", "Change Password");
            if (String::IsNullOrWhiteSpace(newPassword)) return;

            String^ name1 = _login;
            std::string login = marshal_as<std::string>(name1);
            std::string newPass = marshal_as<std::string>(newPassword);

            try {
                if (!User::is_valid_input(newPass)) {
                    throw std::runtime_error("Password contains forbidden characters");
                }
                TVector<Player> players = Player::load_all_players();
                for (int i = 0; i < players.size(); ++i) {
                    if (players[i].get_username() == login) {
                        players[i].set_password(newPass);
                        break;
                    }
                }
                Player::save_all_objects(players, PLAYER_DB_PATH);
                TVector<User> users = User::load_all_users(USER_DB_PATH);
                for (int i = 0; i < players.size(); ++i) {
                    if (users[i].get_username() == login) {
                        users[i].set_password(newPass);
                        break;
                    }
                }
                User::save_all_objects(users, USER_DB_PATH);

                MessageBox::Show("Password successfully changed.");
            }
            catch (const std::exception& ex) {
                MessageBox::Show(gcnew String(ex.what()), "Registration Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
        String^ Prompt(String^ text, String^ caption) {
            Form^ prompt = gcnew Form();
            prompt->Width = 400;
            prompt->Height = 160;
            prompt->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            prompt->Text = caption;
            prompt->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            prompt->BackColor = Color::FromArgb(245, 250, 255);
            prompt->ForeColor = Color::Black;
            prompt->StartPosition = FormStartPosition::CenterScreen;

            Label^ textLabel = gcnew Label();
            textLabel->Left = 20;
            textLabel->Top = 20;
            textLabel->Text = text;
            textLabel->AutoSize = true;

            TextBox^ inputBox = gcnew TextBox();
            inputBox->Left = 20;
            inputBox->Top = 50;
            inputBox->Width = 340;
            inputBox->UseSystemPasswordChar = true;

            Button^ confirmation = gcnew Button();
            confirmation->Text = "OK";
            confirmation->Left = 280;
            confirmation->Width = 80;
            confirmation->Height = 30;
            confirmation->Top = 85;
            confirmation->DialogResult = System::Windows::Forms::DialogResult::OK;

            confirmation->BackColor = Color::FromArgb(70, 130, 180);
            confirmation->ForeColor = Color::White;

            prompt->Controls->Add(textLabel);
            prompt->Controls->Add(inputBox);
            prompt->Controls->Add(confirmation);
            prompt->AcceptButton = confirmation;

            return prompt->ShowDialog() == System::Windows::Forms::DialogResult::OK ? inputBox->Text : nullptr;
        }

        void OnFormResize(Object^ sender, EventArgs^ e) {
            this->viewServersButton->Left = (this->ClientSize.Width - this->viewServersButton->Width) / 2;
        }

        void FindAndStorePlayer() {
            String^ name1 = _login;
            std::string login = marshal_as<std::string>(name1);
            TVector<Player> players = Player::load_all_players();

            for (int i = 0; i < players.size(); ++i) {
                if (players[i].get_username() == login) {
                    _player = new Player(players[i]);
                    return;
                }
            }

            MessageBox::Show("Player not found.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }

        void OnViewServersClick(Object^ sender, EventArgs^ e) {
            Server* server = _player->get_server();
            if (server == nullptr) {
                MessageBox::Show("Server is nullptr");
                return;
            }

            std::string serverName = server->get_name();
            System::String^ managedServerName = gcnew System::String(serverName.c_str());

            ServerMenu^ serverMenu = gcnew ServerMenu(managedServerName);
            serverMenu->ShowDialog();
        }

        void AddInfoRow(String^ labelText, String^ valueText) {
            Label^ label = gcnew Label();
            label->Text = labelText;
            label->AutoSize = true;
            label->Font = gcnew System::Drawing::Font("Segoe UI Semibold", 10, FontStyle::Bold);
            label->ForeColor = Color::FromArgb(30, 60, 120); // dark blue

            Label^ value = gcnew Label();
            value->Text = valueText;
            value->AutoSize = true;
            value->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            value->ForeColor = Color::FromArgb(50, 50, 50); // dark gray

            this->infoLayout->Controls->Add(label);
            this->infoLayout->Controls->Add(value);
            this->infoLayout->RowCount += 1;
        }

        void AddPrivilegeHistoryRow(std::string name, double price, Core::DateTime date) {
            String^ label = gcnew String((name + " — " + std::to_string(price) + "$ — " + date.to_string()).c_str());

            Label^ lbl = gcnew Label();
            lbl->Text = label;
            lbl->AutoSize = true;
            lbl->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            lbl->BackColor = Color::FromArgb(230, 240, 255); // light blue block
            lbl->Padding = System::Windows::Forms::Padding(5);
            lbl->Margin = Windows::Forms::Padding(5);
            lbl->ForeColor = Color::FromArgb(30, 30, 30); // dark text

            this->privilegeList->Controls->Add(lbl);
        }


        void LoadPlayerInfo() {
            Player& p = *_player;
            AddInfoRow("Login:", gcnew String(p.get_username().c_str()));
            AddInfoRow("Status:", gcnew String(p.get_status().c_str()));
            AddInfoRow("Join Date:", gcnew String(p.get_join_date().to_string().c_str()));
            AddInfoRow("Minutes Played:", p.get_minutes_played().ToString());

            std::string moneyStr = std::to_string(p.get_money_spent());
            size_t dot = moneyStr.find('.');
            if (dot != std::string::npos && dot + 3 <= moneyStr.size())
                moneyStr = moneyStr.substr(0, dot + 3);
            AddInfoRow("Money Spent:", gcnew String(moneyStr.c_str()));

            if (p.get_privilege())
                AddInfoRow("Privilege:", gcnew String(p.get_privilege()->get_name().c_str()));
            else
                AddInfoRow("Privilege:", "None");

            if (p.get_server())
                AddInfoRow("Server:", gcnew String(p.get_server()->get_name().c_str()));
            else
                AddInfoRow("Server:", "None");

            Label^ historyTitle = gcnew Label();
            historyTitle->Text = "Privilege History:";
            historyTitle->Font = gcnew System::Drawing::Font("Segoe UI Semibold", 10, FontStyle::Bold);
            historyTitle->AutoSize = true;
            this->privilegeList->Controls->Add(historyTitle);

            TVector<Privilege*> history = p.get_privilege_history();
            if (history.is_empty()) {
                Label^ noHistory = gcnew Label();
                noHistory->Text = "No privilege history";
                noHistory->BackColor = Color::FromArgb(230, 240, 255);
                noHistory->AutoSize = true;
                this->privilegeList->Controls->Add(noHistory);
            } else {
                for (int i = 0; i < history.size(); ++i) {
                    Privilege* pr = history[i];
                    AddPrivilegeHistoryRow(pr->get_name(), pr->get_price(), pr->get_purchase_date());
                }
            }
        }
  };
}