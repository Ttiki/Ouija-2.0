#include "UltraEngine.h"
#include "Global.h"

using namespace UltraEngine;

bool RescaleUI(const Event& event, shared_ptr<Object> extra)
{
	float dpiscale = float(event.data) / 100.0f;
	auto ui = dynamic_pointer_cast<Interface>(extra);
	ui->SetScale(dpiscale);
	auto window = dynamic_pointer_cast<Window>(event.source);
	window->SetShape(event.position.x, event.position.y, event.size.x, event.size.y);
	return true;
}

int main(int argc, const char* argv[])
{
	//Variables
	const short border = 4;
	short charOnLine = 0;
	short yPos = 0;
	int charLibraryIndex = 0;
	int charLibrarySize;
	const int STATUSBARHEIGHT = 32;
	WString charLibrary[] = {
	L"a",
	L"b",
	L"c",
	L"d",
	L"e",
	L"f",
	L"g",
	L"h",
	L"i",
	L"j",
	L"k",
	L"l",
	L"m",
	L"n",
	L"o",
	L"p",
	L"q",
	L"r",
	L"s",
	L"t",
	L"u",
	L"v",
	L"w",
	L"x",
	L"y",
	L"z",
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L",",
	L".",
	L"!",
	L"?",
	L" ",
	};
	// We calculate the new array size
	charLibrarySize = my_sizeof(charLibrary) / my_sizeof(charLibrary[0]);


	//Get displays
	auto displays = GetDisplays();
	if (displays.empty()) return 1;
	float dpiscale = displays[0]->scale;

	//Create window
	auto mainwindow = CreateWindow("Ouija 2.0", 0, 0, 1024, 632, displays[0], WINDOW_HIDDEN | WINDOW_CENTER | WINDOW_TITLEBAR | WINDOW_RESIZABLE);
	mainwindow->SetMinSize(1024, 632);

	//Create user interface
	auto ui = CreateInterface(mainwindow);
	iVec2 sz = ui->root->ClientSize();

	//==================================================
	// 	   GUI CODE HERE
	//==================================================
	//Create Menu
	auto menu = CreateMenu("", ui->root);

	//Create menu items
	auto menu_file = CreateMenu(L"Fichier", menu);
	const auto menu_new = CreateMenu(L"Nouveau", menu_file);
	auto menu_save = CreateMenu(L"Sauvegarder", menu_file);
	CreateMenu("", menu_file);
	auto menu_openLibrary = CreateMenu(L"Ouvrir bilbiothèque", menu_file);
	CreateMenu("", menu_file);
	auto menu_exit = CreateMenu(L"Quitter", menu_file);

	/*auto menu_tools = CreateMenu(L"Outils", menu);
	const auto menu_option = CreateMenu(L"Option", menu_tools);*/

	//auto pausePlayBtn = CreateButton(L"⏸️", border, yPos, 56, 56, mainPanel, BUTTON_TOGGLE);
	////⏸️ ▶️
	//pausePlayBtn->SetFontScale(1.75);
	//pausePlayBtn->SetLayout(1, 0, 0, 1);

	auto mainPanel = CreatePanel(border, menu->size.y + border, sz.x - border * 2, sz.y - menu->size.y - border * 2 - STATUSBARHEIGHT, ui->root, PANEL_BORDER);
	mainPanel->SetLayout(1, 1, 1, 1);

	//CURRENT CHAR CODE
	yPos = border;
	auto currentChar = CreateLabel(L"", (mainPanel->size.x - 96) / 2, yPos, 96, 96, mainPanel, LABEL_CENTER | LABEL_MIDDLE | LABEL_BORDER);
	currentChar->SetFontScale(5);
	currentChar->SetLayout(1, 1, 1, 0);
	yPos += 96 + border;
	auto stopBtn = CreateButton(L"Écrire", border, yPos + border * 2, mainPanel->size.x - border * 3, 56, mainPanel);
	stopBtn->SetFontScale(1.75);
	stopBtn->SetLayout(1, 1, 1, 0);
	currentChar->SetText(charLibrary[charLibraryIndex]);

	//TEXT AREA
	yPos += 56 + border * 2;
	auto textAreaSave = CreateTextArea(border, yPos + border, mainPanel->size.x - border * 2, 256, mainPanel, TEXTAREA_WORDWRAP);
	textAreaSave->SetLayout(1, 1, 1, 1);
	//textAreaSave->SetFontScale(1.75);

	//Frequency slider
	yPos += 256 + border * 2;
	auto frequencyPanel = CreatePanel(0, yPos, mainPanel->size.x - border, 96, mainPanel, PANEL_GROUP);
	frequencyPanel->SetLayout(1, 1, 0, 1);
	frequencyPanel->SetText(L"Fréquence (ms)");
	auto label_frequency = CreateLabel(L"0 ms", frequencyPanel->size.x - 96, 0, 96, 64, frequencyPanel);
	label_frequency->SetLayout(1, 1, 1, 0);
	auto frequencySlider = CreateSlider(0, 32, frequencyPanel->size.x - 42, 32, frequencyPanel, SLIDER_HORIZONTAL);
	frequencySlider->SetLayout(1, 1, 0, 1);
	frequencySlider->SetRange(MIN_FREQUENCY, MAX_FREQUENCY);
	frequencySlider->SetValue(1000);
	label_frequency->SetText(to_wstring(frequencySlider->GetValue()) + L" ms");

	//-------------------------------------------------------
	// Create status bar
	//-------------------------------------------------------

	auto statusbar = CreatePanel(0, sz.y - STATUSBARHEIGHT, sz.x, STATUSBARHEIGHT, ui->root);
	statusbar->SetLayout(1, 1, 0, 1);
	auto statusbarlabel_view = CreateLabel(L"Playing", border, 0, 300, statusbar->size.y, statusbar, LABEL_LEFT | LABEL_MIDDLE);
	auto statusbarlabel_version = CreateLabel(VERSION, statusbar->size.x - (300 + border), 0, 300, statusbar->size.y, statusbar, LABEL_RIGHT | LABEL_MIDDLE);
	statusbarlabel_version->SetLayout(0, 1, 0, 1);

	auto charTimer = CreateTimer(frequencySlider->GetValue());

	//Enable DPI scaling changes
	ui->SetScale(dpiscale);
	ListenEvent(EVENT_WINDOWDPICHANGED, mainwindow, RescaleUI, ui);

	//Show the window
	mainwindow->Show();
	mainwindow->Activate();

	while (true)
	{
		const Event event = WaitEvent();
		switch (event.id)
		{
		case EVENT_WIDGETSELECT:
			break;
		case EVENT_WIDGETACTION:
			if (event.source == menu_new)
			{
				if (Confirm(L"Réinitialiser l'interface?", L"Réinitialiser Ouija 2.0", true) == 1) {
					mainwindow->Restore();
					charLibraryIndex = 0;
					charLibrarySize = my_sizeof(charLibrary) / my_sizeof(charLibrary[0]);
					textAreaSave->SetText(L"");
				}
			}
			//OPENING NEW LIBRARY
			else if (event.source == menu_openLibrary)
			{
				//OPEN THE .TXT FILE
				WString newLibraryPath;
				newLibraryPath = RequestFile(L"Ouvrir une nouvelle bibliothèque de charactère", "", "Text Files(*.txt) :txt");
				Print(newLibraryPath);

				//OPEN THE STREAM FROM THE FILE
				auto stream = ReadFile(newLibraryPath);
				if (stream == NULL)
				{
					Print("Failed to write file.");
					Notify("Erreur ! Impossible d'ouvrir le fichier. Vérifiez son format (.TXT).");

					break;
				}

				//READING THE FILE
				while (!stream->EOF())
				{
					WString currentChar = stream->ReadLine(); //Reading the current line to get the current char
					Print(currentChar);
					charLibrary->append(currentChar);
				}

				stream->Close();

				//We calculate the new array size
				charLibrarySize = my_sizeof(charLibrary) / my_sizeof(charLibrary[0]);
				Print("New array size " + to_wstring(charLibrarySize));
			}
			else if (event.source == menu_save) //Save text area content to a .txt file
			{
				WString savePath;
				savePath = RequestFile("Sauvegarder le fichier", "", "Text Files(*.txt) :txt", 0, true);

				//OPEN THE STREAM FROM THE FILE
				auto stream = WriteFile(savePath);
				if (stream == NULL)
				{
					Print("Failed to write file.");
					break;
				}
				stream->WriteWString(textAreaSave->GetText());

			}
			else if (event.source == menu_exit)
			{

				if (Confirm("Quitter le programme?")) return 0;
			}

			//Frequency slider moved
			//Change the frequency of the timer by voiding it and creating a new one with the new frequency.
			if (event.source == frequencySlider)
			{
				label_frequency->SetText(to_wstring(frequencySlider->GetValue()) + L" ms");
				charTimer = NULL;
				charTimer = CreateTimer(frequencySlider->GetValue());
			}

			//Stop button pressed
			//If pressed, we had the char to the text area.
			if (event.source == stopBtn) {
				textAreaSave->AddText(charLibrary[charLibraryIndex - 1]);
			}
			//Play / Pause button pressed
			//If pressed, we stop / restart the timer.
			/*if (event.source == pausePlayBtn) {

				if (pausePlayBtn->GetState() == false) {
					pausePlayBtn->SetText(L"⏸️");
					charTimer = NULL;
					charTimer = CreateTimer(frequencySlider->GetValue());
					statusbarlabel_view->SetText(L"Playing...");
				}
				else {
					pausePlayBtn->SetText(L"▶️");

					charTimer = NULL;
					charTimer = CreateTimer(-1);
					statusbarlabel_view->SetText(L"Pause !");
				}

			}*/
			break;
		case EVENT_TIMERTICK:
			if (charLibraryIndex >= charLibrarySize) {
				charLibraryIndex = 0;
			}

			if (charLibrary[charLibraryIndex] == L" ") {
				currentChar->SetText(L" ");
				//currentChar->SetText(L"˽");
			}
			else {
				currentChar->SetText(charLibrary[charLibraryIndex]);
			}

			//currentChar->Redraw();

			charLibraryIndex++;

			break;
		case EVENT_WINDOWCLOSE:
			if (event.source == mainwindow) return 0;
			break;
		}

	}
	return 0;
}