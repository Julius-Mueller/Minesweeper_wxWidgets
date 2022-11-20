#include "cMain.h"
#include <string>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(600, 300), wxSize(400, 400))
{

	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	wxFont font(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain()
{
	delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;

	// Generate or unveil the minefield:
	if (bFirstClick)
	{
		minefield.generate(x, y);
		bFirstClick = false;
	}
	else
	{
		minefield.unveil(x, y);
	}

	// Check if user has triggered a mine:
	if (minefield.mines[y][x])
	{
		bLost = true;

	}
	else if (minefield.remaining() == nMines)
	{
		bWon = true;
	}
	UpdateBtns();
	if (bLost)
	{
		wxMessageBox("You perish! Try again!");
		minefield.reload();
		bFirstClick = true;
		bLost = false;
		bWon = false;
		UpdateBtns();
	}
	else if (bWon)
	{
		wxMessageBox("Congratulations, you survive!");
		minefield.reload();
		bFirstClick = true;
		bLost = false;
		bWon = false;
		UpdateBtns();
	}
	// Need to tell the event that it has been handled, otherwise OnClick-functions
	// of parent and parents-parent etc. will be called as well:
	evt.Skip();
}

void cMain::UpdateBtns()
{
	for (int i = 0; i < (nFieldHeight * nFieldWidth); ++i)
	{
		std::string label = "";
		int x = i % nFieldWidth;
		int y = i / nFieldHeight;
		if ((minefield.shroud[y][x] == 0) || bLost)
		{
			if (minefield.neighbors[y][x] > 0)
			{
				label = std::to_string(minefield.neighbors[y][x]);
			}
			if (minefield.mines[y][x])
			{
				label = "X";
			}
			btn[i]->Enable(false);
			btn[i]->SetLabel(label);
		}
		else
		{
			btn[i]->Enable(true);
			btn[i]->SetLabel("");
		}
	}
}