// Copyright 2025 Alewa8131
#pragma once

#include "../GameEntitiesLib/Server.h"
#include "../GameEntitiesLib/Event.h"

namespace ServManageSyst {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace msclr::interop;

    public ref class ServerMenu : public Form {
    private:
        String^ _serverName;
        TableLayoutPanel^ serverInfoLayout;
        Panel^ eventPanel;
        FlowLayoutPanel^ eventList;

    public:
        ServerMenu(String^ serverName) {
            _serverName = serverName;
            InitializeComponent();
            LoadServerInfo();
        }

    protected:
        ~ServerMenu() {
            if (components) delete components;
        }

    private:
        Label^ labelInfo;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void) {
            this->serverInfoLayout = gcnew TableLayoutPanel();
            this->SuspendLayout();

            // Table layout for server info
            this->serverInfoLayout->ColumnCount = 2;
            this->serverInfoLayout->RowCount = 0;
            this->serverInfoLayout->AutoSize = true;
            this->serverInfoLayout->Location = Point(20, 0);
            this->serverInfoLayout->Padding = System::Windows::Forms::Padding(10);
            this->serverInfoLayout->BackColor = Color::FromArgb(255, 255, 255); // white

            // Scroll panel for events
            this->eventPanel = gcnew Panel();
            this->eventPanel->AutoScroll = true;
            this->eventPanel->Location = Point(20, 220);
            this->eventPanel->Size = System::Drawing::Size(500, 200);
            this->eventPanel->BorderStyle = BorderStyle::FixedSingle;
            this->eventPanel->BackColor = Color::FromArgb(240, 248, 255); // light blueish

            // Container for events
            this->eventList = gcnew FlowLayoutPanel();
            this->eventList->AutoSize = true;
            this->eventList->FlowDirection = FlowDirection::TopDown;
            this->eventList->WrapContents = false;
            this->eventList->Padding = System::Windows::Forms::Padding(10);
            this->eventList->BackColor = Color::FromArgb(250, 250, 250); // slightly off-white

            this->eventPanel->Controls->Add(this->eventList);
            this->Controls->Add(this->eventPanel);

            this->serverInfoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::AutoSize));
            this->serverInfoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));

            this->Controls->Add(this->serverInfoLayout);
            this->AutoSize = true;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Text = L"Server Menu";
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

            this->serverInfoLayout->Controls->Add(label);
            this->serverInfoLayout->Controls->Add(value);
            this->serverInfoLayout->RowCount += 1;
        }

        void AddEventRow(int id, String^ name, const Core::DateTime& startDate) {
            String^ label = String::Format("ID: {0} - {1} - Start: {2}", id, name, gcnew String(startDate.to_string().c_str()));

            Label^ lbl = gcnew Label();
            lbl->Text = label;
            lbl->AutoSize = true;
            lbl->BackColor = Color::FromArgb(230, 240, 255); // light blue block
            lbl->Padding = System::Windows::Forms::Padding(5);
            lbl->Margin = Windows::Forms::Padding(5);
            lbl->ForeColor = Color::FromArgb(30, 30, 30); // dark text

            this->eventList->Controls->Add(lbl);
        }

        void LoadServerInfo() {
            String^ name1 = _serverName;
            std::string serverName = marshal_as<std::string>(name1);
            TVector<Server> servers = Server::load_all();

            for (int i = 0; i < servers.size(); ++i) {
                Server s = servers[i];
                if (s.get_name() == serverName) {
                    AddInfoRow("Server Name:", gcnew String(s.get_name().c_str()));
                    AddInfoRow("IP Address:", gcnew String(s.get_ip().c_str()));
                    AddInfoRow("Rent Cost:", s.get_rent_cost().ToString());
                    AddInfoRow("Uptime:", s.get_uptime().ToString());

                    // Event List
                    Label^ eventTitle = gcnew Label();
                    eventTitle->Text = "Events:";
                    eventTitle->Font = gcnew System::Drawing::Font("Segoe UI Semibold", 10, FontStyle::Bold);
                    eventTitle->AutoSize = true;
                    this->eventList->Controls->Add(eventTitle);

                    TVector<Event*> events = s.get_events();
                    if (events.is_empty()) {
                        Label^ noEvent = gcnew Label();
                        noEvent->Text = "No events available";
                        noEvent->BackColor = Color::FromArgb(230, 240, 255);
                        noEvent->AutoSize = true;
                        this->eventList->Controls->Add(noEvent);
                    }
                    else {
                        for (int i = 0; i < events.size(); ++i) {
                            Event* e = events[i];
                            AddEventRow(e->get_id(), gcnew String(e->get_name().c_str()), e->get_start_date());
                        }
                    }
                }
            }
        }
    };
}
