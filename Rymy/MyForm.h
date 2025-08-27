// MyForm.h
#pragma once
#include "PolishRhymeTool.h"
#include <msclr/marshal_cppstd.h>
#include <sstream>
#include <map> // We'll use a map to group rhyming words


namespace Rymy {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent(); 
			polishTool = new PolishRhymeTool();
			
			rhymeColors = gcnew List<Color>();
			rhymeColors->Add(Color::SteelBlue);

		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			delete polishTool;
		}

	private:
		PolishRhymeTool* polishTool; 

		// --- Designer-managed components ---
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::RichTextBox^ rtbLyricsInput;
	private: System::Windows::Forms::TextBox^ txtSyllableCounts;
	private: System::Windows::Forms::Timer^ analysisTimer;
	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->rtbLyricsInput = (gcnew System::Windows::Forms::RichTextBox());
			   this->txtSyllableCounts = (gcnew System::Windows::Forms::TextBox());
			   this->analysisTimer = (gcnew System::Windows::Forms::Timer(this->components));
			   this->SuspendLayout();
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label1->Location = System::Drawing::Point(12, 9);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(189, 17);
			   this->label1->TabIndex = 0;
			   this->label1->Text = L"Wpisz tekst (analiza na ¿ywo)";
			   // 
			   // rtbLyricsInput
			   // 
			   this->rtbLyricsInput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				   | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->rtbLyricsInput->Font = (gcnew System::Drawing::Font(L"Consolas", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->rtbLyricsInput->Location = System::Drawing::Point(12, 38);
			   this->rtbLyricsInput->Name = L"rtbLyricsInput";
			   this->rtbLyricsInput->Size = System::Drawing::Size(441, 311);
			   this->rtbLyricsInput->TabIndex = 1;
			   this->rtbLyricsInput->Text = L"";
			   this->rtbLyricsInput->TextChanged += gcnew System::EventHandler(this, &MyForm::rtbLyricsInput_TextChanged);
			   // 
			   // txtSyllableCounts
			   // 
			   this->txtSyllableCounts->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->txtSyllableCounts->BackColor = System::Drawing::SystemColors::ControlLight;
			   this->txtSyllableCounts->Font = (gcnew System::Drawing::Font(L"Consolas", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->txtSyllableCounts->Location = System::Drawing::Point(459, 38);
			   this->txtSyllableCounts->Multiline = true;
			   this->txtSyllableCounts->Name = L"txtSyllableCounts";
			   this->txtSyllableCounts->ReadOnly = true;
			   this->txtSyllableCounts->Size = System::Drawing::Size(53, 311);
			   this->txtSyllableCounts->TabIndex = 2;
			   this->txtSyllableCounts->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			   this->txtSyllableCounts->WordWrap = false;
			   // 
			   // analysisTimer
			   // 
			   this->analysisTimer->Interval = 400;
			   this->analysisTimer->Tick += gcnew System::EventHandler(this, &MyForm::analysisTimer_Tick);
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(524, 361);
			   this->Controls->Add(this->txtSyllableCounts);
			   this->Controls->Add(this->rtbLyricsInput);
			   this->Controls->Add(this->label1);
			   this->MinimumSize = System::Drawing::Size(400, 300);
			   this->Name = L"MyForm";
			   this->Text = L"Licznik Sylab";
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

		   // NEW: This event fires every time you type a character in the RichTextBox
	private: System::Void rtbLyricsInput_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		// Stop the timer if it's already running and start it again.
		// This resets the countdown, so the analysis only happens when you pause typing.
		analysisTimer->Stop();
		analysisTimer->Start();
	}

		   // NEW: This event fires when the timer's interval has passed without being reset.
	private: System::Void analysisTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
		// First, stop the timer so it doesn't fire again until the text changes.
		analysisTimer->Stop();

		// This is the SAME analysis logic that used to be in the button click event.
		System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
		msclr::interop::marshal_context context;

		for each(String ^ managedLine in rtbLyricsInput->Lines)
		{
			std::wstring stdLine = context.marshal_as<std::wstring>(managedLine);

			if (stdLine.empty()) {
				sb->AppendLine("");
				continue;
			}

			std::wstringstream wss(stdLine);
			std::wstring word;
			int lineTotalSyllables = 0;

			while (wss >> word) {
				lineTotalSyllables += polishTool->countSyllables(word);
			}

			sb->AppendLine(lineTotalSyllables.ToString());
		}

		// Update the counts. We check if the RichTextBox still has focus to prevent
		// a rare issue where the update could happen at an awkward time.
		if (this->rtbLyricsInput->Focused) {
			txtSyllableCounts->Text = sb->ToString();
		}
	}
	};
}