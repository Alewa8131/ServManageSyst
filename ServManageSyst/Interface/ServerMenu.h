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
        Panel^ mainPanel;
        TableLayoutPanel^ serverInfoLayout;
        Panel^ eventPanel;
        FlowLayoutPanel^ eventList;
        Panel^ buttonsPanel;

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
        System::ComponentModel::Container^ components;

        void InitializeComponent(void) {
            this->SuspendLayout();

            // Main container panel
            this->mainPanel = gcnew Panel();
            this->mainPanel->Dock = DockStyle::Fill;
            this->mainPanel->Padding = System::Windows::Forms::Padding(20);
            this->mainPanel->BackColor = Color::FromArgb(220, 235, 250); // идентичный PlayerMenu

            // Server info table layout
            this->serverInfoLayout = gcnew TableLayoutPanel();
            this->serverInfoLayout->Dock = DockStyle::Top;
            this->serverInfoLayout->AutoSize = true;
            this->serverInfoLayout->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->serverInfoLayout->ColumnCount = 2;
            this->serverInfoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::AutoSize));
            this->serverInfoLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));
            this->serverInfoLayout->Margin = System::Windows::Forms::Padding(0, 0, 0, 20);
            this->serverInfoLayout->Padding = System::Windows::Forms::Padding(15);
            this->serverInfoLayout->BackColor = Color::White;
            this->serverInfoLayout->BorderStyle = BorderStyle::FixedSingle;

            // Events scroll panel (исправлено для прокрутки)
            this->eventPanel = gcnew Panel();
            this->eventPanel->Dock = DockStyle::Fill;
            this->eventPanel->AutoScroll = true;
            this->eventPanel->BorderStyle = BorderStyle::FixedSingle;
            this->eventPanel->BackColor = Color::FromArgb(240, 248, 255);

            // Events list container
            this->eventList = gcnew FlowLayoutPanel();
            this->eventList->FlowDirection = FlowDirection::TopDown;
            this->eventList->WrapContents = false;
            this->eventList->AutoSize = true;
            this->eventList->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->eventList->Padding = System::Windows::Forms::Padding(15);
            this->eventList->BackColor = Color::FromArgb(250, 250, 250);
            this->eventList->Dock = DockStyle::Top;

            this->eventPanel->Controls->Add(this->eventList);

            // Buttons panel (как в PlayerMenu)
            this->buttonsPanel = gcnew Panel();
            this->buttonsPanel->Dock = DockStyle::Bottom;
            this->buttonsPanel->Height = 60;
            this->buttonsPanel->Padding = System::Windows::Forms::Padding(20, 10, 20, 10);
            this->buttonsPanel->BackColor = Color::Transparent;

            // Add controls to main panel
            Panel^ contentPanel = gcnew Panel();
            contentPanel->Dock = DockStyle::Fill;
            contentPanel->Controls->Add(this->eventPanel);
            contentPanel->Controls->Add(this->serverInfoLayout);

            this->mainPanel->Controls->Add(contentPanel);
            this->mainPanel->Controls->Add(this->buttonsPanel);

            // Form settings
            this->Controls->Add(this->mainPanel);
            this->MinimumSize = System::Drawing::Size(600, 500); // как в PlayerMenu
            this->Text = L"Server Menu - " + _serverName;
            this->Font = gcnew System::Drawing::Font("Segoe UI", 10); // как в PlayerMenu
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
            this->BackColor = Color::FromArgb(220, 235, 250); // как в PlayerMenu

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void AddInfoRow(String^ labelText, String^ valueText) {
            Label^ label = gcnew Label();
            label->Text = labelText;
            label->AutoSize = true;
            label->Font = gcnew System::Drawing::Font("Segoe UI Semibold", 10, FontStyle::Bold);
            label->ForeColor = Color::FromArgb(30, 60, 120); // как в PlayerMenu
            label->Margin = System::Windows::Forms::Padding(0, 5, 10, 5);

            Label^ value = gcnew Label();
            value->Text = valueText;
            value->AutoSize = true;
            value->Font = gcnew System::Drawing::Font("Segoe UI", 10);
            value->ForeColor = Color::FromArgb(50, 50, 50); // как в PlayerMenu
            value->Margin = System::Windows::Forms::Padding(0, 5, 0, 5);

            this->serverInfoLayout->Controls->Add(label);
            this->serverInfoLayout->Controls->Add(value);
            this->serverInfoLayout->RowCount += 1;
            this->serverInfoLayout->RowStyles->Add(gcnew RowStyle(SizeType::AutoSize));
        }

        void AddEventRow(int id, String^ name, const Core::DateTime& startDate) {
            Panel^ eventItem = gcnew Panel();
            eventItem->Width = this->eventPanel->ClientSize.Width - 40; // учитываем padding
            eventItem->Height = 40;
            eventItem->BackColor = Color::FromArgb(230, 240, 255); // как в PlayerMenu
            eventItem->Padding = System::Windows::Forms::Padding(10);
            eventItem->Margin = System::Windows::Forms::Padding(0, 0, 0, 5);
            eventItem->BorderStyle = BorderStyle::FixedSingle;

            Label^ eventLabel = gcnew Label();
            eventLabel->Text = String::Format("ID: {0} - {1} - Start: {2}",
                id, name, gcnew String(startDate.to_string().c_str()));
            eventLabel->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            eventLabel->ForeColor = Color::FromArgb(30, 30, 30); // как в PlayerMenu
            eventLabel->Location = Point(10, 10);
            eventLabel->AutoSize = true;

            eventItem->Controls->Add(eventLabel);
            this->eventList->Controls->Add(eventItem);
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
                    AddInfoRow("Uptime:", String::Format("{0} hours", s.get_uptime()));

                    // Events title
                    Label^ eventsTitle = gcnew Label();
                    eventsTitle->Text = "SCHEDULED EVENTS:";
                    eventsTitle->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
                    eventsTitle->AutoSize = true;
                    eventsTitle->ForeColor = Color::FromArgb(70, 130, 180); // как в PlayerMenu
                    eventsTitle->Margin = System::Windows::Forms::Padding(0, 10, 0, 10);
                    this->eventList->Controls->Add(eventsTitle);

                    TVector<Event*> events = s.get_events();
                    if (events.is_empty()) {
                        Label^ noEvents = gcnew Label();
                        noEvents->Text = "No scheduled events";
                        noEvents->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Italic);
                        noEvents->ForeColor = Color::FromArgb(100, 100, 100);
                        noEvents->AutoSize = true;
                        this->eventList->Controls->Add(noEvents);
                    }
                    else {
                        for (int i = 0; i < events.size(); ++i) {
                            Event* e = events[i];
                            AddEventRow(e->get_id(), gcnew String(e->get_name().c_str()), e->get_start_date());
                        }
                    }
                    break;
                }
            }
        }
    };
}