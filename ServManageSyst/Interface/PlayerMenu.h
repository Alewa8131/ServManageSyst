// Copyright 2025 Alewa8131
// Player can buy privilege, find other players
#pragma once

#include "../GameEntitiesLib/Player.h"

namespace ServManageSyst {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace msclr::interop;

    public ref class PlayerMenu : public Form {
    private:
        String^ _login;
        TableLayoutPanel^ infoLayout;
        Panel^ privilegePanel;
        FlowLayoutPanel^ privilegeList;

    public:
        PlayerMenu(String^ login) {
            _login = login;
            InitializeComponent();
            LoadPlayerInfo();
        }

    protected:
        ~PlayerMenu() {
            if (components) delete components;
        }

    private:
        Label^ labelInfo;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void) {
            this->infoLayout = gcnew TableLayoutPanel();
            this->SuspendLayout();

            // Table layout for player info
            this->infoLayout->ColumnCount = 2;
            this->infoLayout->RowCount = 0;
            this->infoLayout->AutoSize = true;
            this->infoLayout->Location = Point(20, 0);
            this->infoLayout->Padding = System::Windows::Forms::Padding(10);
            this->infoLayout->BackColor = Color::FromArgb(255, 255, 255); // white

            // Scroll panel
            this->privilegePanel = gcnew Panel();
            this->privilegePanel->AutoScroll = true;
            this->privilegePanel->Location = Point(20, 220);
            this->privilegePanel->Size = System::Drawing::Size(500, 200);
            this->privilegePanel->BorderStyle = BorderStyle::FixedSingle;
            this->privilegePanel->BackColor = Color::FromArgb(240, 248, 255); // light blueish

            // Container for privileges
            this->privilegeList = gcnew FlowLayoutPanel();
            this->privilegeList->AutoSize = true;
            this->privilegeList->FlowDirection = FlowDirection::TopDown;
            this->privilegeList->WrapContents = false;
            this->privilegeList->Padding = System::Windows::Forms::Padding(10);
            this->privilegeList->BackColor = Color::FromArgb(250, 250, 250); // slightly off-white

            this->privilegePanel->Controls->Add(this->privilegeList);
            this->Controls->Add(this->privilegePanel);

            this->infoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::AutoSize));
            this->infoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));

            this->Controls->Add(this->infoLayout);
            this->AutoSize = true;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Text = L"Player Menu";
            this->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            this->BackColor = Color::FromArgb(220, 235, 250); // form background

            this->ResumeLayout(false);
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
            lbl->BackColor = Color::FromArgb(230, 240, 255); // light blue block
            lbl->Padding = System::Windows::Forms::Padding(5);
            lbl->Margin = Windows::Forms::Padding(5);
            lbl->ForeColor = Color::FromArgb(30, 30, 30); // dark text

            this->privilegeList->Controls->Add(lbl);
        }


        void LoadPlayerInfo() {
            String^ name1 = _login;
            std::string login = marshal_as<std::string>(name1);
            TVector<Player> players = Player::load_all();

            for (int i = 0; i < players.size(); ++i) {
                Player p = players[i];
                if (p.get_username() == login) {
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
                    }
                    else {
                        for (int i = 0; i < history.size(); ++i) {
                            Privilege* pr = history[i];
                            AddPrivilegeHistoryRow(pr->get_name(), pr->get_price(), pr->get_purchase_date());
                        }
                    }
                }
            }
        }
    };
}