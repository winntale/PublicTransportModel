#pragma once
#include "envClass.h"

namespace PublicTransportModel {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для ModelForm
	/// </summary>
	public ref class ModelForm : public System::Windows::Forms::Form
	{
		   Graphics^ graf;
		   Bitmap^ myBitmap;
	private: MyEnvironment^ env;
	private: System::Windows::Forms::Timer^ timeAction;
	private: System::Windows::Forms::Timer^ timerStopSpamming;








	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label12;
		   Bitmap^ background;

	public:
		ModelForm(void)
		{
			InitializeComponent();
			myBitmap = gcnew Bitmap(1600, 900);
			env = gcnew MyEnvironment();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~ModelForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ buttonTaxiSpawn;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ maxVelocityValue;

	private: System::Windows::Forms::ComboBox^ colorValue;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ buttonExit;

	private: System::ComponentModel::IContainer^ components;

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(ModelForm::typeid));
			this->buttonTaxiSpawn = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->maxVelocityValue = (gcnew System::Windows::Forms::TextBox());
			this->colorValue = (gcnew System::Windows::Forms::ComboBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->buttonExit = (gcnew System::Windows::Forms::Button());
			this->timeAction = (gcnew System::Windows::Forms::Timer(this->components));
			this->timerStopSpamming = (gcnew System::Windows::Forms::Timer(this->components));
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// buttonTaxiSpawn
			// 
			this->buttonTaxiSpawn->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->buttonTaxiSpawn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->buttonTaxiSpawn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonTaxiSpawn->ForeColor = System::Drawing::Color::Silver;
			this->buttonTaxiSpawn->Location = System::Drawing::Point(1287, 220);
			this->buttonTaxiSpawn->Name = L"buttonTaxiSpawn";
			this->buttonTaxiSpawn->Size = System::Drawing::Size(259, 95);
			this->buttonTaxiSpawn->TabIndex = 0;
			this->buttonTaxiSpawn->Text = L"Создать объект машины такси";
			this->buttonTaxiSpawn->UseVisualStyleBackColor = false;
			this->buttonTaxiSpawn->Click += gcnew System::EventHandler(this, &ModelForm::buttonTaxiSpawn_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::Silver;
			this->label1->Location = System::Drawing::Point(1190, 31);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(356, 31);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Укажите цвет машины такси";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::Silver;
			this->label2->Location = System::Drawing::Point(1210, 128);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(336, 31);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Укажите скорость машины";
			// 
			// maxVelocityValue
			// 
			this->maxVelocityValue->Location = System::Drawing::Point(1446, 162);
			this->maxVelocityValue->Name = L"maxVelocityValue";
			this->maxVelocityValue->Size = System::Drawing::Size(100, 39);
			this->maxVelocityValue->TabIndex = 3;
			// 
			// colorValue
			// 
			this->colorValue->FormattingEnabled = true;
			this->colorValue->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"жёлтый", L"белый", L"чёрный" });
			this->colorValue->Location = System::Drawing::Point(1425, 65);
			this->colorValue->Name = L"colorValue";
			this->colorValue->Size = System::Drawing::Size(121, 39);
			this->colorValue->TabIndex = 9;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(-1, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1165, 927);
			this->pictureBox1->TabIndex = 10;
			this->pictureBox1->TabStop = false;
			// 
			// buttonExit
			// 
			this->buttonExit->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->buttonExit->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonExit->ForeColor = System::Drawing::Color::Silver;
			this->buttonExit->Location = System::Drawing::Point(1413, 801);
			this->buttonExit->Name = L"buttonExit";
			this->buttonExit->Size = System::Drawing::Size(159, 48);
			this->buttonExit->TabIndex = 11;
			this->buttonExit->Text = L"Выйти";
			this->buttonExit->UseVisualStyleBackColor = false;
			this->buttonExit->Click += gcnew System::EventHandler(this, &ModelForm::buttonExit_Click);
			// 
			// timeAction
			// 
			this->timeAction->Enabled = true;
			this->timeAction->Interval = 50;
			this->timeAction->Tick += gcnew System::EventHandler(this, &ModelForm::timeAction_Tick);
			// 
			// timerStopSpamming
			// 
			this->timerStopSpamming->Interval = 500;
			this->timerStopSpamming->Tick += gcnew System::EventHandler(this, &ModelForm::timerStopSpamming_Tick);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 13));
			this->label11->Location = System::Drawing::Point(1180, 745);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(44, 24);
			this->label11->TabIndex = 20;
			this->label11->Text = L"way";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 13));
			this->label12->Location = System::Drawing::Point(1180, 779);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(42, 24);
			this->label12->TabIndex = 21;
			this->label12->Text = L"dirs";
			// 
			// ModelForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(73)),
				static_cast<System::Int32>(static_cast<System::Byte>(79)));
			this->ClientSize = System::Drawing::Size(1584, 927);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->buttonExit);
			this->Controls->Add(this->colorValue);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->maxVelocityValue);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->buttonTaxiSpawn);
			this->Controls->Add(this->pictureBox1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Margin = System::Windows::Forms::Padding(7);
			this->Name = L"ModelForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"ModelForm";
			this->Load += gcnew System::EventHandler(this, &ModelForm::ModelForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ModelForm_Load(System::Object^ sender, System::EventArgs^ e) {
		env->VerticesGen();
		timerStopSpamming->Interval = ANTISPAM_INTERVAL;
	}
		   // функция, возвращающая наименование цвета на английском. используется для обращения к опр. картинки машины такси (taxiCarYellow и т.д.)
		   String^ GetColor(int _colorIndex) {
			   switch (_colorIndex) {
			   case 0: // жёлтый
				   return "Yellow";
			   case 1: // белый
				   return "White";
			   case 2: // чёрный
				   return "Black";
			   }
		   }

		   
		   Void DrawTaxiCars(Graphics^ _graf) {
			   for (int i = 0; i < env->TaxiCars->Count; i++) {
				   Bitmap^ taxiImg = gcnew Bitmap(String::Format("..\\forPTM\\taxiCar{0}.png", env->TaxiCars[i]->color::get()));

				   if (env->TaxiCars[i]->direction::get() == "left") { taxiImg->RotateFlip(RotateFlipType::Rotate270FlipNone); }
				   if (env->TaxiCars[i]->direction::get() == "right") { taxiImg->RotateFlip(RotateFlipType::Rotate90FlipNone); }
				   if (env->TaxiCars[i]->direction::get() == "down") { taxiImg->RotateFlip(RotateFlipType::Rotate180FlipNone); }

				   _graf->DrawImage(taxiImg, env->TaxiCars[i]->xPos::get(), env->TaxiCars[i]->yPos::get(), 32, 32);
				   delete taxiImg;
			   }
		   }

		   Void DrawPassengers(Graphics^ _graf) {
			   Random^ rndGen = gcnew Random();
			   for (int i = 0; i < env->Passengers->Count; i++) {
				   if (((env->Passengers[i]->state::get() != 2) && (env->Passengers[i]->state::get() != 3)) || ((env->Passengers[i]->state::get() == 3 && env->Passengers[i]->isMovingAway::get()))) {
					   Bitmap^ passengerImg = gcnew Bitmap(String::Format("..\\forPTM\\passenger{0}.png", env->Passengers[i]->color::get()));

					   if (env->Passengers[i]->endNode->direction == "left") { passengerImg->RotateFlip(RotateFlipType::Rotate270FlipNone); }
					   if (env->Passengers[i]->endNode->direction == "right") { passengerImg->RotateFlip(RotateFlipType::Rotate90FlipNone); }
					   if (env->Passengers[i]->endNode->direction == "down") { passengerImg->RotateFlip(RotateFlipType::Rotate180FlipNone); }

					   _graf->DrawImage(passengerImg, env->Passengers[i]->xPos::get(), env->Passengers[i]->yPos::get(), PASSENGER_HEIGHT, PASSENGER_HEIGHT);
					   delete passengerImg;
				   }
			   }
		   }

		   Void ScreenUpdater() {
			   graf = graf->FromImage(myBitmap);
			   graf->Clear(Color::White);
			   DrawTaxiCars(graf);
			   DrawPassengers(graf);
			   myBitmap->MakeTransparent(Color::White);
			   pictureBox1->Image = myBitmap;
			   delete graf;
		   }

		   // TaxiSpawn (он прорисовывает первый кадр машины, спавнит объект) -> TaxiMove (он описывает изменениее координат изображения) ->
		   // -> ScreenUpdate (совмещает в себе все функции движения) -> TimerTick (регулирует частоту обновления экрана)

	private: System::Void buttonTaxiSpawn_Click(System::Object^ sender, System::EventArgs^ e) {
		timerStopSpamming->Enabled = true;
		buttonTaxiSpawn->Enabled = false;
		buttonTaxiSpawn->ForeColor = Color::DimGray;

		if ((colorValue->SelectedIndex + 1) && !String::IsNullOrEmpty(maxVelocityValue->Text)) {
			env->TaxiSpawn();
			env->TaxiCars[env->TaxiCars->Count - 1]->color::set(GetColor(colorValue->SelectedIndex)); // отдаём созданному объекту значение поля цвет, выбранное в комбобоксе на форме
			env->TaxiCars[env->TaxiCars->Count - 1]->maxVelocity::set(Convert::ToInt16(maxVelocityValue->Text)); // то же самое для поля максимальная скорость
		}
		else if (!(colorValue->SelectedIndex + 1)) { 
			MessageBox::Show("Укажите цвет машины такси", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		else {
			MessageBox::Show("Укажите максимальную скорость машины", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		// то есть перед отрисовкой машины такси имеем 4 заполненных поля
		// 1) координаты 2) направление 3) цвет 4) макс. скорость
		
		ScreenUpdater();
	}
	private: System::Void buttonExit_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void timeAction_Tick(System::Object^ sender, System::EventArgs^ e) {
		ScreenUpdater();
		env->TimerTickActions(label11, label12);
	}
private: System::Void timerStopSpamming_Tick(System::Object^ sender, System::EventArgs^ e) {
	timerStopSpamming->Enabled = false;
	buttonTaxiSpawn->Enabled = true;
	buttonTaxiSpawn->ForeColor = Color::Silver;
	buttonTaxiSpawn->Cursor = Cursors::Hand;
}
};
}
