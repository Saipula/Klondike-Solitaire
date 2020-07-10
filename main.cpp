// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "class.h"
#include <ctime>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
Game *game;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
	srand(time(NULL));

	game = new Game(img, ImageList2);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender) {
	game = new Game(img, ImageList2);
	game->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender) {
	Button1->Visible = false;
	N1->Visible = true;
	game->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	game->Click(X, Y);

	if (game->Win()) {
		ShowMessage("Вы победили!");
		N1Click(Sender);
	}
}
// ---------------------------------------------------------------------------
