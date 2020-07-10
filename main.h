// ---------------------------------------------------------------------------

#ifndef mainH
#define mainH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TForm1 : public TForm {
__published: // IDE-managed Components
	TImageList *ImageList2;
	TButton *Button1;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TImage *img;

	void __fastcall N1Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);

private: // User declarations
public: // User declarations
	__fastcall TForm1(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
// ---------------------------------------------------------------------------
#endif
