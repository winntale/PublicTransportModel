#pragma once
#include "ClassContainer.h"

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
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;

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
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// buttonTaxiSpawn
			// 
			this->buttonTaxiSpawn->Location = System::Drawing::Point(1287, 220);
			this->buttonTaxiSpawn->Name = L"buttonTaxiSpawn";
			this->buttonTaxiSpawn->Size = System::Drawing::Size(259, 95);
			this->buttonTaxiSpawn->TabIndex = 0;
			this->buttonTaxiSpawn->Text = L"Создать объект машины такси";
			this->buttonTaxiSpawn->UseVisualStyleBackColor = true;
			this->buttonTaxiSpawn->Click += gcnew System::EventHandler(this, &ModelForm::buttonTaxiSpawn_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1190, 31);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(356, 31);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Укажите цвет машины такси";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(1070, 128);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(476, 31);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Укажите макс. скорость машины такси";
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
			this->pictureBox1->Size = System::Drawing::Size(1025, 861);
			this->pictureBox1->TabIndex = 10;
			this->pictureBox1->TabStop = false;
			// 
			// buttonExit
			// 
			this->buttonExit->Location = System::Drawing::Point(1413, 801);
			this->buttonExit->Name = L"buttonExit";
			this->buttonExit->Size = System::Drawing::Size(159, 48);
			this->buttonExit->TabIndex = 11;
			this->buttonExit->Text = L"Выйти";
			this->buttonExit->UseVisualStyleBackColor = true;
			this->buttonExit->Click += gcnew System::EventHandler(this, &ModelForm::buttonExit_Click);
			// 
			// timeAction
			// 
			this->timeAction->Enabled = true;
			this->timeAction->Interval = 50;
			this->timeAction->Tick += gcnew System::EventHandler(this, &ModelForm::timeAction_Tick);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1235, 434);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 31);
			this->label3->TabIndex = 12;
			this->label3->Text = L"label3";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(1323, 434);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(82, 31);
			this->label4->TabIndex = 13;
			this->label4->Text = L"label4";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(1235, 386);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(82, 31);
			this->label5->TabIndex = 15;
			this->label5->Text = L"label5";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(1323, 386);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(82, 31);
			this->label6->TabIndex = 14;
			this->label6->Text = L"label6";
			// 
			// ModelForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize = System::Drawing::Size(1584, 861);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->buttonExit);
			this->Controls->Add(this->colorValue);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->maxVelocityValue);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->buttonTaxiSpawn);
			this->Controls->Add(this->pictureBox1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
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
		if (env->TaxiCars->Count) {  }
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
				// Bitmap^ taxiImg = gcnew Bitmap("..\\forPTM\\bus-export-export.png");

				   if (env->TaxiCars[i]->direction::get() == "left") { taxiImg->RotateFlip(RotateFlipType::Rotate270FlipNone); }
				   if (env->TaxiCars[i]->direction::get() == "right") { taxiImg->RotateFlip(RotateFlipType::Rotate90FlipNone); }
				   if (env->TaxiCars[i]->direction::get() == "down") { taxiImg->RotateFlip(RotateFlipType::Rotate180FlipNone); }

				   _graf->DrawImage(taxiImg, env->TaxiCars[i]->xPos::get(), env->TaxiCars[i]->yPos::get(), 32, 32);
				   delete taxiImg;
			   }
		   }

		   Void ScreenUpdater() {
			   graf = graf->FromImage(myBitmap);
			   graf->Clear(Color::White);
			   DrawTaxiCars(graf);
			   myBitmap->MakeTransparent(Color::White);
			   pictureBox1->Image = myBitmap;
			   delete graf;
		   }

		   // TaxiSpawn (он прорисовывает первый кадр машины, спавнит объект) -> TaxiMove (он описывает изменениее координат изображения) ->
		   // -> ScreenUpdate (совмещает в себе все функции движения) -> TimerTick (регулирует частоту обновления экрана)

	private: System::Void buttonTaxiSpawn_Click(System::Object^ sender, System::EventArgs^ e) {
		if ((colorValue->SelectedIndex + 1) && !String::IsNullOrEmpty(maxVelocityValue->Text)) {
			env->TaxiSpawn(label5, label6);
			env->TaxiCars[env->TaxiCars->Count - 1]->color::set(GetColor(colorValue->SelectedIndex)); // отдаём созданному объекту значение поля цвет, выбранное в комбобоксе на форме
			env->TaxiCars[env->TaxiCars->Count - 1]->maxVelocity::set(Convert::ToInt16(maxVelocityValue->Text)); // то же самое для поля максимальная скорость
		}
		else if (!(colorValue->SelectedIndex + 1)) { 
			MessageBox::Show("Укажите цвет машины такси", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		else {
			MessageBox::Show("Укажите максимальную скорость машины", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		label3->Text = Convert::ToString(env->TaxiCars[env->TaxiCars->Count - 1]->npCrossroadIndex::get());
		label4->Text = Convert::ToString(env->TaxiCars[env->TaxiCars->Count - 1]->npVerticeIndex::get());
		// то есть перед отрисовкой машины такси имеем 4 заполненных поля
		// 1) координаты 2) направление 3) цвет 4) макс. скорость
		
		ScreenUpdater();
	}
	private: System::Void buttonExit_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void timeAction_Tick(System::Object^ sender, System::EventArgs^ e) {
		ScreenUpdater();
		env->TimerTickActions();
	}
};
}
