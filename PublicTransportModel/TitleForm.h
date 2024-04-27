#pragma once
#include "ModelForm.h"

namespace PublicTransportModel {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для TitleForm
	/// </summary>
	public ref class TitleForm : public System::Windows::Forms::Form
	{
	public:
		TitleForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~TitleForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ buttonStart;
	private: System::Windows::Forms::Button^ buttonExit;
	protected:


	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonStart = (gcnew System::Windows::Forms::Button());
			this->buttonExit = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// buttonStart
			// 
			this->buttonStart->Location = System::Drawing::Point(695, 499);
			this->buttonStart->Name = L"buttonStart";
			this->buttonStart->Size = System::Drawing::Size(218, 57);
			this->buttonStart->TabIndex = 0;
			this->buttonStart->Text = L"Начать";
			this->buttonStart->UseVisualStyleBackColor = true;
			this->buttonStart->Click += gcnew System::EventHandler(this, &TitleForm::buttonStart_Click);
			// 
			// buttonExit
			// 
			this->buttonExit->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->buttonExit->Location = System::Drawing::Point(695, 562);
			this->buttonExit->Name = L"buttonExit";
			this->buttonExit->Size = System::Drawing::Size(218, 48);
			this->buttonExit->TabIndex = 1;
			this->buttonExit->Text = L"Выйти";
			this->buttonExit->UseVisualStyleBackColor = true;
			this->buttonExit->Click += gcnew System::EventHandler(this, &TitleForm::buttonExit_Click);
			// 
			// TitleForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize = System::Drawing::Size(1584, 861);
			this->Controls->Add(this->buttonExit);
			this->Controls->Add(this->buttonStart);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"TitleForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TitleForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void buttonStart_Click(System::Object^ sender, System::EventArgs^ e) {
		ModelForm^ f = gcnew ModelForm;
		f->Owner = this;
		f->Show();
		this->Hide();
	}
	private: System::Void buttonExit_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	};
}
