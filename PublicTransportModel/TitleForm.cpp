#include "TitleForm.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThread]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	PublicTransportModel::TitleForm form;
	Application::Run(% form);
}