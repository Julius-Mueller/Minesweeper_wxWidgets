#pragma once
#include "wx/wx.h"
#include "board.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	int nFieldWidth = 10;
	int nFieldHeight = 10;
	int nMines = 12;
	// Define Array of pointers to buttons:
	wxButton** btn;
	bool bFirstClick = true;
	bool bLost = false;
	bool bWon = false;
	board minefield = board(nFieldWidth, nFieldHeight, nMines);

	void OnButtonClicked(wxCommandEvent& evt);
	void UpdateBtns();

	wxDECLARE_EVENT_TABLE();
};

