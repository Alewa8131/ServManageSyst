// Copyright 2025 Alewa8131
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

            this->infoLayout->ColumnCount = 2;
            this->infoLayout->RowCount = 0;
            this->infoLayout->AutoSize = true;
            this->infoLayout->Location = Point(20, 0);
            this->infoLayout->Padding = System::Windows::Forms::Padding(10);
            this->infoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::AutoSize));
            this->infoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));

            this->Controls->Add(this->infoLayout);
            this->AutoSize = true;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Text = L"Player Menu";
            this->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            this->ResumeLayout(false);
        }

        void AddInfoRow(String^ labelText, String^ valueText) {
            Label^ label = gcnew Label();
            label->Text = labelText;
            label->AutoSize = true;
            label->Font = gcnew System::Drawing::Font("Segoe UI Semibold", 10, FontStyle::Bold);

            Label^ value = gcnew Label();
            value->Text = valueText;
            value->AutoSize = true;

            this->infoLayout->Controls->Add(label);
            this->infoLayout->Controls->Add(value);
            this->infoLayout->RowCount += 1;
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
                    break;
                }
            }
        }
    };
}